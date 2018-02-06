#include "openflow.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum ofp_type {
    OFPT_HELLO = 0,
    OFPT_ERROR = 1,
    OFPT_ECHO_REQUEST = 2,
    OFPT_ECHO_REPLY = 3,
};

static void handle_echo_request(client_t *);

void handle_ofp_packet(client_t *client) {
    switch (client->cur_packet->type) {
        case OFPT_HELLO:
            break;
        case OFPT_ECHO_REQUEST:
            handle_echo_request(client);
            break;
        default:
            printf("Got unexpected packet type 0x%02x\n",
                   client->cur_packet->type);
            break;
    }
}

void handle_echo_request(client_t *client) {
    uint16_t length;
    ofp_header_t *hdr;

    length = client->cur_packet->length;
    if ((hdr = malloc(length)) == NULL) {
        perror("malloc");
        exit(-1);
    }

    hdr->version = 0x04;
    hdr->type = OFPT_ECHO_REPLY;
    hdr->length = htons(length);
    memcpy(hdr->data, client->cur_packet->data, length);

    /* TODO: something smart */
    write(client->fd, hdr, length);
}
