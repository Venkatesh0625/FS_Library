#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>  
#include <netinet/in.h> 

#include "error.h"
#include "shared_memory.h"

#define BUFFER_SIZE 1024
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
	int server_fd,
		client_fd,
		valread,
		opt = 1,
		memory_index = 0;

	struct sockaddr_in address;  
	int addrlen = sizeof(address); 
	char buffer[1024] = {0};

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 

	key_t key = 456;
	int shm_id;
	struct memory *shm_ptr;

	create_memory(&key, &shm_id, shm_ptr);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
		error("socket failed"); 
		
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
		error("setsockopt"); 
		
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
		error("bind failed"); 

	if (listen(server_fd, 3) < 0) 
		error("listen"); 
		
	if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
		error("accept");

	while(1) {
		valread = recv( client_fd ,buffer, BUFFER_SIZE, 0);
		struct change *result = deserialize_structure(buffer);
		update_memory(shm_ptr, result, memory_index);
	}
	return 0; 
} 

