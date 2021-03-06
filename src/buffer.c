#include "buffer.h"


#include <stdlib.h>
#include <alloca.h>
#include <string.h>


void    sbuf_create(sbuf_t* sbuf, int max_length)
{
    sbuf->data = NULL;
    sbuf->max_length = max_length;
    sbuf->data = (uint8_t*)malloc(max_length);
    if(sbuf->data == NULL)
        sbuf->max_length = 0;
    sbuf->length = 0;    
}

void    sbuf_reset(sbuf_t* sbuf)
{
    sbuf->length = 0;    
}

void    sbuf_release(sbuf_t* sbuf)
{
    sbuf->max_length = 0;
    sbuf->length = 0;
    free(sbuf->data);
    sbuf->data = NULL;
}






int sbuf_add_data(sbuf_t* sbuf, const void* data, int length)
{
    if(length == 0)
        return 0;

    // Check for overflow:
    if(sbuf->length + length > sbuf->max_length)
    {
        return 0;
    }

    memcpy(sbuf->data + sbuf->length, data, length);

    sbuf->length += length;

    return sbuf->length;
}
