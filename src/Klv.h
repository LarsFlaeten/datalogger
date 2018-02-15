#ifndef _KLH_V_
#define _KLH_V_

#include <stdexcept>
#include <iostream>
#include <string.h>


#include "buffer.h"


namespace klv
{

// BER encodes the given length, and adds it to the buffer
// Returns the number of bytes the BER encoded length consisted of
int encode_and_add_len(sbuf_t* buf, int length);

// Decodes a byte sequence with a BER encoded length, and returns it
// size of bytes must be at least 5
// BER_len[out] os the byte length of the encoded BER
int decode_BER(uint8_t* bytes, int* BER_len);

// Reads the BER length from the first byte of a BER sequence
// If it is short form, returns 0 (the byte is the length <127) and can be used directly
int get_BER_len(uint8_t byte);
// If the above func returns >0, use this one to decode the BER long form
int get_BER_long_len(uint8_t* bytes, int ber_length);


// General KLV add function
// returns number of bytes added to the buffer
template<class T>
int Add(sbuf_t* buf, uint8_t key, const T& value)
{
    int b1, b2, b3;
    int l0 = buf->length;
    // key
    b1 = sbuf_add_data(buf, &key, sizeof(key));

    // BEr encoded length
    b2 = encode_and_add_len(buf, sizeof(value));

    // add the payload
    b3 = sbuf_add_data(buf, (uint8_t*)&value, sizeof(value));


    // extensive OBC checking
    if( (b3 == 0) || (b2 == 0) || (b1 == 0) || b3!=buf->length)
        throw std::runtime_error("Buffer overflow");


    return buf->length - l0;
}

template<class T, class S>
int Add(sbuf_t* buf, uint8_t key, const T& val1, const S& val2)
{
    uint8_t values[sizeof(T) + sizeof(S)];

    memcpy(values, (uint8_t*)&val1, sizeof(T));
    memcpy(values + sizeof(T), (uint8_t*)&val2, sizeof(S));

    int b1, b2, b3;
    int l0 = buf->length;
    // key
    b1 = sbuf_add_data(buf, &key, sizeof(key));

    // BEr encoded length
    b2 = encode_and_add_len(buf, sizeof(T) + sizeof(S));

    // add the payload
    b3 = sbuf_add_data(buf, values, sizeof(T) + sizeof(S));

    
    // extensive OBC checking
    if( (b3 == 0) || (b2 == 0) || (b1 == 0) || b3!=buf->length)
        throw std::runtime_error("Buffer overflow");


    return buf->length - l0;
}

// add double-indexed general value
template<class T>
int AddIndexed(sbuf_t* buf, uint8_t key, uint8_t ind1, uint8_t ind2, const T& value)
{
    int b1, b2, b3, b4, b5;
    int l0 = buf->length;
    // key
    b1 = sbuf_add_data(buf, &key, sizeof(key));

    // BEr encoded length
    b2 = encode_and_add_len(buf, sizeof(value)+2);

    // add the payload
    b3 = sbuf_add_data(buf, &ind1, 1);
    b4 = sbuf_add_data(buf, &ind2, 1);
    b5 = sbuf_add_data(buf, (uint8_t*)&value, sizeof(value));


    // extensive OBC checking
    if( (b5 == 0) || (b4 == 0) || (b3 == 0) || (b2 == 0) || (b1 == 0) || b5!=buf->length)
        throw std::runtime_error("Buffer overflow");


    return buf->length - l0;
}


// For adding a string:
int Add(sbuf_t* buf, uint8_t key, const std::string& str);
// For adding a string with an index
int Add(sbuf_t* buf, uint8_t key, const uint8_t& index, const std::string& str);
// For adding a string with a double index
int Add(sbuf_t* buf, uint8_t key, const uint8_t& ind1, const uint8_t& ind2, const std::string& str);



}
#endif
