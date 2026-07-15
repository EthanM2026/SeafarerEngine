#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Packet sent from Client to Server
typedef struct {
    char key;
} InputPacket;

// Packet sent from Server to all Clients
typedef struct {
    float p1_x, p1_y;
    float p2_x, p2_y;
} StatePacket;

struct _Seafarer_Multiplayer_Server
{

};

int match_addr(struct _Seafarer_Multiplayer_Server* SMS, struct sockaddr_in a, struct sockaddr_in b);

#endif // SERVER_H_INCLUDED
