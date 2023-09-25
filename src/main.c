/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: main.c
 * @date: 18.11.2023
 */

#include <stdlib.h>
#include "args.h"
#include "req.h"

int main(int argc, char *argv[]) {
    config_t config;
    parseArgs(&config, argc, argv);
    validateArgs(&config);

    int socket = openSocket(&config);

    request(socket, &config);

    return EXIT_SUCCESS;
}