#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Para sbrk()

#define MEMORY_SIZE 1024  // Tamanho fixo da memória simulada
#define MAX_ALLOCATIONS 1000

#pragma pack(push, 1)
struct mem_block {
    int isfree; // Se o bloco está livre: 1 se livre, 0 se ocupado.
    size_t size; // Tamanho do bloco de memória.
    void *memptr; // Ponteiro para o bloco de memória.
    struct mem_block *next; // Ponteiro para o próximo bloco na lista.
    struct mem_block *prior; // Ponteiro para o bloco anterior na lista.
};
#pragma pack(pop)

struct mem_block *head = NULL; // Cabeça da lista de blocos de memória

void initialize_memory() {
    head = (struct mem_block *)sbrk(sizeof(struct mem_block));
    head->isfree = 1;
    head->size = MEMORY_SIZE - sizeof(struct mem_block);
    head->memptr = (void *)((char *)head + sizeof(struct mem_block));
    head->next = NULL;
    head->prior = NULL;
}

// Função smalloc para alocar um novo bloco de memória
void* smalloc(size_t size) {
    struct mem_block *current = head;

    while (current != NULL) {
        if (current->isfree && current->size >= size) {
            if (current->size > size + sizeof(struct mem_block)) {
                // Dividir o bloco
                struct mem_block *new_block = (struct mem_block *)((char *)current->memptr + size);
                new_block->isfree = 1;
                new_block->size = current->size - size - sizeof(struct mem_block);
                new_block->memptr = (void *)((char *)new_block + sizeof(struct mem_block));
                new_block->next = current->next;
                new_block->prior = current;
                if (current->next != NULL) {
                    current->next->prior = new_block;
                }
                current->next = new_block;
                current->size = size;
            }
            current->isfree = 0;
            return current->memptr;
        }
        current = current->next;
    }
    printf("Out of memory\n");
    return NULL;
}

// Função para liberar o bloco de memória
void sfree(void *ptr) {
    struct mem_block *current = head;

    while (current != NULL) {
        if (current->memptr == ptr) {
            current->isfree = 1;

            // Coalescência com blocos adjacentes livres
            if (current->next != NULL && current->next->isfree) {
                current->size += sizeof(struct mem_block) + current->next->size;
                current->next = current->next->next;
                if (current->next != NULL) {
                    current->next->prior = current;
                }
            }
            if (current->prior != NULL && current->prior->isfree) {
                current->prior->size += sizeof(struct mem_block) + current->size;
                current->prior->next = current->next;
                if (current->next != NULL) {
                    current->next->prior = current->prior;
                }
            }
            return;
        }
        current = current->next;
    }
    printf("Pointer not found\n");
}

// Estrutura de alocação
struct allocation_t {
    struct mem_block *block;
} allocations[MAX_ALLOCATIONS];

int main(int argc, char *argv[]) {
    unsigned int currentMemory = 0;
    int allocCount = 0;
    unsigned int maxMemory;  
    unsigned int size;

    if (argc < 2) {
        printf("Usage: %s <max memory in bytes>\n", argv[0]);
        return 1;
    }

    maxMemory = atoi(argv[1]);

    srand(time(NULL));
    initialize_memory();

    while (1) {
        if (rand() % 2 == 0 && allocCount < MAX_ALLOCATIONS - 1) {
            /* Allocate memory */
            size = rand() % 100;
            if (currentMemory + size > maxMemory) {
                printf("Out of memory\n");
                continue;
            }
            allocations[allocCount].block = (struct mem_block *)smalloc(size);
            if (!allocations[allocCount].block) {
                printf("Failed to allocate memory\n");
                continue;
            }
            currentMemory += size;
            allocCount++;
            printf("Allocated %u bytes, total %u, position %i\n", size, currentMemory, allocCount);
        } else {
            /* Free memory */
            if (allocCount > 0) {
                allocCount--;
                size = allocations[allocCount].block->size;
                currentMemory -= size;
                sfree(allocations[allocCount].block->memptr);
                printf("Freed %u bytes, total %u, position %i\n", size, currentMemory, allocCount);
            }
        }
    }

    return 0;
}
