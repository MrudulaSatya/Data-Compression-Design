# Code Design Test: Data Compression Design

This project implements an algorithm in C to compress a given data buffer of bytes. It includes a decompression algorithm to restore the buffer to its original form. 

## Pre-requisites

1. Tested on macOS 
2. gcc compiler

## How to build

1. Clone project to local directory. Open Terminal and change directory to project folder 
2. Run the following make commands 
```
   make clean
   make all
```

## Design Description

### Key Design Points
1. Problem statment mentions that it is common for the data in the buffer to have the same value repeated in the series. Each byte in the data buffer will contain a number from 0x00 to 0x7F. 
2. The design focuses on removing most of the repeated bytes. Numbers larger than 0x7F can be used as a differentiator between normal bytes (which are <= 0x7F) and bytes that are modified for compression (which are > 0x7F)
3. Three pieces of information are critical - the value of the repeated byte, the number of times the value repeated and a differentiator that indicates these bytes need to be processed to bring the original buffer back. 
4. Traverse the data in the buffer starting from index 0.
5. Find the sequences where a number is repeating more than once. 
6. Once such a sequence is found, add 0x80 (number > 0x7F) to repeating number. Place this modified value at the index where this sequence of repeated bytes started. As the resulting value will be greater than 0x7F, the decompress algorithm will know, this isnt a normal data byte. On subtracting 0x80 from the value at this index, it will be able to know the value of the repeated byte. 
7. Place the number of times the byte repeated at the index next to where (0x80 + repeated number) is placed. 
8. The remaining repeated bytes in the sequence can be removed which will compress the buffer and decrease the size of resulting buffer. 
9. If a byte is repeated more than 0xFF times, stop counting at 0xFF. Perform Step 5 till 7. Restart counting for the remaining repeated numbers. This is because, the data buffer can only hold 8-bit values. The maximum value of 8-bit unsigned integer is 0xFF. Hence, split the sequence into smaller chunks each of size <= 0xFF. 

### Fill Delete Table

1. `delete_table` struct array used to compress buffer. Each element of this delete table will have two values. First one is the starting index of the sequence that needs to be deleted. Second one is the number of bytes that need to be deleted. 
2. Add the starting index of the remaining repeated bytes and the number of remaining repeated bytes to the delete table. 


Traverse the entire data buffer and fill up the delete table with all the sequences that are repetitive and can be removed. 

### Compression steps
1. Shift the bytes that need to be kept into indices that need to be deleted to compress the array in place. 
2. Traverse through the delete table. Get the first sequence of elements that need to be kept. Copy this sequence to the appropriate indices in the same data buffer
3. Update the index where the next section that needs to be kept can be copied. This index will be where copying of previous section ended. 
4. Get the next sequence that need to be kept and copy this section at the index updated in Step 3. This will in effect shift the elements to the left overwriting the indices that need to be removed. 
5. Repeat Steps 3 and 4 until all the indices marked for deletion are overwritten or not considered in the final size. 
6. Save the new size of this array. 

Psuedo-code for byte_compress algorithm
```
    byte_compress(data_ptr, data_size):
        traverse data_ptr from index 1 till data_size:
            if((curr_element == data_ptr[data_index]) and (count_of_repeated_chars < 0xFF)):
                count_of_repeated_chars++
            else:
                if(count_of_repeated_chars >= 2):
                    data_ptr[data_index - count_of_repeated_chars] = 0x80 + prev_element;
                    data_ptr[(data_index - count_of_repeated_chars) + 1] = count_of_repeated_chars;
                    Store index [(data_index - count_of_repeated_chars) + 2] and  [count_of_repeated_chars - 2] 
                    as starting index and length to be deleted in delete table

        new_size = compact_array(data_ptr)
        return new_size
```
Psuedo-code for array_compaction algorithm
```
    compact_array(data_ptr):
        Traverse delete_table:
            Copy 0 to delete_table[i].start_index to data_ptr
            Save the index at which the above copy ended. This index is called data_index. 
            Get the next section to keep. next section to keep starts at index delete_table[i].start_index + delete_table[i].len_to_delete
            Copy the above section at data_index overwriting the locations that need to be deleted marked by the delete_table. 
            Repeat the process until entire delete_table is traversed. 
        return data_index as it is the new size of the compressed array. 
```

## How I reached solution

1. I had pre-requisite knowledge that zipping of files/folders is done by removing the sequences of consecutive repeated bytes. The problem statement too mentioned that the data buffer will commonly contain such sequences. I decided to focus on that. 
2. I leveraged the fact that the data in the buffer contains numbers only from 0x00 to 0x7F. All the bytes starting from 0x80 were reserved. So I could use them in the compressing algorithm. 
3. At first, I thought of compressing each repeated sequence into 3 bytes. The first byte would be 0x80 indicating that that next 2 bytes are special bytes. This information will be used by the decompression algorithm. The second byte would be the number of times the number repeated. The third byte would be the repeated byte itself. This would tell the decompression algorithm, repeat the third byte, 2nd byte number of times to decompress this array. 
4. To further decrease the size of the compressed array, I thought of combining the information the first and 3rd byte provide into 1 byte. Thus, compressing each repeated sequence into 2 bytes. For combining, I decided to add the 0x80 to the value of the repeated value. As the maximum value of the repeated byte could be 0x7F, adding 0x80 to it would still be in bounds for a 8-bit unsigned integer. 
5. If a character repeated more than 0xFF times, I decided to split the sequences each of size <= 0xFF.
6. Next challenge was compact the array. The problem statement says that on compression, the data in the buffer is modified and size of the modified buffer is returned. 
7. My first thought was to use another temporary array. I would copy the elements I want to keep in this temporary array first. Then copy this temporary array into the original data buffer. But this would mean a space complexity of O(n).
8. The efficient way would be to perform the shift in place and use a seperate hash table style data structure to store the indices and lengths of sequences to be deleted. This way the sapce complexity would be O(d) where d is the number of sequences to be deleted. d will be less than n hence O(d) < O(n). 

## Assumptions

1. The data buffer holds 8-bit unsigned integers. 
2. Maximum size of data buffer is 65535 (max 16-bit unsigned value).
3. Indices are 0-based

## Disadvantages of proposed algorithm

1. If there are no consecutive repeated numbers in the array, this algorithm will not compress the array. 
2. If there all the sequences of consecutive repeated numbers are of size 2, the algorithm will effectively not compress the array. 

## Improvements to algorithm

1. Identify patterns in the numbers - for example 2 numbers repeat itself in a sequence, or the difference between consecutive number is same or has some mathematical patter etc. If such patterns are detected, a similar algorithm can be used to compress such sequences. 

## Improvements to implementation

1. The delete_table array is defined as a global with fixed size. If a pointer to this delete_table could be passed to byte_compress() function, the size of the delete_table could be changes as per the size of the data buffer. This would help in saving space where the data buffer is not of the maximum possible size. 
In the problem statement the signature of the byte_compress() function takes only the data_ptr and data_size, hence I have used the same signature in my implementation. 

## Testing Strategy

1. Unity is used for unit testing byte_compress function (16 cases) and array_compaction (9 cases) function.
2. Edge cases such as if the input array is of size 0 or of maximum possible size of 65535 were tested. 
3. Tested both of these functions for different types of inputs such if the values in the array are all 0x00 (min) or 0x7F (max). If the repetitive sequences are found in the beggining or middle or end of the array. 

## Notes
1. As the open role pertains to an embedded systems domain, I refrained from using mallocs in my code. In embedded systems, it is recommended to not use malloc as we cannot predict the memory usage before run-time and it also poses risk of fragmentation in memory. 



