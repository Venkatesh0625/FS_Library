#include <stdio.h>  
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h> 
#include "change.h"
#define OPEN 0
#define CLOSED 1


struct change chg;

struct memory {
	short int status;
	struct change changes[100];
}*ptr;


void error(char* err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

void create_memory(int uniq_id, int *shm_id, struct memory *ptr)
{
    int id;
    key_t key = (int) uniq_id;

	id = (int) shmget(key, sizeof(struct memory),IPC_CREAT | 0666);
	if(id < 0)
		error("*** shmget error (server) ***\n");
	
	ptr = (struct memory*) shmat(id, NULL, 0);
	if((int) ptr < 0) 
		error("*** shmat error (server) ***\n");
	
	printf("Server has attached the shared memory...\n");

    *shm_id = id;
}

void update_memory(struct memory *ptr,struct change value, int index)
{
    while(ptr->status == CLOSED);
    ptr->changes[index] = value;
    ptr->status = OPEN;
    printf("\n status: %d \t %d %d %s",
            ptr->status,
            ptr->changes[index].line_number,
            ptr->changes[index].type,
            ptr->changes[index].line_data);
}

void close_memory(int *shm_id, struct memory *ptr)
{
    printf("Server has detected the completion of its child...\n");
    shmdt((void *)ptr);
    printf("Server has detached its shared memory...\n");
    shmctl(*shm_id, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
}

int main()
{
    int shm_id;
    chg.line_number = 9;
    key_t key = 345;
    chg.type = 2;
    create_memory(123,&shm_id, ptr);
    int i;
    for(i = 0;i<10;i++)
    {
        scanf("%s",chg.line_data);
        update_memory(ptr,chg,i);
    }
    close_memory(&shm_id,ptr);
}