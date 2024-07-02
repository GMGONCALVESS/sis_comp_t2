#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //incluir o sbrk

/* Maximum number of memory blocks that can be allocated */
#define MAX_ALLOCATIONS 1000  

/* Definindo um tamanho específico de memória para os blocos */
#define MEMORIA 1024

struct allocation_t{
    void *ptr;
    unsigned int size;
} allocations[MAX_ALLOCATIONS];

#pragma pack(push, 1)
struct mem_block
{
    int is_free;
    size_t size;
    void* mem_ptr;
    struct mem_block* seguinte;
    struct mem_block* anterior;
}; 
#pragma pack(pop)

struct mem_block* primeiro = NULL;

void initialize_memory()
{
    primeiro = (struct mem_block*)sbrk(sizeof(struct mem_block));//Aloca um ponteiro para um espaço da estrutura
    primeiro->is_free = 1;
    primeiro->size = MEMORIA - sizeof(struct mem_block);
    primeiro->mem_ptr = (void*)((char*)primeiro + sizeof(struct mem_block));//Colocamos char para fazer aritimética de ponteiros
    primeiro->seguinte = NULL;
    primeiro->anterior = NULL;
}

void* smalloc(size_t tamanho)
{
    struct mem_block* lido = primeiro;
    
    while(lido != NULL)
    {	
    	if(lido->is_free && lido->size >= tamanho) //garantindo que o bloco tá livre e tem tamanho suficiente
    	{
    	    if(lido->size > tamanho + sizeof(struct mem_block)) //garantindo q o bloco suporte o cabeçalho e o arquivo
    	    {
    	    }
     	 
    	}
    }	
}

int main(int argc, char *argv[]) {
    unsigned int currentMemory = 0;
    int allocCount = 0;
    unsigned int maxMemory;  
    unsigned int size;

    if (argc < 2){
        printf("Usage: %s <max memory in bytes>\n", argv[0]);
        return 1;
    }

    maxMemory = atoi(argv[1]);
    initialize_memory()
    srand(time(NULL));

    while(1){
        if (rand() % 2 == 0 && allocCount < MAX_ALLOCATIONS - 1){
            /* Allocate memory */
            size = rand() % 100;//file size
            if (currentMemory + size > maxMemory){
                printf("Out of memory\n");
                continue;
            }
            /* Change malloc() by smalloc() */
            allocations[allocCount].ptr = malloc(size);
            allocations[allocCount].size = size;
            currentMemory += size;
            allocCount++;
            printf("Allocated %u bytes, total %u, position %i\n", size, currentMemory, allocCount);
        }else{
            /* Free memory */
            if (allocCount > 0){
                allocCount--;
                size = allocations[allocCount].size;
                currentMemory -= size;
                /* Change free() by sfree() */
                free(allocations[allocCount].ptr);
                printf("Free %u bytes, total %u, position %i\n", size, currentMemory, allocCount);
            }
        }
    }

    return 0;
}




