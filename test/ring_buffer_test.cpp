#include "gtest/gtest.h"
extern "C" { // This allows C++ to link with C code
#include "../src/ring_buffer.h"
}

// Test fixture class
class RingBufferTest: public ::testing::Test {
  protected:
    void SetUp() override {
        // Initialize buffer with a fixed size before each test
        rb_init_buffer(5);
    }

    void TearDown() override {
        // Free the buffer after each test
        rb_free_buffer();
    }
};

// Test buffer initialization
TEST_F(RingBufferTest, InitializeBuffer) {
    EXPECT_NO_THROW(rb_init_buffer(5)); // Should not throw or fail
    EXPECT_TRUE(rb_is_empty());
    EXPECT_FALSE(rb_is_full());
}

// Test adding elements
TEST_F(RingBufferTest, AddElement) {
    rb_add_element(10);
    EXPECT_FALSE(rb_is_empty());
    EXPECT_FALSE(rb_is_full());
}

// Test removing elements
TEST_F(RingBufferTest, RemoveElement) {
    rb_add_element(20);
    rb_add_element(30);
    int element;

    EXPECT_TRUE(rb_remove_element(&element));
    EXPECT_EQ(element, 20); // FIFO behavior

    EXPECT_TRUE(rb_remove_element(&element));
    EXPECT_EQ(element, 30);

    EXPECT_TRUE(rb_is_empty());
}

// Test overwriting behavior
TEST_F(RingBufferTest, OverwriteWhenFull) {
    rb_add_element(1);
    rb_add_element(2);
    rb_add_element(3);
    rb_add_element(4);
    rb_add_element(5);
    EXPECT_TRUE(rb_is_full());

    rb_add_element(6); // Overwrites oldest element (1)
    int element;
    EXPECT_TRUE(rb_remove_element(&element));
    EXPECT_EQ(element, 2); // Oldest element is now 2
    EXPECT_FALSE(rb_is_empty());
}

// Test buffer full and empty states
TEST_F(RingBufferTest, BufferFullAndEmptyStates) {
    for (int i = 0; i < 5; i++)
        rb_add_element(i + 1);
    EXPECT_TRUE(rb_is_full());
    EXPECT_FALSE(rb_is_empty());

    for (int i = 0; i < 5; i++)
        rb_remove_element(NULL);
    EXPECT_TRUE(rb_is_empty());
    EXPECT_FALSE(rb_is_full());
}

// Test removing from an empty buffer
TEST_F(RingBufferTest, RemoveFromEmptyBuffer) {
    int element;
    EXPECT_FALSE(rb_remove_element(&element)); // Removing from empty buffer should fail
}

// Test double initialization
TEST_F(RingBufferTest, DoubleInitialization) {
    // Attempting to initialize again should not crash or reinitialize
    rb_init_buffer(10);             // Second initialization ignored
    EXPECT_EQ(rb_is_empty(), true); // The buffer remains initialized
}

// Test freeing buffer
TEST_F(RingBufferTest, FreeBuffer) {
    rb_free_buffer();
    EXPECT_FALSE(rb_is_initialized()); // Buffer should not be initialized after free

    // Accessing buffer functions after free should not crash
    EXPECT_FALSE(rb_is_empty());
    EXPECT_FALSE(rb_is_full());
    EXPECT_FALSE(rb_remove_element(NULL));
}

// Test retrieving an element at specific index
TEST_F(RingBufferTest, GetElementAtIndex) {
    rb_add_element(10);
    rb_add_element(20);
    rb_add_element(30);

    int element;
    EXPECT_EQ(rb_get_element_at(1, &element), 0); // Index 1 should be valid
    EXPECT_EQ(element, 20);

    EXPECT_EQ(rb_get_element_at(5, &element), -1); // Invalid index
}
