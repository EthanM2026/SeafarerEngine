#include "client.h"
#include "shared.h"

void set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void network_update(struct _Seafarer_Engine_Network_Client* SENC) {
    StatePacket state_pkt;

    while (1) {
        // Peek at the stream to see how many bytes are currently waiting
        int bytes_available = recv(SENC->sock_fd, &state_pkt, sizeof(StatePacket), MSG_PEEK);

        // Handle server disconnection
        if (bytes_available == 0) {
            printf("Server disconnected gracefully.\n");
            exit(0);
        }

        // If there isn't a full StatePacket ready yet, don't touch it!
        // Break and wait until the next frame.
        if (bytes_available < (int)sizeof(StatePacket)) {
            break;
        }

        // Since we peeked and know a full packet is ready, read it for real
        int bytes_read = 0;
        while (bytes_read < sizeof(StatePacket)) {
            int ret = recv(SENC->sock_fd, ((char*)&state_pkt) + bytes_read, sizeof(StatePacket) - bytes_read, 0);
            if (ret <= 0) {
                printf("Server disconnected during stream read.\n");
                exit(0);
            }
            bytes_read += ret;
        }

        // Apply the updated world state
        if (state_pkt.type == MSG_STATE) {
            printf("Updating State\n");
            memcpy(SENC->local_players, state_pkt.players, sizeof(SENC->local_players));
        }
    }
}

void handle_input(struct _Seafarer_Engine_Network_Client* SENC,unsigned char key, int x, int y) {
    Packet move_pkt;
    move_pkt.type = MSG_MOVE;
    move_pkt.x = 0.0f;
    move_pkt.y = 0.0f;

    float speed = 0.05f;

    switch (key) {
        case 'w': move_pkt.y = speed; break;
        case 's': move_pkt.y = -speed; break;
        case 'a': move_pkt.x = -speed; break;
        case 'd': move_pkt.x = speed; break;
        case 27: exit(0); // ESC key
    }

    // Send input request to the server instead of moving locally
    if (move_pkt.x != 0.0f || move_pkt.y != 0.0f) {
        send(SENC->sock_fd, &move_pkt, sizeof(Packet), 0);
    }
}


int Connect_IPv4(struct _Seafarer_Engine_Network_Client* SENC,const char* IPv4_Address, unsigned short PORT, const char* Username) {
    struct sockaddr_in serv_addr;
    if ((SENC->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IPv4_Address, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(SENC->sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connection successful!\n");
    // Send the join packet containing the username right away
    // Send the join packet containing the username right away
    Packet join_pkt;
    join_pkt.type = MSG_JOIN;
    strncpy(join_pkt.username, Username, MAX_NAME);
    send(SENC->sock_fd, &join_pkt, sizeof(Packet), 0);

    // 1. Read the welcome packet synchronously (blocking) FIRST
    Packet welcome_pkt;
    int bytes_received = 0;
    while (bytes_received < sizeof(Packet)) {
        int ret = recv(SENC->sock_fd, ((char*)&welcome_pkt) + bytes_received, sizeof(Packet) - bytes_received, 0);
        if (ret <= 0) {
            perror("Failed to receive welcome packet from server");
            return -1;
        }
        bytes_received += ret;
    }

    if (welcome_pkt.type == MSG_WELCOME) {
        SENC->my_id = welcome_pkt.player_id;
        printf("Connected successfully! Assigned Player ID: %d\n", SENC->my_id);
    } else {
        printf("Unexpected packet type received during handshake: %d\n", welcome_pkt.type);
        return -1;
    }

    // 2. NOW it is safe to turn on non-blocking mode for our game loops!
    set_non_blocking(SENC->sock_fd);
    return 1;
}
