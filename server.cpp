#include <unistd.h> 
#include <iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>

#define BUFFER_SIZE 1024
#define PORT 8080 

void error(char* err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) 
{ 
	int server_fd,
		client_fd,
		valread,
		opt = 1; 

	struct sockaddr_in address;  
	int addrlen = sizeof(address); 
	char buffer[1024] = {0};
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 

	// Creating socket file descriptor 
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
		printf("%d %d %s",chg.line_number,chg.type,chg.line_data);
	}
	return 0; 
} 

