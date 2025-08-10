#include "compression.h"

void printArray(uint8_t *data_ptr, uint16_t data_size)
{
    for(int i=0; i<data_size; i++)
    {
        printf("%x ", data_ptr[i]);
    }
    printf("\n");
}

int main()
{
    uint8_t data_ptr[] = {0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09};
    uint16_t data_size = 24; 

    printArray(data_ptr, data_size);

    uint16_t retSize = byte_compress(data_ptr, data_size);

    printf("%d\n", retSize);

    printArray(data_ptr, retSize);

    uint16_t decompSize = decompress(data_ptr, data_size, retSize);

    printArray(data_ptr, decompSize);

    return 0;
}