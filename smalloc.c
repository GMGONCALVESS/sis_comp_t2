#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main(unsigned int size)
{
		
	printf("Ola mundo");


}
