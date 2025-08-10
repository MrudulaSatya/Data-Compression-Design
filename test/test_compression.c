#include "compression.h"
#include "unity.h"
#include <string.h>

void setUp(void) {
}

void tearDown(void) {
}

void printArray(uint8_t *data_ptr, uint16_t data_size)
{
    for(int i=0; i<data_size; i++)
    {
        printf("%x ", data_ptr[i]);
    }
    printf("\n");
}

void compareArray(uint8_t *data_ptr, uint8_t *expected_data_ptr, uint16_t data_size, uint16_t expected_return_data_size)
{
    uint16_t len = data_size;

    if(expected_return_data_size < len)
    {
        len = expected_return_data_size;
    }

    char msg[64] = {0};
    for(int i = 0; i < len; i++)
    {
        snprintf(msg, sizeof(msg), "did not match at i = %d", i);
        
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(expected_data_ptr[i], data_ptr[i], msg);
    }

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(data_size, expected_return_data_size, "returned size incorrect!");
}

void test_arrayOfSizeZero(void) {
    uint8_t data_ptr[] = {};
    uint16_t data_size = 0; 
    uint8_t expected_ptr[] = {};
    uint16_t expected_return_data_size = 0; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_arrayOfSizeOne(void) {
    uint8_t data_ptr[] = {0x03};
    uint16_t data_size = 1; 
    uint8_t expected_ptr[] = {0x03};
    uint16_t expected_return_data_size = 1; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_arrayOfSizeTwoWithSameElement(void) {
    uint8_t data_ptr[] = {0x03, 0x03};
    uint16_t data_size = 2; 
    uint8_t expected_ptr[] = {0x83, 0x02};
    uint16_t expected_return_data_size = 2; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_arrayOfSizeTwoWithDiffElement(void) {
    uint8_t data_ptr[] = {0x03, 0x05};
    uint16_t data_size = 2; 
    uint8_t expected_ptr[] = {0x03, 0x05};
    uint16_t expected_return_data_size = 2; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_allDifferentValues(void) {
    uint8_t data_ptr[] = {0x03, 0x05, 0x78, 0x79, 0x45, 0x09};
    uint16_t data_size = 6; 
    uint8_t expected_ptr[] = {0x03, 0x05, 0x78, 0x79, 0x45, 0x09};
    uint16_t expected_return_data_size = 6; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_allSameValues(void) {
    uint8_t data_ptr[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
    uint16_t data_size = 7; 
    uint8_t expected_ptr[] = {0x83, 0x07};
    uint16_t expected_return_data_size = 2; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);


    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameValuesInBeggining(void) {
    uint8_t data_ptr[] = {0x03, 0x03, 0x03, 0x45, 0x33, 0x78, 0x65};
    uint16_t data_size = 7; 
    uint8_t expected_ptr[] = {0x83, 0x03,0x45, 0x33, 0x78, 0x65};
    uint16_t expected_return_data_size = 6; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameValuesInMiddle(void) {
    uint8_t data_ptr[] = {0x03, 0x23, 0x45, 0x45, 0x45, 0x45, 0x65};
    uint16_t data_size = 7; 
    uint8_t expected_ptr[] = {0x03, 0x23, 0xC5, 0x04, 0x65};
    uint16_t expected_return_data_size = 5; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameValuesInEnd(void) {
    uint8_t data_ptr[] = {0x03, 0x23, 0x49, 0x45, 0x65, 0x65, 0x65};
    uint16_t data_size = 7; 
    uint8_t expected_ptr[] = {0x03, 0x23, 0x49, 0x45, 0xE5, 0x03};
    uint16_t expected_return_data_size = 6; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameInBeginningAndEnding(void) {
    uint8_t data_ptr[] = {0x74, 0x74, 0x74, 0x74, 0x74, 0x30, 0x35, 0x60, 0x64, 0x65, 0x78, 0x00, 0x01, 0x02, 0x03, 0x04, 0x56, 0x45, 0x56, 0x57, 0x58, 0x09, 0x09, 0x09};
    uint16_t data_size = 24; 
    uint8_t expected_ptr[] = {0xF4, 0x05, 0x30, 0x35, 0x60, 0x64, 0x65, 0x78, 0x00, 0x01, 0x02, 0x03, 0x04, 0x56, 0x45, 0x56, 0x57, 0x58, 0x89, 0x03};
    uint16_t expected_return_data_size = 20; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameInBeginningAndMiddle(void) {
    uint8_t data_ptr[] = {0x74, 0x74, 0x74, 0x74, 0x74, 0x30, 0x35, 0x60, 0x64, 0x65, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x45, 0x56, 0x57, 0x58, 0x09, 0x19, 0x59};
    uint16_t data_size = 24; 
    uint8_t expected_ptr[] = {0xF4, 0x05, 0x30, 0x35, 0x60, 0x64, 0x65, 0x81, 0x07, 0x45, 0x56, 0x57, 0x58, 0x09, 0x19, 0x59};
    uint16_t expected_return_data_size = 16; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t expected_decompressed_ptr[] = {0x74, 0x74, 0x74, 0x74, 0x74, 0x30, 0x35, 0x60, 0x64, 0x65, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x45, 0x56, 0x57, 0x58, 0x09, 0x19, 0x59};
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameInMiddleAndEnding(void) {
    uint8_t data_ptr[] = {0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09};
    uint16_t data_size = 24; 
    uint8_t expected_ptr[] = {0x03, 0x74, 0x84, 0x03, 0xb5, 0x02, 0xe4, 0x04, 0x80, 0x05, 0x56, 0x45, 0xd6, 0x03, 0x89, 0x03 };
    uint16_t expected_return_data_size = 16; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_sameInMiddleSeperated(void) {
    uint8_t data_ptr[] = {0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x79, 0x09};
    uint16_t data_size = 24; 
    uint8_t expected_ptr[] = {0x03, 0x74, 0x84, 0x03, 0xb5, 0x02, 0xe4, 0x04, 0x80, 0x05, 0x56, 0x45, 0xd6, 0x03, 0x09, 0x79, 0x09 };
    uint16_t expected_return_data_size = 17; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_maxSizeOfArray(void)
{
    uint8_t data_ptr[65535];
    uint16_t data_size = 65535; 
    uint16_t count = 0;
    for(int i=0; i<65535; i++)
    {
        data_ptr[i] = 0x23;
    }
    
    uint8_t expected_ptr[514];
    count = 0;
    for(int i=0; i<514; i=i+2)
    {
        expected_ptr[i] = 0xA3;
        expected_ptr[i+1] = 0xFF;
    }
   
    uint16_t expected_return_data_size = 514; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_eachCharacterIsZero(void)
{
    uint8_t data_ptr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint16_t data_size = 7; 
    uint8_t expected_ptr[] = {0x80, 0x07};
    uint16_t expected_return_data_size = 2; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}

void test_eachCharacterIs7F(void)
{
    uint8_t data_ptr[] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};
    uint16_t data_size = 7; 
    uint8_t expected_ptr[] = {0xFF, 0x07};
    uint16_t expected_return_data_size = 2; 

    uint16_t ret_size = byte_compress(data_ptr, data_size);

    compareArray(data_ptr, expected_ptr, ret_size, expected_return_data_size);

    ret_size = decompress(data_ptr, data_size, ret_size);

    printf("test decompression\n");

    uint8_t *expected_decompressed_ptr = data_ptr;
    compareArray(data_ptr, expected_decompressed_ptr, data_size, ret_size);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_arrayOfSizeZero);
    RUN_TEST(test_arrayOfSizeOne);
    RUN_TEST(test_arrayOfSizeTwoWithSameElement);
    RUN_TEST(test_arrayOfSizeTwoWithSameElement);
    RUN_TEST(test_arrayOfSizeTwoWithDiffElement);

    RUN_TEST(test_allDifferentValues);
    RUN_TEST(test_allSameValues);

    RUN_TEST(test_sameValuesInBeggining);
    RUN_TEST(test_sameValuesInMiddle);
    RUN_TEST(test_sameValuesInEnd);

    RUN_TEST(test_sameInBeginningAndEnding);
    RUN_TEST(test_sameInBeginningAndMiddle);
    RUN_TEST(test_sameInMiddleAndEnding);

    RUN_TEST(test_maxSizeOfArray);
    RUN_TEST(test_eachCharacterIsZero);
    RUN_TEST(test_eachCharacterIs7F);

    return UNITY_END();
}