#include<stdio.h>
#include<stdlib.h>

int Total_Size=1024;

struct freeL
{
	int front;
	int size;
	struct freeL *prev;
	struct freeL *next;
};

struct allocateL
{
	int front;
	int size;
	struct allocateL *prev;
	struct allocateL *next;
	int Label;
};

typedef struct freeL FreeList;
typedef struct allocateL AllocatedList;

FreeList *FreeHead;
AllocatedList *Allochead;

void MergeFreeBlock(FreeList *newnode);

void ALLOCATE(int n,int name)
{
	FreeList *p=FreeHead;
	while(p && p->size<n)
	{
		p=p->next;
	}
	if(p==NULL)
	{
		printf("\nMemory can't be allocated");
	}
	else{
		if(p->size>n)
		{
			FreeList *newnode;
			newnode=(FreeList *)malloc(sizeof(struct freeL));
			newnode->front=p->front+n;
			newnode->prev=newnode->next=NULL;
			
			if(p==FreeHead)
			{
				newnode->next=FreeHead->next;
				FreeHead=newnode;
			}
			else{
				(p->prev)->next=newnode;
				newnode->next=p->next;
				newnode->prev=p->prev;
				if(p->next)
				{
					p->next->prev=newnode;
				}
			}
			newnode->size=p->size-n;
		}
		else{
			if (p->prev)
            {
                p->prev->next = p->next;
                if(p->next)
                    p->next->prev = p->prev;
            }
            else
            {
                FreeHead = p->next;
                if(FreeHead)
                FreeHead->prev = NULL;
            }
		}
		
		AllocatedList *alloc1 = (AllocatedList *)malloc(sizeof(struct allocateL));
        alloc1->next = alloc1->prev = NULL;
        alloc1->size = n;
        alloc1->Label = name;
        alloc1->front = p->front;
        free(p);
        
        if (Allochead == NULL)
            Allochead = alloc1;
        else
        {
            AllocatedList *temp = Allochead;
            while (temp->next && temp->front < alloc1->front)
            {
                temp = temp->next;
            }
            
            if (temp == Allochead && temp->front > alloc1->front)
            {
                alloc1->next = temp;
                temp->prev = alloc1;
                Allochead = alloc1;
            }
            else if (temp->next == NULL && temp->front < alloc1->front)
            {
                temp->next = alloc1;
                alloc1->prev = temp;
                alloc1->next = NULL;
            }
            else
            {
                (temp->prev)->next = alloc1;
                alloc1->prev = temp->prev;
                alloc1->next = temp;
                temp->prev = alloc1;
            }
        }

        printf("\n\nMemory is allocated from index %d to %d\n",alloc1->front,alloc1->front+alloc1->size);
	}
}

void DEALLOCATE(int name)
{
	AllocatedList *p=Allochead;
	
	while( p && p->Label!=name)
	{
		p=p->next;
	}
	if(p==NULL)
	{
		printf("\nNode with name %d is not found",name);
	}
	else{
		FreeList *newnode=(FreeList *)malloc(sizeof(struct freeL));
		newnode->prev=newnode->next=NULL;
		newnode->size=p->size;
		newnode->front=p->front;
		
		if(p==Allochead)
		{
			Allochead=p->next;
		}	
		else{
			p->prev->next=p->next;
			if(p->next)
			{
				p->next->prev=p->prev;
			}
		}
		
		FreeList *temp=FreeHead;
		if(FreeHead==NULL)
		{
			FreeHead=newnode;
		}
		else{
			while(temp && temp->next && temp->front < newnode->front)
			{
				temp=temp->next;
			}
			
			if(temp && temp->next==NULL && temp->front < newnode->front)
			{
				temp->next=newnode;
				newnode->prev=temp;
			}
			else{
				if(temp && temp==FreeHead)
				{
					newnode->next = temp;
                    temp->prev = newnode;
                    FreeHead = newnode;
				}
				else if(temp)
				{
					newnode->prev = temp->prev;
                    temp->prev->next = newnode;
                    temp->prev = newnode;
                    newnode->next = temp;
				}
			}
		}
		MergeFreeBlock(newnode);
	}
}

void MergeFreeBlock(FreeList *newnode)
{
	if(newnode->prev)
	{
		if(newnode->prev->front + newnode->prev->size == newnode->front)
		{
			newnode->prev->size = newnode->prev->size + newnode->size;
		    newnode->prev->next = newnode->next;
            if(newnode->next)
                newnode->next->prev = newnode->prev;
            FreeList *temp = newnode;
            newnode = newnode->prev;
            free(temp);
		}
	}
	
	if(newnode->next)
	{
		if(newnode->front + newnode->size == newnode->next->front)
		{
			newnode->size += newnode->next->size;
		    newnode->next = newnode->next->next;
            if(newnode->next)
                newnode->next->prev = newnode;  
		}
	}
	
}

int main()
{
    int flag=0,choice,choice2,size,name;

	FreeHead = (FreeList *)malloc(sizeof(struct freeL));
	FreeHead->front=0;
	FreeHead->size=Total_Size;
	FreeHead->prev=FreeHead->next=NULL;
	
	Allochead=NULL;
	
	printf("Implementation of Heap Management using First Fit Strategy with available memory of 1024 units.\n\n");
	
	while(flag==0)
	{
		printf("Enter the choice:\n 1 for ALLOCATE\n 2 for DEALLOCATE\n 3 to stop the program.\n\n");
		scanf("%d",&choice);
		
		if(choice==3)
		{
			flag=1;
			printf("\n\nProgram is stopped.");
		}
		else if(choice==1)
		{
			int flag2=0;
			printf("\nEnter the size to allocate :");
			scanf("%d",&size);
			printf("\nEnter label for allocated block (label is integer so please give input as integer):");
			scanf("%d",&name);
			
			AllocatedList *p=Allochead;
			while(p && flag2==0)
			{
				if(p->Label==name)
				{
					printf("\nMemory location with same label is found so previous will be deallocated.");
					DEALLOCATE(name);
					flag2=1;
				}
				p=p->next;
			}
			ALLOCATE(size,name);
		}
		else if(choice==2)
		{
			printf("\nEnter label of block to deallocate/delete :");
			scanf("%d",&name);
			DEALLOCATE(name);
		}
		else{
			printf("\n\n Wrong choice input.\nProgram stopped.");
			flag=1;
		}
		
		AllocatedList *m=Allochead;
		FreeList *n=FreeHead;
		
		printf("\nAllocated List :\n");
		while(m)
        {
        	printf("%d to %d\n",m->front,m->front+m->size);
            m=m->next;
        }
        
        printf("\nFree List :\n");
        while(n)
        {
        	printf("%d to %d\n",n->front,n->front+n->size);
        	n=n->next;
		}
		
		printf("\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");
	}
	
	return 0;
	
}
