#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include <queue>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include "headers/error.h"
#include "headers/change.h"
#include "headers/file_chng.h"
#include "headers/serialize.h"

#define PORT 8088

using namespace std;

int sendc_data(int &, queue<struct change> &);

int init_client(int &server_fd, queue<change> &to_send, queue<change> &to_change)
{
    char host_addr[] = "127.0.0.1";
	char buffer[1024] = {0};
	char* file_buf;
	int valread; 
	struct sockaddr_in serv_addr; 

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
		error("Socket creation error");
	

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)  
		error("Invalid address/ Address not supported "); 
	

	if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
		error("Connection Failed"); 
		
	thread t(sendc_data, ref(server_fd), ref(to_send));

	while(true)
	{ 
		valread = recv(server_fd , buffer, 1024, 0); 
		struct change *result = deserialize_structure(buffer);

		if(result->type == REPLACE_FILE)
		{
			//file_buffer to store the received file content
			file_buf = (char *) malloc(result->line_number);

			//Size of the replacing file will be stored in line_number of structure received
			valread = recv( server_fd, file_buf, result->line_number, 0);
			replace_file(result->filename, file_buf);

			//Clearing memory
			delete file_buf;
		}

		else if(result->type == CREATE_N_REPLACE)
		{
			file_buf = (char *) malloc(result->line_number);

			//Creating a file using touch
			char *argument[] = {"touch", result->filename};
        	execvp(argument[0], argument);

			//file_buffer to store the received file content
			file_buf = (char *) malloc(result->line_number);

			//Size of the replacing file will be stored in line_number of structure received
			valread = recv( server_fd, file_buf, result->line_number, 0);
			replace_file(result->filename, file_buf);

			//Clearing memory
			delete file_buf;
		}
		else
			//Push into the queue to send 
			to_change.push(*result);
		struct change data = *result;
		cout<<data.type<<"\t"<<data.line_number<<"\t"<<data.filename<<"\t"<<data.line_data<<endl<<endl;
	}

	t.join();
	
} 

int sendc_data(int &server_fd, queue<struct change> &que)
{
	// while(true)
	// {
	// 	while(que.empty());

	// 	while(!que.empty())
	// 	{
	// 		char buffer[1024] = {0};
	// 		serialize_structure(buffer, &que.front());
	// 		send(server_fd, buffer, 1024, 0);
	// 		que.pop();
	// 	}
	// }
    long siz;
    FILE* ptr = fopen("watch.c","r");
    fseek(ptr, 0, SEEK_END);
    siz = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    char file_buf[siz+1] = {0};
    fread(file_buf, 1, siz, ptr); 
    file_buf[siz] = 0;
    fclose(ptr);
    struct change data = {CREATE_N_REPLACE,siz+1,"file.txt","from client"};
    char buffer[1024] = {0};
    serialize_structure(buffer, &data); 
    send(server_fd, buffer, 1024, 0); 
    send(server_fd, file_buf, siz+1, 0); 
}

#endif
int main()
{
    queue <struct change> to_send;
	queue <struct change> to_change;
    int watch_fd, watch_wd;
    int server_fd, client_fd;
    
	thread t1(init_client, ref(server_fd), ref(to_send), ref(to_change));

	t1.join();
}

