void sfree(void* ptr) {
    struct mem_block* current = primeiro;
    
    while (current != NULL) {
        if (current->mem_ptr == ptr) {
            current->is_free = 1;
            
            // Coalescência com o bloco anterior, se estiver livre
            if (current->prior != NULL && current->prior->is_free == 1) {
                current->mem_ptr = current->prior->mem_ptr;
                current->size += current->prior->size + sizeof(struct mem_block);
                current->prior = current->prior->prior;
                if (current->prior != NULL) {
                    current->prior->next = current;
                }
            }
            
            // Coalescência com o bloco seguinte, se estiver livre
            if (current->next != NULL && current->next->is_free == 1) {
                current->size += current->next->size + sizeof(struct mem_block);
                current->next = current->next->next;
                if (current->next != NULL) {
                    current->next->prior = current;
                }
            }
            
            return; // Termina a função após liberar o bloco e coalescê-lo
        }
        
        current = current->next; // Avança para o próximo bloco
    }
}
