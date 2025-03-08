#include <stdio.h>
#include <stdbool.h>

#define HEAP_SIZE 2048

typedef struct Block {
    int size;
    int free;
    struct Block *next;
} Block;

static char memory[HEAP_SIZE];
static Block *heap = NULL;

void initialize_heap() {
    heap = (Block *)memory;
    heap->size = HEAP_SIZE - sizeof(Block);
    heap->free = 1;
    heap->next = NULL;
}

char* allocate(int size) {
    Block *current = heap;
    while (current) {
        if (current->free && current->size >= size) {
            if (current->size >= size + sizeof(Block)) {
                Block *new_block = (Block *)(current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;
                current->next = new_block;
            }
            current->size = size;
            current->free = 0;
            return (char*)current + sizeof(Block);
        }
        current = current->next;
    }
    return NULL;
}

void merge_free_blocks() {
    Block *current = heap;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void free_block(char *ptr) {
    if (!ptr || ptr < (char *)heap || ptr >= (char *)heap + HEAP_SIZE) return;

    Block *block = (Block *)(ptr - sizeof(Block));
    block->free = 1;
    merge_free_blocks();
}

int get_total_free_memory() {
    int total_free = 0;
    Block *current = heap;
    while (current) {
        if (current->free) {
            total_free += current->size;
        }
        current = current->next;
    }
    return total_free;
}

void display_heap() {
    printf("Heap Memory Status\n");
    Block *current = heap;
    while (current) {
        printf("Block at %p | Size: %d | %s\n", (void *)current, current->size, current->free ? "Free" : "Allocated");
        current = current->next;
    }
    printf("--------------------------------------\n");
    printf("Total Free Memory: %d bytes\n", get_total_free_memory());
}

int main() {
    initialize_heap();
    printf("Initial Heap State:\n");
    display_heap();

    char *ptr1 = allocate(150);
    char *ptr2 = allocate(120);
    printf("\nAfter Allocations:\n");
    display_heap();

    char *ptr3 = allocate(200);
    char *ptr4 = allocate(100);
    printf("\nAfter Allocations:\n");
    display_heap();

    free_block(ptr1);
    free_block(ptr2);
    printf("\nAfter Freeing Blocks:\n");
    display_heap();

    return 0;
}
