//bbb arrombado, leo gay
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
    primeiro->mem_ptr = (void*)((char*)primeiro + sizeof(struct mem_block));//Colocamos char para fazer aritimética de ponteiros, apontando depois do cabeçalho
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
    	        //Divide blocos
    	    	struct mem_block* novo = (struct mem_block*)((char*)lido->mem_ptr + tamanho);
    	    	novo->is_free = 1;
    	    	novo->size = lido->size -tamanho -sizeof(struct mem_block); //tamanho anterior - tamanho armazenado - cabeçalho do novo bloco
    	    	novo->mem_ptr = (void*)((char*)novo + sizeof(struct mem_block));
    	    	novo->seguinte = lido->seguinte;//O final do lido é o mesmo final do novo
    	    	novo->anterior = lido;
    	    	
    	    	if(lido->seguinte != NULL)
    	    	{
    		    lido->seguinte->anterior = novo;//Atualizando a variavel anterior no bloco seguinte
    	    	}
    	    	lido->seguinte = novo;
    	    	lido->size = tamanho; 	//Atualizando a estrutura atual
    	    }	 
    	        lido->is_free = 0;
    	        return lido->mem_ptr; //Sai do while, retorno da função
    	}
    	    lido = lido->seguinte;
    }	
        //printf("Out of Memory");
        return NULL;
}

void sfree(void* ptr)
{
    struct mem_block* lido = primeiro;
    
    while(lido != NULL)
    {
    	if(lido->mem_ptr == ptr)
    	{
    	    lido->is_free = 1;
    	    //Coalescência
    	    if(lido->anterior != NULL && lido->anterior->is_free == 1)
    	    {
    	    	lido->mem_ptr = lido->anterior->mem_ptr;
    	    	lido->size = lido->size + lido->anterior->size + sizeof(struct mem_block);
    	    	lido->anterior = lido->anterior->anterior; // O anterior torna-se o anterior dele mesmo
    	    	if (lido->anterior != NULL) { 
                   lido->anterior->seguinte = lido; 
                }
    	    }
    	    
    	    if(lido->seguinte != NULL && lido->seguinte->is_free == 1)
    	    {
    	    	lido->size = sizeof(struct mem_block) + lido->seguinte->size;
    	    	lido->seguinte = lido->seguinte->seguinte;
    	    	if (lido->seguinte != NULL) {
                    lido->seguinte->anterior = lido;
                }
    	    }
        return;
    	}
    	lido = lido->seguinte;
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
    initialize_memory();
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
            allocations[allocCount].ptr = smalloc(size);
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
                sfree(allocations[allocCount].ptr);
                printf("Free %u bytes, total %u, position %i\n", size, currentMemory, allocCount);
            }
        }
    }

    return 0;
}




