#ifndef COMMON_H
#define COMMON_H

#define MAX_PLAYERS 12
#define MAX_NAME 32

typedef struct {
    int id;
    char username[MAX_NAME];
    double x, y, z;
    double pitch, yaw;
    int active;
} Player;

typedef enum {
    MSG_JOIN,      // Client -> Server: "I want to join with this name"
    MSG_WELCOME,   // Server -> Client: "Welcome, your ID is X and your position is Y"
    MSG_MOVE,      // Client -> Server: "I want to move by dx, dy"
    MSG_STATE      // Server -> Client: "Here is the current world state"
} MsgType;

// Standard communication packet
typedef struct {
    MsgType type;
    int player_id;
    char username[MAX_NAME];
    double x, y, z;
    double pitch, yaw;
} Packet;

// World state broadcast packet
typedef struct {
    MsgType type;
    int player_count;
    Player players[MAX_PLAYERS];
} StatePacket;

#endif
