#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <queue>
#include <limits.h>
#include <unistd.h>

#include "headers/change.h"
#include "headers/get_changes.h"

using namespace std;

/*Max. number of events to process at one go*/
#define MAX_EVENTS 1024 
/*Assuming length of the filename won't exceed 16 bytes*/
#define LEN_NAME 1024 
/*size of one event*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) 
/*buffer to store the data of events*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) 

void show_queue(queue<struct change> que)
{
    cout<<"show_queue";
    while(!que.empty())
    {
        struct change data = que.front();
        cout<<data.type<<"\t"<<data.line_number<<"\t"<<data.filename<<"\t"<<data.line_data<<endl;
        que.pop();
    }
}
int is_equal(struct inotify_event* x, struct inotify_event* y)
{
    return (int) ( (strcmp(x->name, y->name) == 0) && x->len == y->len && x->mask == x->mask && x->wd == x->wd && x->cookie == y->cookie); 
}

int assign(struct inotify_event* dest, struct inotify_event* src)
{
    dest->cookie = src->cookie;
    dest->len = src->len;
    dest->mask = src->mask;
    dest->wd = src->wd;
    strcpy(dest->name, src->name);
}

void get_events(int fd, queue <struct change> &que) 
{
    while(1)
    {
        char buffer[BUF_LEN];
        int length, i = 0,j=0;

        length = read(fd, buffer, BUF_LEN);
        if (length < 0) 
            perror( "read" );

        while (i < length) 
        {
            struct inotify_event *curr = (struct inotify_event *) &buffer[i];

            if(curr->len)
            {
                if (curr->mask & IN_CREATE && (!(curr->mask & IN_ISDIR))) 
                {
                    cout<<curr->name<<"\t"<<"IN_CREATE"<<endl;
                    struct change data = {0,0,0,0};
                    strcpy(data.filename, curr->name);
                    data.type = CREATE_FILE;
                    que.push(data);
                }

                if (curr->mask & IN_MODIFY && (!(curr->mask & IN_ISDIR))) 
                {
                    cout<<curr->name<<"\t"<<"IN_MODIFY"<<endl;
                    get_changes(curr->name, que);
                    show_queue(que);
                }

                if (curr->mask & IN_DELETE && (!(curr->mask & IN_ISDIR))) 
                {
                    cout<<curr->name<<"\t"<<"IN_DELETE"<<endl;
                    struct change data = {0,0,0,0};
                    strcpy(data.filename, curr->name);
                    data.type = DELETE_FILE;
                    cout<<"Queue Data\n";
                    que.push(data);
                }


                i += (EVENT_SIZE + curr->len);
            }
        }
    }
}

void watch_dog(char* dir,int fd,int wd, queue<struct change> &que)
{
    fd = inotify_init();
    if ( fd < 0 )
        perror( "Couldn't initialize inotify");

    wd = inotify_add_watch(fd, dir, IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) 
        printf("Couldn't add watch to %s\n",dir);
    else 
        printf("Watching:: %s\n",dir);

    get_events(fd, que);

    inotify_rm_watch( fd, wd );

    close( fd );
}

