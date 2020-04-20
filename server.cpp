
#include <iostream>
#include <queue>
#include <thread>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>  
#include <netinet/in.h> 

#include "headers/change.h"
#include "headers/error.h"
#include "headers/serialize.h"
#include "headers/file_chng.h"

using namespace std;

#define BUFFER_SIZE 1024
#define PORT 8088

int show_q(queue<struct change>  q)
{
	while(!q.empty())
	{
		struct change data = q.front();
		cout<<data.type<<"\t"<<data.line_number<<"\t"<<data.filename<<"\t"<<data.line_data<<endl;
		q.pop();
	}
}
int send_data(int &, queue<struct change> &);

int init_socket(int &server_fd, int &client_fd, queue<struct change> &to_send, queue<struct change> &to_change)
{

	int	valread,
		opt = 1,
		memory_index = 0;
	
	struct sockaddr_in address;  
	int addrlen = sizeof(address); 
	char buffer[1024] = {0},*file_buf;

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
		perror("socket failed"); 
		
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
		perror("setsockopt"); 
		
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
		perror("bind failed"); 

	if (listen(server_fd, 3) < 0) 
		perror("listen"); 
		
	if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
		perror("accept");
	cout<<"\naccepted\n";

	thread t(send_data, ref(client_fd), ref(to_send));
	while(1) 
	{
		valread = recv( client_fd ,buffer, BUFFER_SIZE, 0);
		struct change *result = deserialize_structure(buffer);

		if(result->type == REPLACE_FILE)
		{
			//file_buffer to store the received file content
			file_buf = (char *) malloc(result->line_number);

			//Size of the replacing file will be stored in line_number of structure received
			valread = recv( client_fd, file_buf, result->line_number, 0);
			replace_file(result->filename, file_buf);

			//Clearing memory
			delete file_buf;
		}
		else if(result->type == CREATE_N_REPLACE)
		{
			cout<<"got";
			file_buf = (char *) malloc(result->line_number);

			//Creating a file using touch
			char *argument[] = {"touch", result->filename};
			if(!fork())
        		execvp(argument[0], argument);
			else
			{
				wait(NULL);
				cout<<"did";

				//Size of the replacing file will be stored in line_number of structure received
				valread = recv( client_fd, file_buf, result->line_number, 0);
				cout<<file_buf;
				replace_file(result->filename, file_buf);

				//Clearing memory
				delete file_buf;
			}
		}
		else
			//Push into the queue to send 
			to_change.push(*result);
		struct change data = *result;
		cout<<data.type<<"\t"<<data.line_number<<"\t"<<data.filename<<"\t"<<data.line_data<<endl<<endl;
	}
	t.join();
}

int send_data(int &client_fd, queue<struct change> &que)
{
	while(true)
	{
		//wait until queue is not empty
		while(que.empty());
		//iterate to empty the queue
		while(!que.empty())
		{
			char buffer[BUFFER_SIZE] = {0};
			serialize_structure(buffer, &que.front());
			send(client_fd, buffer, BUFFER_SIZE, 0);
			que.pop();
		}
	}
}

int main()
{
    queue <struct change> to_send;
	queue <struct change> to_change;
    int watch_fd, watch_wd;
    int server_fd, client_fd;
	thread t1(init_socket, ref(server_fd), ref(client_fd), ref(to_send), ref(to_change));
	while(true)
	{
		struct change data = {1,2,"fh","from server"};
		sleep(1);
		for(int i = 0;i<10;i++)
			to_send.push(data);
	}
	t1.join();
}