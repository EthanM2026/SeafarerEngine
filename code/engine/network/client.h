#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <GL/glut.h>
#include "shared.h"

struct _Seafarer_Engine_Network_Client
{
    int sock_fd = 0;
    int my_id = -1;
    Player local_players[MAX_PLAYERS];
};

struct _Seafarer_Engine_Network_Client* Create_Seafarer_Engine_Network_Client();
void Initialize_Seafarer_Engine_Network_Client(struct _Seafarer_Engine_Network_Client* SENC);

void set_non_blocking(int fd);
void network_update(struct _Seafarer_Engine_Network_Client* SENC);
void handle_input(struct _Seafarer_Engine_Network_Client* SENC,unsigned char key, int x, int y);
int Connect_IPv4(struct _Seafarer_Engine_Network_Client* SENC,const char* IPv4_Address, unsigned short PORT, const char* Username);

#endif

