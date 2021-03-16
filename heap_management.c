#include<stdio.h>
#include<stdlib.h>

typedef struct data_heap
{
    int block_id;
    int process_id;
    int data_size;
    struct data_heap *prev;
    struct data_heap *next;
}Heap_block;

typedef struct free_heap
{
    int block_id;
    int flag;
    int free_memory;
    struct free_heap *prev;
    struct free_heap *next;
}free_block;

void create_heap(int size,free_block **fpptr)
{
    free_block *nptr=(free_block*)malloc(sizeof(free_block));
    if(nptr!=NULL&&size!=0)
    {
        nptr->prev=NULL;
        nptr->next=NULL;
        nptr->free_memory=size;
        nptr->flag=1;
        if(*fpptr==NULL)
        {
            nptr->block_id=0;
            *fpptr=nptr;
        }
        else{
            nptr->block_id=(*fpptr)->block_id+1;
            (*fpptr)->next=nptr;
            nptr->prev=*fpptr;
            *fpptr=nptr;
        }
    }
}
void allocate(int i,int size,free_block *free_ptr,Heap_block **hpptr,Heap_block **h_fpptr)
{
    int loop_flag=1;
    if(size)
    {
        while(free_ptr!=NULL&&loop_flag)
        {
            if(free_ptr->free_memory>=size)//first fit allocation
            {
                Heap_block *nptr=(Heap_block*)malloc(sizeof(Heap_block));
                if(nptr!=NULL)
                {
                    nptr->block_id=free_ptr->block_id;
                    nptr->data_size=size;
                    free_ptr->free_memory=free_ptr->free_memory-size;
                    nptr->prev=NULL;
                    nptr->next=NULL;
                    nptr->process_id=i;
                    if(!free_ptr->free_memory)//no space in memory
                    {
                        free_ptr->flag=0;
                    }
                    if(*hpptr==NULL)
                    {
                        *hpptr=nptr;
                        *h_fpptr=nptr;
                    }
                    else
                    {
                       (*hpptr)->next=nptr;
                       nptr->prev=*hpptr;
                       *hpptr=nptr;
                    }
                    loop_flag=0;
                }
            }
            else
            {
                free_ptr=free_ptr->next;
            }
        }
        if(free_ptr==NULL&&loop_flag)//no block found to store it
        {
            printf("No space in the heap to allocate size:%d using first fit\n",size);
        }
    }
}
void free_memory(int p_no,free_block *free_ptr,Heap_block **p_ptr,Heap_block **hpptr)
{
    Heap_block *start_ptr=*p_ptr;
    Heap_block *next_ptr=NULL,*prev_ptr=NULL;
    int loop_flag=1,flag2=1;
    while(start_ptr!=NULL&&loop_flag)//to iterate memory space
    {
        if(start_ptr->process_id==p_no)
        {
            while(free_ptr!=NULL&&flag2)//for iterating the space allocated to the user
            {
                if(start_ptr->block_id==free_ptr->block_id)
                {
                    free_ptr->flag=1;
                    free_ptr->free_memory=free_ptr->free_memory+start_ptr->data_size;
                    flag2=0;
                }
                else{
                    free_ptr=free_ptr->next;
                }
            }
            if(start_ptr->prev==NULL)//first node to be deleted
            {
                *p_ptr=(*p_ptr)->next;
                if(start_ptr->next!=NULL)//not single node
                {
                    (*p_ptr)->prev=NULL;
                }
            }
            if(start_ptr->next==NULL)//last node to be deleted
            {
                *hpptr=(*hpptr)->prev;
                if(start_ptr->prev!=NULL)//not single node case
                {
                    (*hpptr)->next=NULL;
                }
            }
            else
            {
                next_ptr=start_ptr->next;
                next_ptr->prev=start_ptr->prev;
                if(start_ptr->prev!=NULL)//not first node
                {
                    prev_ptr=start_ptr->prev;
                    prev_ptr->next=next_ptr;
                }
            }
            free(start_ptr);
            loop_flag=0;
        }
        else
        {
            start_ptr=start_ptr->next;
        }
    }
}
void print_allocated_segment(Heap_block *nptr)
{
	printf("Block_id      process_id    data_size\n");
	while(nptr!=NULL)
	{
		printf(" %d  		%d	   %d\n",nptr->block_id,nptr->process_id,nptr->data_size);
        nptr=nptr->next;
	}
}
void print_memory_segment(free_block *nptr)
{
    printf("Block_id      flag    free_memory\n");
    while(nptr!=NULL)
    {
        printf(" %d  		%d	   %d\n",nptr->block_id,nptr->flag,nptr->free_memory);
        nptr=nptr->next;
    }
}
int main()
{
    int i,n1,n2;
    printf("Enter no of blocks available for allocating memory:");
    scanf("%d",&n1);
    int blocks_available[n1];
    for(i=0;i<n1;i++)
    {
        scanf("%d",&blocks_available[i]);
    }
    printf("Enter no of blocks to be allocated:");
    scanf("%d",&n2);
    int to_allocate[n2];
    for(i=0;i<n2;i++)
    {
        scanf("%d",&to_allocate[i]);
    }
    free_block *fptr,*ptr;
    fptr=NULL;
    ptr=NULL;
    for(i=0;i<n1;i++)
    {
        create_heap(blocks_available[i],&fptr);
        if(!i)
        {
            ptr=fptr;
        }
    }
    Heap_block *h_ptr,*h_fptr;
    h_ptr=NULL;
    h_fptr=NULL;
    for(i=0;i<n2;i++)
    {
        allocate(i,to_allocate[i],ptr,&h_ptr,&h_fptr);
    }
    int p_no;
    printf("Enter process_id:");
    scanf("%d",&p_no);
    free_memory(p_no,ptr,&h_fptr,&h_ptr);//to free memory allocated in heap

    printf("\nMemory available in heap after deletion\n");
    print_memory_segment(ptr);
    return 0;
}
