#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <stddef.h>
#include <stdint.h>

#define SERVER_SOCKET_TYPE_DATA 1
#define SERVER_SOCKET_TYPE_CONNECT 2
#define SERVER_SOCKET_TYPE_CLOSE 3
#define SERVER_SOCKET_TYPE_ACCEPT 4
#define SERVER_SOCKET_TYPE_ERROR 5

struct server_socket_message {
	int type;
	int id;
	int ud;
	char * buffer;
};

struct server_context;

void server_socket_init();
void server_socket_exit();
void server_socket_free();
int server_socket_poll();

int server_socket_send(uint32_t source, int id, void *buffer, int sz);
void server_socket_send_lowpriority(uint32_t source, int id, void *buffer, int sz);
int server_socket_listen(uint32_t source, const char *host, int port, int backlog);
int server_socket_connect(uint32_t source, const char *host, int port);
int server_socket_bind(uint32_t source, int fd);
void server_socket_close(uint32_t source, int id);
void server_socket_start(uint32_t source, int id);
void server_socket_nodelay(uint32_t source, int id);

#endif
