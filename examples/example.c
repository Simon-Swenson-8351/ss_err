#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#include "../src/ss_err.h"

ss_err_t *swap_bytes(uint8_t *buf1, size_t buf1_size, uint8_t *buf2, size_t buf2_size) {
    if(!buf1)
        SS_ERR_RETURN(&SS_ERR_TYPE_DOMAIN, __FILE__, __func__, __LINE__, "");
    if(!buf2)
        SS_ERR_RETURN(&SS_ERR_TYPE_DOMAIN, __FILE__, __func__, __LINE__, "buf2 was null");
    if(buf1_size != buf2_size)
        SS_ERR_RETURN(&SS_ERR_TYPE_DOMAIN, __FILE__, __func__, __LINE__, "buffers were different sizes");
    for(size_t i = 0; i < buf1_size; i++) {
        uint8_t tmp = buf2[i];
        buf2[i] = buf1[i];
        buf1[i] = tmp;
    }
    return NULL;
}

enum { ARY_SIZE = 7 };
int main(int argc, char *argv[]) {
    ss_err_t *e;
    int buf1[ARY_SIZE] = {0, 1, 2, 3, 4, 5, 6};
    int buf2[ARY_SIZE] = {2, 3, 5, 7, 11, 13, 17};
    size_t buf1_size = sizeof(int) * ARY_SIZE;
    size_t buf2_size = sizeof(int) * ARY_SIZE;
    if(e = swap_bytes(NULL, buf1_size, (uint8_t *)buf2, buf2_size))
        printf("%s\n", e->str);
    if(e = swap_bytes(NULL, buf1_size, (uint8_t *)buf2, buf2_size))
        printf("%s\n", e->str);
    if(e = swap_bytes((uint8_t *)buf1, buf1_size, NULL, buf2_size))
        printf("%s\n", e->str);
    if(e = swap_bytes((uint8_t *)buf1, buf1_size, (uint8_t *)buf2, buf2_size - 1))
        printf("%s\n", e->str);
    if(e = swap_bytes((uint8_t *)buf1, buf1_size, (uint8_t *)buf2, buf2_size))
        printf("should not print\n");
    printf("%d %d\n", buf1[3], buf2[3]);
    return 0;
}