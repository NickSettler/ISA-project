/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: error.c
 * @date: 18.11.2023
 */

#include "error.h"

void processResponseError(int errorCode) {
    switch (errorCode) {
        case 1:
            ERROR_DNS_RESPONSE_FORMAT
        case 2:
            ERROR_DNS_RESPONSE_SERVER_FAILURE
        case 3:
            ERROR_DNS_RESPONSE_NX_DOMAIN
        case 4:
            ERROR_DNS_RESPONSE_NOT_IMP
        case 5:
            ERROR_DNS_RESPONSE_REFUSED
        case 6:
            ERROR_DNS_RESPONSE_X_DOMAIN
        case 7:
            ERROR_DNS_RESPONSE_X_RR_SET
        case 8:
            ERROR_DNS_RESPONSE_NOT_AUTH
        case 9:
            ERROR_DNS_RESPONSE_NOT_ZONE
        default:
            return;
    }
}