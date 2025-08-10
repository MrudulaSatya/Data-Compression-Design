#include <stdio.h>
#include <stdint.h>
#include <string.h>

/** Macro Definitions */
#define MAX_UINT8_VAL           (0xFF) // Maximum value of an unsigned 8-bit integer 
#define MAX_VAL_IN_BUFFER       (0x7F) // Maximum number that can be stored in the array of bytes
#define COMPRESS_ALGO_CONST     (0x80) // Constant that is added to a consecutively repeated byte to compress it and differentiate it from normal bytes

//#define USE_ASSERT //Uncomment this line to activate assert statements inside function definition

/**
 * @brief This is a structure that stores the indices to the elements that need to be deleted to compress the array
 */
typedef struct delete_table
{
    /**
     * @brief Starting index of a sequence that needs to be deleted
     * 
     *  If consecutive sequence of elements need to be deleted, this index indicates the start of that sequence
     */
    uint16_t start_index;

    /**
     * @brief Number of items to be deleted starting from start_index
     * 
     *  If consecutive sequence of elements need to be deleted, this specifies how many of them need to be removed
     */
    uint16_t len_to_be_deleted;
} DT;


/**
 * @brief Function compresses the array of bytes by identifying sequences of consecutively repeating bytes and 
 *  shrinking each such sequence to a sequence of size 2 -
 *  The first byte is - 0x80 + the repeating byte. This gives two types of information to the decompress algorithm. 
 *                                                 The value of the repeating byte and special flag that indicates this sequence is compressed. 
 *  The second byte is - number of times the byte repeated in that sequence
 * 
 * @param data_ptr - Array of bytes
 * @param data_size - Size of array
 * 
 * @return Returns the new size of the compressed array
 * 
 * This function updates the array and compresses it in place. 
 * 
 * Time complexity - O(n + d) - traverses the data_ptr twice and traverses the delete table once. Delete table has d number of sequences to delete
 * Space complexity - O(d) - d is the number of sequences that need to be deleted. No extra space used for compacting the array. 
 */
uint16_t byte_compress(uint8_t *data_ptr, uint16_t data_size);

/**
 * @brief Function that compacts the array by moving the elements within the array to indices occupied by repeated bytes which are marked for deletion
 *  The function knows what items to delete using an array of DT structure, that mentions the starting index and lengths of 
 *  all sequences to remove
 * 
 * @param  total_items_to_delete - Total number of sequences to delete
 * @param  data_size - size of the array
 * @param  data_ptr - array of bytes
 * 
 * @return Returns the new size of the compressed array
 * 
 */
uint16_t array_compaction(uint16_t total_items_to_delete, uint16_t data_size, uint8_t *data_ptr);

/**
 * @brief Function that decompresses the array by reversing the compress algorithm and expanding the array by moving the elements
 * 
 * @param  data_ptr - array of bytes
 * @param  decompressed_arr_size - size of the original decompressed array
 * @param  compressed_arr_size - size of the compressed array
 * 
 * @return  Returns the size of the decompressed array
 */
uint16_t decompress(uint8_t *data_ptr, uint16_t decompressed_arr_size, uint16_t compressed_arr_size);