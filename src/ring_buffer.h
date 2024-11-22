#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
/*
A circular buffer (also known as a ring buffer) is a data structure that uses a single, fixed-size
buffer as if it were connected end-to-end. This structure is useful for buffering data streams, such
as data read from hardware interfaces or network sockets.
Your task is to implement a circular buffer in C with the following functionalities:
1. Initialisation: Create a function to initialise the circular buffer.
2. Add Element: Create a function to add an element to the buffer.
3. Remove Element: Create a function to remove an element from the buffer.
4. Check Full: Create a function to check if the buffer is full.
5. Check Empty: Create a function to check if the buffer is empty.

this module would be prefixed with `rb_`.
this data structure is thread-safe.

*/
#include <pthread.h>
#include <stdbool.h>

/**
 * @brief Initialize the ring buffer with a given size.
 *        Must be called before any other buffer operation.
 *        If the buffer is already initialized, no effect.
 *
 * @param size Size of the buffer to be initialized.
 */
void rb_init_buffer(int size);

/**
 * @brief Add an element to the ring buffer.
 *
 * @param element Element to be added to the buffer.
 */
void rb_add_element(int element);

/**
 * @brief Remove an element from the ring buffer.
 *
 * @param element Pointer to store the removed element (NULL if not needed).
 * @return true If an element was removed.
 * @return false If the buffer is empty.
 */
bool rb_remove_element(int *element);

/**
 * @brief Check if the ring buffer is full.
 *
 * @return true If the buffer is full.
 * @return false If the buffer is not full or if the buffer is not initialized.
 */
bool rb_is_full();

/**
 * @brief Check if the ring buffer is empty.
 *
 * @return true If the buffer is empty.
 * @return false If the buffer is not empty or if the buffer is not initialized.
 */
bool rb_is_empty();

/**
 * @brief Free the memory allocated for the ring buffer.
 */
void rb_free_buffer();

/**
 * @brief Get the element at a specific index in the ring buffer.
 *
 * @param index Index of the element to retrieve.
 * @param element Pointer to store the element, if NULL, the function will return -1.
 * @return int 0 if the element was retrieved successfully.
 */
int rb_get_element_at(int index, int *element);

/**
 * @brief Check if the ring buffer is initialized.
 *
 * @return true If the buffer is initialized.
 * @return false If the buffer is not initialized.
 */
bool rb_is_initialized();


/**
 * @brief Get the last element in the ring buffer.
 *
 * @param element Pointer to store the last element.
 * @return int 0 if the last element was retrieved successfully.
 *             -1 if the buffer is not initialized or empty.
 */
int rb_get_last_element(int *element);

#endif // __RING_BUFFER_H__
