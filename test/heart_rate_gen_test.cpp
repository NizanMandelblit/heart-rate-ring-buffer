#include "gtest/gtest.h"
extern "C" {
#include "../src/ring_buffer.h"
#include "../src/heart_rate_gen.h"
}

// Test fixture class
class HeartRateTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Initialize the ring buffer before each test
        rb_init_buffer(10);
    }

    void TearDown() override {
        // Free the ring buffer after each test
        rb_free_buffer();
    }
};

// Test hr_generate_heart_rate
TEST_F(HeartRateTest, GenerateHeartRateWithinRange) {
    for (int i = 0; i < 100; i++) {
        int heart_rate = hr_generate_heart_rate();
        EXPECT_GE(heart_rate, HR_MIN_HEART_RATE);
        EXPECT_LE(heart_rate, HR_MAX_HEART_RATE);
    }
}

// Test hr_update_buffer with valid and invalid heart rate values
TEST_F(HeartRateTest, UpdateBufferValidValues) {
    hr_update_buffer(50);
    hr_update_buffer(100);
    hr_update_buffer(150);

    EXPECT_FALSE(rb_is_empty());
    EXPECT_EQ(rb_is_full(), false);

    int element;
    rb_remove_element(&element);
    EXPECT_EQ(element, 50);

    rb_remove_element(&element);
    EXPECT_EQ(element, 100);

    rb_remove_element(&element);
    EXPECT_EQ(element, 150);
}

TEST_F(HeartRateTest, UpdateBufferInvalidValues) {
    hr_update_buffer(10);  // Below minimum heart rate
    hr_update_buffer(200); // Above maximum heart rate

    EXPECT_TRUE(rb_is_empty()); // Invalid values should not be added
}

// Test hr_calculate_ema with valid values
TEST_F(HeartRateTest, CalculateEMABasic) {
    hr_update_buffer(60);
    hr_update_buffer(80);
    hr_update_buffer(100);

    double smoothing_factor = 0.5;
    double ema = hr_calculate_ema(smoothing_factor);

    // EMA for the first value should be initialized to the first value
    EXPECT_NEAR(ema, 100, 1e-3); // Expect EMA around the latest value (simple logic)
}

TEST_F(HeartRateTest, CalculateEMAWithEmptyBuffer) {
    double smoothing_factor = 0.5;
    double ema = hr_calculate_ema(smoothing_factor);

    EXPECT_DOUBLE_EQ(ema, -1.0); // Expect -1.0 if the buffer is empty
}

TEST_F(HeartRateTest, CalculateEMAEdgeCases) {
    hr_update_buffer(70);
    hr_update_buffer(80);
    hr_update_buffer(90);

    // Invalid smoothing factor (too low)
    EXPECT_DOUBLE_EQ(hr_calculate_ema(0.0), -1.0);

    // Invalid smoothing factor (too high)
    EXPECT_DOUBLE_EQ(hr_calculate_ema(1.1), -1.0);
}

// Test integration of heart rate generator and EMA calculation
TEST_F(HeartRateTest, GenerateAndUpdateAndCalculateEMA) {
    for (int i = 0; i < 5; i++) {
        int heart_rate = hr_generate_heart_rate();
        hr_update_buffer(heart_rate);
    }

    EXPECT_FALSE(rb_is_empty());

    double ema = hr_calculate_ema(0.5);
    EXPECT_GT(ema, HR_MIN_HEART_RATE);
    EXPECT_LT(ema, HR_MAX_HEART_RATE); // EMA should be in a valid range
}
