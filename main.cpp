#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <map>
#include <queue>
#include <thread>

#include "headers/change.h"
#include "headers/server.h"
#include "headers/file_chng.h"
#include "headers/get_changes.h"
#include "headers/watch_dog.h"
#include "headers/client.h"

using namespace std;

int main(int argc, char **argv)
{
    queue <struct change>   to_send, to_change;

    map<int, char*> directories;

    int watch_fd, watch_wd;

    char host_addr[16] = {0};

    int server_fd, client_fd;

    if(argv[2])

    if(strcmp(argv[1], "start") == 0) 
    {
        thread sock(init_server, ref( server_fd ), ref( client_fd ), ref( to_send ), ref( to_change ));   
    }
    else if(strcmp(argv[2], "sync") == 0)
    { 
        if(strcmp(argv[2])
            strcpy(host_addr, argv[2]);
        
        thread sock(init_client, ref( server_fd ), host_addr, ref( to_send ), ref( to_change ));
        struct change temp = ""
    }

}