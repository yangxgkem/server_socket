//gcc -o socket server_socket.c socket_server.c main.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "server_socket.h"

static void
create_thread(pthread_t *thread, void *(*start_routine) (void *), void *arg) {
    if (pthread_create(thread,NULL, start_routine, arg)) {
        fprintf(stderr, "Create thread failed");
        exit(1);
    }
}

static void *
_client_socket(void *p) {
    uint32_t source = 1002;
    int reserve_id;

    reserve_id = server_socket_connect(source, "192.168.247.130", 8001);

    char* msg = "hello server socket";
    size_t sz = strlen(msg);
    char * buffer = malloc(sz+1);
    memcpy(buffer, msg, sz+1);

    server_socket_send(1002, reserve_id, buffer, sz);

    return NULL;
}

int
socket_checkdata(uint32_t source, struct server_socket_message * sm) {
    printf("\n-----------------------------\n");
    int type = sm->type;
    int sz = (int)sizeof(sm);
    int id = sm->id;
    int ud = sm->ud;
    char * buffer = sm->buffer;
    char * data = (char*)(sm+1);
    printf("source:%d, message_sz:%d, id:%d, ud:%d\n", source, sz, id, ud);

    switch (type) {
    case SERVER_SOCKET_TYPE_DATA://socket数据处理
        printf("SERVER_SOCKET_TYPE_DATA. \n");
        printf("buffer: %s\n", buffer);
        break;
    case SERVER_SOCKET_TYPE_CLOSE://关闭socket
        printf("SERVER_SOCKET_TYPE_CLOSE. \n");
        break;
    case SERVER_SOCKET_TYPE_CONNECT://服务器socket开启
        printf("SERVER_SOCKET_TYPE_CONNECT. \n");
        printf("data: %s\n", data);
        break;
    case SERVER_SOCKET_TYPE_ERROR://socket出错
        printf("SERVER_SOCKET_TYPE_ERROR. \n");
        break;
    case SERVER_SOCKET_TYPE_ACCEPT://客户端连入服务器
        printf("SERVER_SOCKET_TYPE_ACCEPT. \n");
        printf("data: %s\n", data);
        server_socket_start(source, ud);
        break;
    default:
        printf("Unknown socket message type %d. \n",type);
        break;
    }
    printf("\n-----------------------------\n");

    return 1;
}

int
main(int argc, char *argv[]) {
    
    uint32_t source = 1001;
    int reserve_id;

    server_socket_init();
    reserve_id = server_socket_listen(source, NULL, 8001, 1024);
    server_socket_start(source, reserve_id);
    
    pthread_t pid;
    create_thread(&pid, _client_socket, "_client_socket");

    for(;;){
        int r = server_socket_poll();
        if (r==0)
            break;
        if (r<0) {
            continue;
        }
    }

    return 0;
}