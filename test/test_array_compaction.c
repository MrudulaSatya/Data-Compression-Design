#include "compression.h"
#include "unity.h"
#include <string.h>

#define MAX_SIZE_OF_DELETE_TABLE    (21845)
DT del_t[MAX_SIZE_OF_DELETE_TABLE];

void setUp(void) {
}

void tearDown(void) {
}

void compareArrays(uint8_t *data_ptr, uint8_t *expected_data_ptr, uint16_t total_deleted_items, uint16_t data_size, uint16_t expected_return_data_size)
{
    uint16_t new_arr_size = array_compaction(total_deleted_items, data_size, data_ptr);

    uint16_t len = data_size;
    if(expected_return_data_size < len)
    {
        len = expected_return_data_size;
    }

    char msg[64];
    for(int i = 0; i < len; i++)
    {
        snprintf(msg, sizeof(msg), "did not match at index i = %d", i);

        TEST_ASSERT_EQUAL_UINT8_MESSAGE(expected_data_ptr[i], data_ptr[i], msg);
    }

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(expected_return_data_size, new_arr_size, "compaction error");
}

void test_noDeletes(void) {
    uint8_t data_ptr[] = {0x01, 0x02, 0x03, 0x05};
    uint8_t expected_ptr[] = {0x01, 0x02, 0x03, 0x05};
    DT del_table[] = {{}};
    memcpy(del_t, del_table, 0);

    compareArrays(data_ptr, expected_ptr, 0, 4, 4);
}

void test_deleteAll(void) {
    uint8_t data_ptr[] = {0x01, 0x02, 0x03, 0x05};
    uint8_t expected_ptr[] = {};
    DT del_table[] = {{0,4}};
    memcpy(del_t, del_table, 1 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 1, 4, 0);
}

void test_singleDeleteInMiddle(void) {
    uint8_t data_ptr[] = {0x01, 0x02, 0x03, 0x05};
    uint8_t expected_ptr[] = {0x01, 0x02, 0x05};
    DT del_table[] = {{2, 1}};
    memcpy(del_t, del_table, 1 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 1, 4, 3);
}

void test_singleDeleteAtBeggining(void) {
    uint8_t data_ptr[] = {0x01, 0x02, 0x03, 0x05};
    uint8_t expected_ptr[] = {0x02, 0x03, 0x05};
    DT del_table[] = {{0,1}};
    memcpy(del_t, del_table, 1 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 1, 4, 3);
}

void test_singleDeleteAtEnd(void) {
    uint8_t data_ptr[] = {0x01, 0x02, 0x03, 0x05};
    uint8_t expected_ptr[] = {0x01, 0x02, 0x03};
    DT del_table[] = {{3,1}};
    memcpy(del_t, del_table, 1 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 1, 4, 3);
}

void test_multipleDeleteswithDeleteAtBeggining(void) {
    uint8_t data_ptr[] = {0x03, 0x74, 0x84, 0x03, 0x04, 0xb5, 0x02, 0xe4, 0x04, 0x64, 0x64, 0x80, 0x05, 0x00, 0x00, 0x00, 0x56, 0x45, 0xd6, 0x03, 0x56, 0x89, 0x03, 0x09 };
    uint8_t expected_ptr[] = {0x74, 0x84, 0x03, 0xb5, 0x02, 0xe4, 0x04, 0x80, 0x05, 0x56, 0x45, 0xd6, 0x03, 0x89, 0x03, 0x09};
    DT del_table[] = {{0,1},{4,1}, {9,2}, {13,3}, {20,1}};
    memcpy(del_t, del_table, 5 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 5, 24, 16);
}

void test_multipleDeleteswithDeleteInMiddle(void) {
    uint8_t data_ptr[] = {0x03, 0x74, 0x84, 0x03, 0x04, 0xb5, 0x02, 0xe4, 0x04, 0x64, 0x64, 0x80, 0x05, 0x00, 0x00, 0x00, 0x56, 0x45, 0xd6, 0x03, 0x56, 0x89, 0x03, 0x09 };
    uint8_t expected_ptr[] = {0x03, 0x74, 0x84, 0x03, 0xb5, 0x02, 0xe4, 0x04, 0x80, 0x05, 0x56, 0x45, 0xd6, 0x03, 0x89, 0x03, 0x09};
    DT del_table[] = {{4,1}, {9,2}, {13,3}, {20,1}};
    memcpy(del_t, del_table, 4 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 4, 24, 17);
}

void test_multipleDeleteswithDeleteAtEnd(void)
{
    uint8_t data_ptr[] = {0x03, 0x74, 0x84, 0x03, 0x04, 0xb5, 0x02, 0xe4, 0x04, 0x64, 0x64, 0x80, 0x05, 0x00, 0x00, 0x00, 0x56, 0x45, 0xd6, 0x03, 0x56, 0x89, 0x03, 0x09 };
    uint8_t expected_ptr[] = {0x03, 0x74, 0x84, 0x03, 0xb5, 0x02, 0xe4, 0x04, 0x80, 0x05, 0x56, 0x45, 0xd6, 0x03, 0x89, 0x03 };
    DT del_table[] = {{4,1}, {9,2}, {13,3}, {20,1}, {23,1}};
    memcpy(del_t, del_table, 5 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 5, 24, 16);
}

void test_multipleDeletesTogetherInMiddle(void) {
    uint8_t data_ptr[] = {0x03, 0x74, 0x84, 0x03, 0x04, 0xb5, 0x02, 0xe4, 0x04, 0x64, 0x64, 0x80, 0x05, 0x00, 0x00, 0x00, 0x56, 0x45, 0xd6, 0x03, 0x56, 0x89, 0x03, 0x09 };
    uint8_t expected_ptr[] = {0x03, 0x74, 0x84, 0xb5, 0x02, 0xe4, 0x04, 0x56, 0x45, 0xd6, 0x03, 0x89, 0x03, 0x09};
    DT del_table[] = {{3,1}, {4,1}, {9,2}, {11, 2}, {13,3}, {20,1}};
    memcpy(del_t, del_table, 6 * sizeof(DT));

    compareArrays(data_ptr, expected_ptr, 6, 24, 14);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_noDeletes);
    RUN_TEST(test_deleteAll);

    RUN_TEST(test_singleDeleteInMiddle);
    RUN_TEST(test_singleDeleteAtBeggining);
    RUN_TEST(test_singleDeleteAtEnd);

    RUN_TEST(test_multipleDeleteswithDeleteAtBeggining);
    RUN_TEST(test_multipleDeleteswithDeleteInMiddle);
    RUN_TEST(test_multipleDeleteswithDeleteAtEnd);

    RUN_TEST(test_multipleDeletesTogetherInMiddle);

    return UNITY_END();
}
