#include "ring_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *buffer;  // Since size is not known at compile time, we need to use a pointer to allocate
                  // memory dynamically
    size_t size;  // Size of the buffer itself
    size_t head;  // Index of the first element
    size_t tail;  // Index of the last element
    size_t count; // Number of elements in the buffer
    bool is_full; // Flag to indicate if the buffer is full
    bool is_initialized;  // Flag to indicate if the buffer is initialized
    pthread_mutex_t lock; // Mutex for thread safety
} ring_buffer_t;

static ring_buffer_t ring_buffer_g = { 0 };

void
rb_init_buffer(int size) {
    if (ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer already initialized\n");
        return;
    }
    pthread_mutex_init(&ring_buffer_g.lock, NULL);

    pthread_mutex_lock(&ring_buffer_g.lock);

    ring_buffer_g.buffer = (int *)malloc(size * sizeof(int));
    if (!ring_buffer_g.buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        pthread_mutex_unlock(&ring_buffer_g.lock);
        exit(EXIT_FAILURE);
    }

    ring_buffer_g.size = size;
    ring_buffer_g.head = 0;
    ring_buffer_g.tail = 0;
    ring_buffer_g.is_full = false;
    ring_buffer_g.is_initialized = true;

    pthread_mutex_unlock(&ring_buffer_g.lock);
}

void
rb_add_element(int element) {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return;
    }
    pthread_mutex_lock(&ring_buffer_g.lock);

    ring_buffer_g.buffer[ring_buffer_g.tail] = element;
    ring_buffer_g.tail = (ring_buffer_g.tail + 1) % ring_buffer_g.size;

    if (ring_buffer_g.is_full) {
        // If full, advance the head to overwrite the oldest data
        ring_buffer_g.head = (ring_buffer_g.head + 1) % ring_buffer_g.size;
    } else {
        // Increment the count if the buffer is not full
        ring_buffer_g.count++;
    }

    // Mark the buffer as full if tail catches up to head
    ring_buffer_g.is_full = (ring_buffer_g.tail == ring_buffer_g.head);

    pthread_mutex_unlock(&ring_buffer_g.lock);
}

bool
rb_remove_element(int *element) {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return false;
    }

    pthread_mutex_lock(&ring_buffer_g.lock);

    if (ring_buffer_g.count == 0) {
        // Buffer is empty
        pthread_mutex_unlock(&ring_buffer_g.lock);
        fprintf(stderr, "Buffer is empty\n");
        return false; // Error
    }
    if (element != NULL)
        *element = ring_buffer_g.buffer[ring_buffer_g.head];
    ring_buffer_g.head = (ring_buffer_g.head + 1) % ring_buffer_g.size;

    ring_buffer_g.count--;

    // Buffer is no longer full after removing an element
    ring_buffer_g.is_full = false;

    pthread_mutex_unlock(&ring_buffer_g.lock);
    return true;
}

bool
rb_is_full() {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return false;
    }
    pthread_mutex_lock(&ring_buffer_g.lock);

    bool is_full = ring_buffer_g.is_full;

    pthread_mutex_unlock(&ring_buffer_g.lock);
    return is_full;
}

bool
rb_is_empty() {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return false;
    }

    pthread_mutex_lock(&ring_buffer_g.lock);

    bool is_empty = (!ring_buffer_g.is_full && (ring_buffer_g.head == ring_buffer_g.tail));

    pthread_mutex_unlock(&ring_buffer_g.lock);
    return is_empty;
}

void
rb_free_buffer() {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return;
    }

    pthread_mutex_lock(&ring_buffer_g.lock);

    free(ring_buffer_g.buffer);
    pthread_mutex_unlock(&ring_buffer_g.lock);
    pthread_mutex_destroy(&ring_buffer_g.lock);

    memset(&ring_buffer_g, 0, sizeof(ring_buffer_t));
}

bool
rb_is_initialized() {
    // no need to lock since this is a read-only operation
    return ring_buffer_g.is_initialized;
}

int
rb_get_element_at(int index, int *element) {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return -1;
    }

    if (!element)
        return -1;

    if (index < 0 || (size_t)index >= ring_buffer_g.count) {
        fprintf(stderr, "Index out of bounds: %d\n", index);
        return -1;
    }

    pthread_mutex_lock(&ring_buffer_g.lock);

    int actual_index = (ring_buffer_g.head + index) % ring_buffer_g.size;
    *element = ring_buffer_g.buffer[actual_index];

    pthread_mutex_unlock(&ring_buffer_g.lock);
    return 0;
}

int
rb_get_last_element(int *element) {
    if (!ring_buffer_g.is_initialized) {
        fprintf(stderr, "Buffer not initialized\n");
        return -1;
    }

    if (!element)
        return -1;

    pthread_mutex_lock(&ring_buffer_g.lock);

    if (ring_buffer_g.count == 0) {
        fprintf(stderr, "Buffer is empty\n");
        pthread_mutex_unlock(&ring_buffer_g.lock);
        return -1;
    }

    int last_index =
        (ring_buffer_g.tail == 0) ? (ring_buffer_g.size - 1) : (ring_buffer_g.tail - 1);
    *element = ring_buffer_g.buffer[last_index];

    pthread_mutex_unlock(&ring_buffer_g.lock);
    return 0;
}
