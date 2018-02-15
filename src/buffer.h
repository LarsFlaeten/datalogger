#ifndef _BUFFER_H_
#define _BUFFER_H_


// A simple buffer used for streaming data (over network, to log files etc)
// We implement this one in plain c..
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct sbuf {
    uint8_t*    data;
    int         length;
    int         max_length;
} sbuf_t;


// Init, two functions
// sbuf_create is normal malloc, and should be freed with sbuf_release
// sbuf_create_stack allocates memory on the stack, and will be automatically freed 
void sbuf_create(sbuf_t* sbuf, int maxLength);

// had to define this as a macro, as a function for this would immediately free
// the stack memory (naturally...)
#define sbuf_create_stack(p, m) do {\
    (p)->max_length = (m);\
    (p)->length = 0;\
    (p)->data = (uint8_t*)alloca(m);\
} while(0)




// Clear data and set length to 0, but can reuse buffer
void sbuf_reset(sbuf_t* sbuf);

// Release
void sbuf_release(sbuf_t* sbuf);

// add data to buffer
// TODO: Should this be static?
// TODO: Maybe inline?
// Returns the new length after bytes added, or 0 if the buffer overflowed
int sbuf_add_data(sbuf_t* sbuf, const void* data, int length);






#ifdef __cplusplus
}
#endif




#endif
