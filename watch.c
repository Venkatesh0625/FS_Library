#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>

#define MAX_EVENTS 1024 /*Max. number of events to process at one go*/
#define LEN_NAME 1024 /*Assuming length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*buffer to store the data of events*/

char *filename[100];
int status[100];
void get_event (int fd) {
    char buffer[BUF_LEN];
    int length, i = 0,j=0;

    length = read( fd, buffer, BUF_LEN );
    if ( length < 0 ) {
        perror( "read" );
    }
while ( i < length ) {
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
        if ( event->len ) {
            if ( event->mask & IN_CREATE) {
                if (!(event->mask & IN_ISDIR))
                {
                        filename[j]=event->name;
                        status[j]=1;
                        printf("%s %d\n",filename[j],status[j]);
                        j++;
                }
        }

            if ( event->mask & IN_MODIFY) {
                if (!(event->mask & IN_ISDIR))
                 {
                        filename[j]=event->name;
                        status[j]=2;
                        printf("%s %d\n",filename[j],status[j]);
                        j++;
                }
        }

            if ( event->mask & IN_DELETE) {
                if (!(event->mask & IN_ISDIR))
                {
                        filename[j]=event->name;
                        status[j]=3;
                        printf("%s %d\n",filename[j],status[j]);
                        j++;
                }
        }
            i += EVENT_SIZE + event->len;
        }
    }
}

int main( int argc, char **argv ) {
    int wd, fd;

    fd = inotify_init();
    if ( fd < 0 ) {
        perror( "Couldn't initialize inotify");
    }

    wd = inotify_add_watch(fd, argv[1], IN_ACCESS | IN_OPEN | IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) {
        printf("Couldn't add watch to %s\n",argv[1]);
    } else {
        printf("Watching:: %s\n",argv[1]);
    }

    /* do it forever*/
    while(1) {
     get_event(fd);
    }

    /* Clean up*/
    inotify_rm_watch( fd, wd );
    close( fd );

    return 0;
}
