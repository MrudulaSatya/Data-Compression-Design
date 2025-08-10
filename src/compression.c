#include "compression.h"
#include <assert.h>

DT del_t[MAX_SIZE_OF_DELETE_TABLE] = {0};

/**
 * @brief Private function that changes the bytes in the array by implementing the compress logic. It also updates the delete table
 *  by adding the starting indices and lengths of the seqeunces that should be marked for deletion. 
 * 
 * @param  count_of_repeated_chars - It decides how many elements to mark for deletion based on how many bytes have been repeated
 * @param  data_index - Current index within data_ptr which the byte_compress() function is looking at
 * @param  prev_element - Byte that indicates the value of the repetitive byte
 * @param  data_ptr - Array of bytes
 * @param  ind_of_del_items - Pointer to the variable that holds the current postion in the delete table where new entry can be made
 */
void update_array_and_mark_items_for_deletion(uint16_t count_of_repeated_chars, uint16_t data_index, uint8_t prev_element, uint8_t *data_ptr, uint16_t *ind_of_del_items);

uint16_t byte_compress(uint8_t *data_ptr, uint16_t data_size)
{
#ifdef USE_ASSERT
    assert(data_ptr != NULL);
    assert(data_size >= 0);
    assert(del_t != NULL);
#endif

    uint16_t data_index = 1, count_of_repeated_chars = 1, ind_of_del_items = 0;
    uint8_t prev_element = data_ptr[0], curr_element = data_ptr[data_index];

    while(data_index < data_size)
    {
        curr_element = data_ptr[data_index];
        if((prev_element == curr_element) && (count_of_repeated_chars < MAX_UINT8_VAL))
        {
            count_of_repeated_chars++;
        }
        else
        {
            update_array_and_mark_items_for_deletion(count_of_repeated_chars, data_index, prev_element, data_ptr, &ind_of_del_items);
            count_of_repeated_chars = 1;
        }
        prev_element = data_ptr[data_index];

        data_index++;
    }

    // if there are repeated characters in the end 
    update_array_and_mark_items_for_deletion(count_of_repeated_chars, data_index, prev_element, data_ptr, &ind_of_del_items);

    return array_compaction(ind_of_del_items, data_size, data_ptr);
}

uint16_t decompress(uint8_t *data_ptr, uint16_t decompressed_arr_size, uint16_t compressed_arr_size)
{
#ifdef USE_ASSERT
    assert(data_ptr != NULL);
    assert(decompressed_arr_size >= 0);
    assert(compressed_arr_size >= 0);
    assert(compressed_arr_size <= decompressed_arr_size);
#endif

    uint16_t data_index = 0;
    uint16_t last_ind_of_array = compressed_arr_size - 1;

    while(data_index < decompressed_arr_size)
    {
        // if a value is greater than 0x7F, then it means it has been manipulated for compression  
        // the following byte definitely exists which holds the number of repetetions
        if(data_ptr[data_index] > MAX_VAL_IN_BUFFER) 
        {
            uint8_t byte_to_replace_with = data_ptr[data_index] - COMPRESS_ALGO_CONST;
            uint16_t no_of_reps = data_ptr[data_index + 1];
            uint16_t len_to_push_forward = no_of_reps - 2;

            if(len_to_push_forward > 0)
            {
                if((data_index + 2 + len_to_push_forward) < decompressed_arr_size)
                {
                    memmove(&data_ptr[data_index + 2 + len_to_push_forward], &data_ptr[data_index + 2], last_ind_of_array - ((data_index + 2) - 1));
                }
                last_ind_of_array = last_ind_of_array + len_to_push_forward;
            }

            for(int i = 0; i < no_of_reps; i++)
            {
                data_ptr[data_index + i] = byte_to_replace_with;
            }

            data_index = data_index + 2 + len_to_push_forward;
        }
        else
        {
            data_index++;
        }
    }

    return data_index;
}

void update_array_and_mark_items_for_deletion(uint16_t count_of_repeated_chars, uint16_t data_index, uint8_t prev_element, uint8_t *data_ptr, uint16_t *ind_of_del_items)
{
#ifdef USE_ASSERT
    assert(data_ptr != NULL);
    assert(data_index >= 0);
    assert(count_of_repeated_chars <= data_index);
#endif

    if(count_of_repeated_chars >= 2)
    {
        if(count_of_repeated_chars > 2)
        {
            del_t[*ind_of_del_items].start_index = (data_index - count_of_repeated_chars) + 2;
            del_t[*ind_of_del_items].len_to_be_deleted  = count_of_repeated_chars - 2;
            *ind_of_del_items = *ind_of_del_items + 1;
        }

        data_ptr[data_index - count_of_repeated_chars] = COMPRESS_ALGO_CONST + prev_element;
        data_ptr[(data_index - count_of_repeated_chars) + 1] = count_of_repeated_chars;
    }
}

uint16_t array_compaction(uint16_t total_items_to_delete, uint16_t data_size, uint8_t *data_ptr)
{
#ifdef USE_ASSERT
    assert(data_ptr != NULL);
    assert(data_size >= 0);
    assert(total_items_to_delete >= 0);
#endif

    // If no item needs to be deleted, then data_ptr does not change hence, the new size of array is same as current size of data_ptr
    if(total_items_to_delete == 0)
        return data_size; 

    uint16_t start_index_of_section_to_keep = 0, index_to_start_copying_at = 0, len_to_copy = 0, start_ind_of_nxt_section_to_keep = 0;

    for(uint16_t i = 0 ; i < total_items_to_delete; i++)
    {
        start_ind_of_nxt_section_to_keep = del_t[i].start_index + del_t[i].len_to_be_deleted;
        len_to_copy = del_t[i].start_index - start_index_of_section_to_keep; 

        if(len_to_copy > 0)
        {
            memmove(&data_ptr[index_to_start_copying_at], &data_ptr[start_index_of_section_to_keep], len_to_copy);
        }

        index_to_start_copying_at = index_to_start_copying_at + len_to_copy;
        start_index_of_section_to_keep = start_ind_of_nxt_section_to_keep;
    }

    // If the next chunk to copy is in between data_ptr, it means we need to copy all bytes starting from 
    // start_ind_of_nxt_section_to_keep index till the end of data_ptr
    if(start_ind_of_nxt_section_to_keep < data_size)
    {
        len_to_copy = data_size - start_index_of_section_to_keep;
        if(len_to_copy > 0)
        {
            memmove(&data_ptr[index_to_start_copying_at], &data_ptr[start_index_of_section_to_keep], len_to_copy);
        }
        index_to_start_copying_at = index_to_start_copying_at + len_to_copy;
    }

    return index_to_start_copying_at;
}


