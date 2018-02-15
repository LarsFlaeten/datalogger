#include "../src/Klv.cpp"
#include <gtest/gtest.h>

#include <memory>
#include <cstdio>
#include <vector>

using namespace std;

// The fixture for testing buffer.c.
class KlvTest : public ::testing::Test {

protected:

    // set-up work for each test here.
    KlvTest();

    // clean-up work that doesn't throw exceptions here.
    virtual ~KlvTest();

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();


    vector<uint8_t> test_pkt;
    vector<uint8_t> key;
    vector<uint8_t> len;
    vector<uint8_t> val;
    
};


KlvTest::KlvTest()
{
    test_pkt = { 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x0B, 0x01, 0x01, 0x0E, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x81, 0x90, 0x02, 0x08, 0x00, 0x04, 0x6C, 0xAE, 0x70, 0xF9, 0x80, 0xCF, 0x41, 0x01, 0x01, 0x05, 0x02, 0xE1, 0x91, 0x06, 0x02, 0x06, 0x0D, 0x07, 0x02, 0x0A, 0xE1, 0x0B, 0x02, 0x49, 0x52, 0x0C, 0x0E, 0x47, 0x65, 0x6F, 0x64, 0x65, 0x74, 0x69, 0x63, 0x20, 0x57, 0x47, 0x53, 0x38, 0x34, 0x0D, 0x04, 0x4D, 0xCC, 0x41, 0x90, 0x0E, 0x04, 0xB1, 0xD0, 0x3D, 0x96, 0x0F, 0x02, 0x1B, 0x2E, 0x10, 0x02, 0x00, 0x84, 0x11, 0x02, 0x00, 0x4A, 0x12, 0x04, 0xE7, 0x23, 0x0B, 0x61, 0x13, 0x04, 0xFD, 0xE8, 0x63, 0x8E, 0x14, 0x04, 0x03, 0x0B, 0xC7, 0x1C, 0x15, 0x04, 0x00, 0x9F, 0xB9, 0x38, 0x16, 0x04, 0x00, 0x00, 0x01, 0xF8, 0x17, 0x04, 0x4D, 0xEC, 0xDA, 0xF4, 0x18, 0x04, 0xB1, 0xBC, 0x81, 0x74, 0x19, 0x02, 0x0B, 0x8A, 0x28, 0x04, 0x4D, 0xEC, 0xDA, 0xF4, 0x29, 0x04, 0xB1, 0xBC, 0x81, 0x74, 0x2A, 0x02, 0x0B, 0x8A, 0x38, 0x01, 0x31, 0x39, 0x04, 0x00, 0x9F, 0x85, 0x4D, 0x01, 0x02, 0xB7, 0xEB };

    key = { 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x0B, 0x01, 0x01, 0x0E, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00 };
    len = { 0x81, 0x90 };
    val = { 0x02, 0x08, 0x00, 0x04, 0x6C, 0xAE, 0x70, 0xF9, 0x80, 0xCF, 0x41, 0x01, 0x01, 0x05, 0x02, 0xE1, 0x91, 0x06, 0x02, 0x06, 0x0D, 0x07, 0x02, 0x0A, 0xE1, 0x0B, 0x02, 0x49, 0x52, 0x0C, 0x0E, 0x47, 0x65, 0x6F, 0x64, 0x65, 0x74, 0x69, 0x63, 0x20, 0x57, 0x47, 0x53, 0x38, 0x34, 0x0D, 0x04, 0x4D, 0xCC, 0x41, 0x90, 0x0E, 0x04, 0xB1, 0xD0, 0x3D, 0x96, 0x0F, 0x02, 0x1B, 0x2E, 0x10, 0x02, 0x00, 0x84, 0x11, 0x02, 0x00, 0x4A, 0x12, 0x04, 0xE7, 0x23, 0x0B, 0x61, 0x13, 0x04, 0xFD, 0xE8, 0x63, 0x8E, 0x14, 0x04, 0x03, 0x0B, 0xC7, 0x1C, 0x15, 0x04, 0x00, 0x9F, 0xB9, 0x38, 0x16, 0x04, 0x00, 0x00, 0x01, 0xF8, 0x17, 0x04, 0x4D, 0xEC, 0xDA, 0xF4, 0x18, 0x04, 0xB1, 0xBC, 0x81, 0x74, 0x19, 0x02, 0x0B, 0x8A, 0x28, 0x04, 0x4D, 0xEC, 0xDA, 0xF4, 0x29, 0x04, 0xB1, 0xBC, 0x81, 0x74, 0x2A, 0x02, 0x0B, 0x8A, 0x38, 0x01, 0x31, 0x39, 0x04, 0x00, 0x9F, 0x85, 0x4D, 0x01, 0x02, 0xB7, 0xEB };



}

KlvTest::~KlvTest()
{

}
void KlvTest::SetUp()
{

}

void KlvTest::TearDown()
{

}



TEST_F(KlvTest, TestEncodeDecodeBERShortForm) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 32);

    int len;
    

    for(len = 1; len < 0x80; ++len)
    {
        int ber_len = klv::encode_and_add_len(&buf, len);
        ASSERT_EQ(ber_len, 1);
       
        int ber_len2 = 0; 
        int decodedBER = klv::decode_BER(buf.data, &ber_len2);
        ASSERT_EQ(ber_len, ber_len2);
        ASSERT_EQ(decodedBER, len);
        
        sbuf_reset(&buf);
    }
}

TEST_F(KlvTest, TestEncodeBERLongForm) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 32);

    int len;
    

    for(len = 0x80; len < 256*256*256-1; ++len)
    {
        int berlen = klv::encode_and_add_len(&buf, len);
        
        if(len<256)
            ASSERT_EQ(berlen, 2);
        else if(len<256*256)
            ASSERT_EQ(berlen, 3);
        else if(len<256*256*256)        
            ASSERT_EQ(berlen, 4);

        int berlen2 = 0;
        int decodedBER = klv::decode_BER(buf.data, &berlen2);
        ASSERT_EQ(berlen, berlen2);
        ASSERT_EQ(decodedBER, len);
        
        sbuf_reset(&buf);
    }
}

TEST_F(KlvTest, TestEncodeBERTooLong) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 32);

    int len = 256*256*256;
    
    ASSERT_THROW(klv::encode_and_add_len(&buf, len), std::runtime_error);
}

TEST_F(KlvTest, TestDecodeBERLongForm) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 32);

    int len;
    

    for(len = 0x80; len < 256*256*256-1; ++len)
    {
        int berlen = klv::encode_and_add_len(&buf, len);
        
        if(len<256)
            ASSERT_EQ(berlen, 2);
        else if(len<256*256)
            ASSERT_EQ(berlen, 3);
        else if(len<256*256*256)        
            ASSERT_EQ(berlen, 4);

        int berlen2 = 0;
        int decodedBER = klv::decode_BER(buf.data, &berlen2);
        ASSERT_EQ(berlen, berlen2);
        ASSERT_EQ(decodedBER, len);
        
        sbuf_reset(&buf);
    }
}

TEST_F(KlvTest, TestDecodeBERTooLong) {

    uint8_t b_too_long = 0x80 | 5;
    int ber_len = 0;
    ASSERT_THROW(klv::decode_BER(&b_too_long, &ber_len), std::runtime_error);
}

TEST_F(KlvTest, TestAddKlv) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 128);
    ASSERT_NE(buf.data, nullptr);
    uint8_t key = 0x10;
    
    // Test int    
    int a = 7;
    int a2;
    int badded = klv::Add<int>(&buf, key, a);
    int ber_len = 0;
    ASSERT_EQ(key, buf.data[0]);
    ASSERT_EQ(klv::decode_BER(&(buf.data[1]), &ber_len), sizeof(int)); 
    ASSERT_EQ(badded, 1+1+sizeof(int));
    memcpy((uint8_t*)&a2, (const void*)&(buf.data[2]), sizeof(int));
    ASSERT_EQ(a, a2);
    sbuf_reset(&buf);

    // test double
    double pi_ = 3.14159;
    double pi2_;
    badded = klv::Add<double>(&buf, key, pi_);
    ASSERT_EQ(key, buf.data[0]);
    ASSERT_EQ(klv::decode_BER(&(buf.data[1]), &ber_len), sizeof(double)); 
    ASSERT_EQ(badded, 1+1+sizeof(double));
    memcpy((uint8_t*)&pi2_, (const void*)&(buf.data[2]), sizeof(double));
    ASSERT_EQ(pi_, pi2_);
    sbuf_reset(&buf);

   


}

TEST_F(KlvTest, TestAdd2Klv) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 128);
    ASSERT_NE(buf.data, nullptr);
    uint8_t key = 0x10;
    
    uint8_t index = 43;
	double  d = 23.438748327;
 
    int badded = klv::Add<uint8_t>(&buf, key, index, d);
    ASSERT_EQ(key, buf.data[0]);
	int size = sizeof(uint8_t) + sizeof(double);
    int ber_len = 0;
    ASSERT_EQ(klv::decode_BER(&(buf.data[1]), &ber_len), size); 
    
    ASSERT_EQ(badded, 1+1+size);
}

TEST_F(KlvTest, TestAddStrFuncs) {

    sbuf_t      buf;
    sbuf_create_stack(&buf, 128);
    ASSERT_NE(buf.data, nullptr);
    uint8_t key = 0x10;
    uint8_t ind1 = 43;
    uint8_t ind2 = 239;

    std::string test_str("This is a test std::string!");
    char str[test_str.length()+1];
 
    int badded = klv::Add(&buf, key, test_str);
    ASSERT_EQ(key, buf.data[0]);
	int size = test_str.length();
    int ber_len = 0;
    ASSERT_EQ(klv::decode_BER(&(buf.data[1]), &ber_len), size); 
    ASSERT_EQ(badded, 1+1+size);
    memcpy(str, (const void*)&(buf.data[2]), size); str[size] = 0;
    ASSERT_EQ(0, test_str.compare(str));
    sbuf_reset(&buf);

    badded = klv::Add(&buf, key, ind1, test_str);
    ASSERT_EQ(key, buf.data[0]);
    ASSERT_EQ(klv::decode_BER(&(buf.data[1]), &ber_len), size+1);
    ASSERT_EQ(buf.data[2], ind1); 
    ASSERT_EQ(badded, 1+1+1+size);
    memcpy(str, (const void*)&(buf.data[3]), size); str[size] = 0;
    ASSERT_EQ(0, test_str.compare(str));
    sbuf_reset(&buf);

    badded = klv::Add(&buf, key, ind1, ind2, test_str);
    ASSERT_EQ(key, buf.data[0]);
    ASSERT_EQ(klv::decode_BER(&(buf.data[1]), &ber_len), size+2); 
    ASSERT_EQ(badded, 1+1+1+1+size);
    memcpy(str, (const void*)&(buf.data[4]), size); str[size] = 0;
    ASSERT_EQ(0, test_str.compare(str));
    sbuf_reset(&buf);

}





