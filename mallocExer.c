#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>

#define TOTAL 8192                                                               

typedef struct memNode{                                                             //memory nodes representing memory blocks
    size_t      LEN;                                                                //requested memory size
    int         status;                                                             //0 indicates free and 1 indicates used
    int         PLEN;               
    struct memNode *PREV;
    struct memNode *NEXT;
} memNode;

    memNode *head;                                                                  
    struct memNode *head8;
    struct memNode *head16;
    struct memNode *head32;
    struct memNode *head64;
    struct memNode *head128;
    struct memNode *freelist;
    struct memNode *last;
    size_t memorySize = 8192;
    struct memNode *pusher8;
    struct memNode *pusher16;
    struct memNode *pusher32;
    struct memNode *pusher64;
    struct memNode *pusher128;

void init(){                                                                        //initializing the environment
    int i, x, y, z, q;

    head = sbrk(TOTAL); 
    head->status = 0;                                                                   
    head->LEN = 8 + sizeof(memNode);
    head->NEXT = NULL;
    head->PREV = NULL;

    last = head;

    struct memNode *block8;                                                         //pointers which will be used to iterate throught my double linked list
    struct memNode *block16;
    struct memNode *block32;
    struct memNode *block64;
    struct memNode *block128;

    head8 = last;
    for (i = 1; i < 10; i++){                                                        //innitiated the first 10 nodes which contains 8 bytes of memory
        block8 = (struct memNode *)((void *)last + (sizeof(memNode)) + 8 + 1);
        block8->status = 0;
        block8->LEN = 8 + sizeof(memNode);
        block8->PREV = last;
        block8->NEXT = NULL;
        last->NEXT = block8;
        last = block8; 
    }

    head16 = last;
    for (x = 0; x < 10; x++){                                                        //innitiated the next 10 nodes which contains 16 bytes of memory
        block16 = (struct memNode *)((void *)last + (sizeof(memNode)) + 16 + 1);
        if (x == 0) head16 = block16;
        block16->status = 0;
        block16->LEN = 16 + sizeof(memNode);
        block16->PREV = last;
        block16->NEXT = NULL;
        last->NEXT = block16;
        last = block16; 
    }

    head32 = last;                                                                   //innitiated the next 10 nodes which contains 32 bytes of memory
    for (y = 0; y < 10; y++){
        block32 = (struct memNode *)((void *)last + (sizeof(last)) + 32 + 1);
        if (y == 0) head32 = block32;
        block32->status = 0;
        block32->LEN = 32 + sizeof(memNode);
        block32->PREV = last;
        block32->NEXT = NULL;
        last->NEXT = block32;
        last = block32; 
    }
        head64 = last;
    for (z = 0; z < 10; z++){                                                        //innitiated the next 10 nodes which contains 64 bytes of memory
        block64 = (struct memNode *)((void *)last + (sizeof(last)) + 64 + 1);
        if (z == 0) head64 = block64;
        block64->status = 0;
        block64->LEN = 64 + sizeof(memNode);
        block64->PREV = last;
        block64->NEXT = NULL;
        last->NEXT = block64;
        last = block64; 
    }

        head128 = last;
    for (q = 0; q < 10; q++){                                                       //innitiated the next 10 nodes which contains 128 bytes of memory
        block128 = (struct memNode *)((void *)last + (sizeof(last)) + 128 + 1);
        if (q == 0) head128 = block128;
        block128->status = 0;
        block128->LEN = 128 + sizeof(memNode);
        block128->PREV = last;
        block128->NEXT = NULL;
        last->NEXT = block128;
        last = block128; 
    }
        freelist = (struct memNode *)((void *)last + (sizeof(memNode)));            //stored critical addresses which will be used later on
        freelist->PREV = last;
        freelist->NEXT = NULL;

        pusher8 = head8;
        pusher16 = head16;
        pusher32 = head32;
        pusher64 = head64;
        pusher128 = head128;
}


void splitIntoFreelist(struct memNode *head_ptr, size_t remain){                    //splitting function, we use this function to split a consumed node into 2, and add the remaining bytes to our freelist
    freelist = (struct memNode *)((void *)last + (sizeof(memNode)) + remain);
    freelist->LEN = remain + sizeof(memNode);
    freelist->status = 0;
    freelist->NEXT = head_ptr;
    head_ptr = freelist;
    printf("The block has %lu of bytes left, which is then splitted and placed in a freelist at the location of %p\n", remain, freelist);
}

void bubbleSort(struct memNode *a, struct memNode *b){                              //bubble sorting method which the freelist will use to sort its nodes based on their size in bytes, which will be helpful for allocating memory in best fit later on
    size_t temp = a->LEN;
    a->LEN = b->LEN;
    b->LEN = temp;
}

int typeConv(char *input){                                                          //method which converts a char array into integers, will be used for malloc input
    char newOut[100];
    memcpy(newOut, input+1, sizeof(input));
    int x = atoi(newOut);
    return x;
}

void coalesce(struct memNode *a){                                                   //method to free a node
    printf("%p\n", a);
    a->LEN = 0;
    printf("The address at : %p is freed.\n", a);
    printf("\n");
    printf("===============================================================================\n");
    printf("\n");

}

    
void *new_malloc(size_t size){                                                      //malloc

    struct memNode *iterator;
    struct memNode *freelistHeap;
    struct memNode *ptr1;
    int input;
    freelistHeap = freelist;
    int swapped;

    if (size <= 0)                              
    {
        printf("Invalid input as size is equals or smaller than 0.\n");
        return NULL;
    }

        do                                                                          //try to allocate memory to the freelist in prior to memory nodes, using best fit method
        {   
            swapped = 0;
            iterator = freelist;
            ptr1 = NULL;
            while (iterator->NEXT != ptr1)
            {
                if (iterator->LEN > iterator->NEXT->LEN)
                {   
                    bubbleSort(iterator, iterator->NEXT);
                    swapped = 1;
                }
                    iterator = iterator->NEXT;           
            }
            ptr1 = iterator;
        }
        while (swapped);     

        switch (size)                                                               //if requested memory is too large for any nodes in the free list to hold, use the 100 list of memory blocks i created instead.
        {
            case 1 ... 8:     
                pusher8->LEN = size;     
                printf("\n");
                printf("===============================================================================\n");
                printf("\n");
                printf("Memory allocated at >>>>>>>>> %p\n", pusher8);
                printf("With the size of : %zu, total size including 32 bytes of metadata is : %zu.\n", size, size+sizeof(memNode));
                printf("\n");
                if(size > 1 && size < 8 && ((8 - size)!=0)) { splitIntoFreelist(pusher8, 8-size); }
                pusher8 = pusher8->NEXT;
                memorySize = memorySize - size - sizeof(memNode);
                return pusher8->PREV;

            break;
                
            case 9 ... 16:
                printf("\n");
                printf("===============================================================================\n");
                printf("\n");
                pusher16->LEN = size;
                printf("Memory allocated at >>>>>>>>> %p\n", pusher16);
                printf("With the size of : %zu, total size including 32 bytes of metadata is : %zu.\n", size, size+sizeof(memNode));
                printf("\n");
                if(size > 8 && size < 8 && ((8 - size)!=0)) { splitIntoFreelist(pusher16->PREV, 16-size); }
                pusher16 = pusher16->NEXT; 
                memorySize = memorySize - size - sizeof(memNode);
                return pusher16->PREV;

            break;

            case 17 ... 32:
                pusher32->LEN = size;
                printf("\n");
                printf("===============================================================================\n");
                printf("\n");
                printf("Memory allocated at >>>>>>>>> %p\n", pusher32);
                printf("With the size of : %zu, total size including 32 bytes of metadata is : %zu.\n", size, size+sizeof(memNode));
                printf("\n");
                if(size > 16 && size < 32 && ((32 - size)!=0)) { splitIntoFreelist(pusher32->PREV, 32-size); }
                pusher32 = pusher32->NEXT;
                memorySize = memorySize - size - sizeof(memNode);
                return pusher32->PREV;

            break;

            case 33 ... 64:
                printf("\n");
                printf("===============================================================================\n");
                printf("\n");
                pusher64->LEN = size;
                printf("Memory allocated at >>>>>>>>> %p\n", pusher64);
                printf("With the size of : %zu, total size including 32 bytes of metadata is : %zu.\n", size, size+sizeof(memNode));
                printf("\n");
                if(size > 33 && size < 64 && ((64 - size)!=0)) { splitIntoFreelist(pusher64->PREV, 64-size); }
                pusher64 = pusher64->NEXT;
                memorySize = memorySize - size - sizeof(memNode);
                return pusher64->PREV;    

            break;

            case 65 ... 124:
                printf("\n");
                printf("===============================================================================\n");
                printf("\n");
                pusher128->LEN = size;       
                printf("Memory allocated at >>>>>>>>>> %p\n", pusher128);
                printf("With the size of : %zu, total size including 32 bytes of metadata is : %zu.\n", size, size+sizeof(memNode));
                printf("\n");
                if(size > 65 && size < 128 && ((128 - size)!=0)) { splitIntoFreelist(pusher128->PREV, 128-size); }
                pusher128 = pusher128->NEXT;
                memorySize = memorySize - size - sizeof(memNode);
                return pusher128->PREV;
            break;            
        }
}


void new_free(void *ptr){                                                               //free method
    coalesce(ptr);
}

void display(){                                                                         //display the current memory nodes and left over total memory
    printf("\n");
    printf("                          Total memory: %ld\n", memorySize - sizeof(memNode));
    printf("\n");
    while (head->NEXT != NULL ){
        if(head->NEXT != NULL && head->NEXT->NEXT != NULL && head->NEXT->NEXT->NEXT != NULL) {
            printf("%p-%ld | %p-%ld | %p-%ld\n", head, (head->LEN - sizeof(memNode)), head->NEXT, ((head->NEXT->LEN) -sizeof(memNode)), head->NEXT->NEXT, (head->NEXT->NEXT->LEN-sizeof(memNode)));
            head = head->NEXT->NEXT->NEXT;
        }
        else if(head->NEXT != NULL && head->NEXT->NEXT != NULL) {
            printf("%p-%ld | %p-%ld ", head, (head->LEN - sizeof(memNode)), head->NEXT, ((head->NEXT->LEN) -sizeof(memNode)));
            head = head->NEXT->NEXT;
        }
        else if(head->NEXT != NULL){
            printf("%p-%ld | %p-%ld \n", head, (head->LEN - sizeof(memNode)), head->NEXT, ((head->NEXT->LEN) -sizeof(memNode)));
            head = head->NEXT;
        }
    }
}

int main(int argc, char *argv[]){                                                           //main function
    init();         

    struct memNode *someptr;
    struct memNode *replacement;
    printf("-----------------------------------------------------------------------\n");
    display();
    printf("\n");
    printf("===============================================================================\n");
    printf("\n");
    char input[100];
    char ps[100];

    while (true)
    {
        printf("How much memory are you wish to request? Please input in a format as A<bytes> for memory allocation. \n");
        printf("Note: PLEASE ONLY ENTER BYTES BETWEEN 0 - 128.\n");
        scanf("%s", input);
        someptr = new_malloc(typeConv(input));
        printf("Returned address after malloc is at: %p\n", someptr);
        printf("===============================================================================\n");

        display();        
        printf("===============================================================================\n");
        printf("To free this memory, please input in a format as <addr>.  \n");
        printf("Note: WHIlE INPUT, F and 0x IS NOT REQUIRED, WRONG INPUT FORMAT WILL CAUSE ERRORS.\n");

        scanf("%p", &replacement);
        new_free(replacement);
    }
}