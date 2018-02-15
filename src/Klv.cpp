#include "Klv.h"

#include <iostream>
#include <stdexcept>

namespace klv
{


// BER encodes the given length, and adds it to the buffer
// Returns the number of bytes the BER encoded length consisted of
int encode_and_add_len(sbuf_t* buf, int length)
{
    uint8_t bytes[16];
	int ber_len = 0;

    int buflen = buf->length;
    if(length < 128)
	{
		bytes[0] = (uint8_t)length;
		if(sbuf_add_data(buf, bytes, 1) != buflen + 1)
        	throw std::runtime_error("Buffer overflow");

    	return 1;
	} else if(length < 256)
	{
        ber_len = 2;
	} else if(length < 256*256)
    {
        ber_len = 3;
    } else if(length < 256 * 256 * 256)
    {
        ber_len = 4;
    } else
    {
        // Too long number...
        throw std::runtime_error("Number was too large for BEr encoder (>0xFFFFFF)");

    }
        
    // Add the BER byte length
    bytes[0] = 127 + ber_len;
    for(int i = 1; i < ber_len; i++)
        bytes[i] = (length >> (8 * (ber_len - i - 1))) & 0xFF;


    if(sbuf_add_data(buf, bytes, ber_len) != buflen + ber_len)
        throw std::runtime_error("Buffer overflow");

    return ber_len;
}




// Decodes a byte sequence with a BER encoded length, and returns it
// BER_len[out] is the length of eth byte sequency comprising the BER
// Decoder from :
// https://github.com/Kitware/burn-out/blob/master/library/klv/klv_parse.cxx
int decode_BER(uint8_t* bytes, int* BER_len)
{
    // Handle BER short form:
    if(!(bytes[0] & 0x80))
    {
        *BER_len = 1;
        return (int)bytes[0];
    }

    uint8_t len_len = ( 0x7F & bytes[0] ) + 1;
    if ( len_len >= 5 )
    {
        throw std::runtime_error("BER length was > 5, not implemented");
    }

    *BER_len = len_len;

    int value_len = 0;
    for ( uint8_t i = 1; i < len_len; ++i )
    {
        value_len <<= 8;
        value_len += *(bytes + i);
    }


    return value_len;

}

int Add(sbuf_t* buf, uint8_t key, const std::string& str)
{


    int b1, b2, b3;
    int l0 = buf->length;
    // key
    b1 = sbuf_add_data(buf, &key, sizeof(key));

    // BEr encoded length
    b2 = encode_and_add_len(buf, str.length());

    // add the payload
    b3 = sbuf_add_data(buf, str.c_str(), str.length());

    
    // extensive OBC checking
    if( (b3 == 0) || (b2 == 0) || (b1 == 0) || b3!=buf->length)
        throw std::runtime_error("Buffer overflow");


    return buf->length - l0;
}

int Add(sbuf_t* buf, uint8_t key, const uint8_t& index, const std::string& str)
{

    int b1, b2, b3, b4;
    int l0 = buf->length;
    // key
    b1 = sbuf_add_data(buf, &key, sizeof(key));

    // BEr encoded length
    b2 = encode_and_add_len(buf, sizeof(index) + str.length());

    // add the payload
    b3 = sbuf_add_data(buf, &index, sizeof(index));
    b4 = sbuf_add_data(buf, str.c_str(), str.length());

    
    // extensive OBC checking
    if( (b4 == 0) || (b3 == 0) || (b2 == 0) || (b1 == 0) || b4!=buf->length)
        throw std::runtime_error("Buffer overflow");


    return buf->length - l0;
}

int Add(sbuf_t* buf, uint8_t key, const uint8_t& ind1, const uint8_t& ind2, const std::string& str)
{

    int b1, b2, b3, b4, b5;
    int l0 = buf->length;
    // key
    b1 = sbuf_add_data(buf, &key, sizeof(key));

    // BEr encoded length
    b2 = encode_and_add_len(buf, 2*sizeof(ind1) + str.length());

    // add the payload
    b3 = sbuf_add_data(buf, &ind1, sizeof(ind1));
    b4 = sbuf_add_data(buf, &ind2, sizeof(ind2));
    b5 = sbuf_add_data(buf, str.c_str(), str.length());

    
    // extensive OBC checking
    if( (b5 == 0) || (b4 == 0) || (b3 == 0) || (b2 == 0) || (b1 == 0) || b5!=buf->length)
        throw std::runtime_error("Buffer overflow");


    return buf->length - l0;
}















}
