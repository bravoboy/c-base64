#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM 64
 
static char base64_chars[MAX_NUM] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

char *  base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len,unsigned int *out_len) {
    char *ret = (char*)malloc((int)(in_len*4.0/3+10));
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    int begin = 0;
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4); 
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6); 
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret[begin++] = base64_chars[char_array_4[i]];
            i = 0;
        }   
    }   

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4); 
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6); 
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret[begin++] = base64_chars[char_array_4[j]];

        while((i++ < 3)) 
            ret[begin++] = '=';

    }   
    ret[begin] = '\0';
    *out_len = begin;
    return ret;
}
static inline int is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}
static inline int getindex(unsigned char c) {
    if (isupper(c)) {
        return c - 'A';
    } else if (islower(c)) {
        return c - 'a' + 26;
    } else if (isdigit(c)){
        return c - '0' + 52;
    } else {
        if (c == '+') {
            return 62;
        } else if (c == '/') {
            return 63;
        } else {
            return -1;
        }
    }
}
char * base64_decode(char * encoded_string,unsigned int *out_len) {
    int in_len = strlen(encoded_string);
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    char* ret = (char*)malloc(in_len);
    int begin = 0;
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = getindex(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4); 
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2); 
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret[begin++] = char_array_3[i];
            i = 0;
        } 
    } 

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < i; j++)
            char_array_4[j] = getindex(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4); 
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2); 
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret[begin++] = char_array_3[j];
    }
    ret[begin] = '\0';
    *out_len = begin;
    return ret;
}
