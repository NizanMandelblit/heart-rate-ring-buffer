#ifndef __HEART_RATE_GEN_H__
#define __HEART_RATE_GEN_H__

/*
a random heart rate generator that creates a new random heart rate every second (values can be from
44 to 185 )and stores it in the circular buffer. The size of the circular buffer will be provided as
a command line argument. Every time a new heart rate is generated, store it in the buffer, update
the buffer, and print the Exponential Moving Average (EMA) calculated from all heart rate values in
the buffer.

this module would be prefixed with `hr_`.
 */

#include "ring_buffer.h"

#define HR_MIN_HEART_RATE 44
#define HR_MAX_HEART_RATE 185

/**
 * @brief Generate a random heart rate value.
 *
 * @return int Random heart rate value.
 */
int hr_generate_heart_rate();

/**
 * @brief Calculate the Exponential Moving Average (EMA) of the heart rate values in the buffer.
 *
 * @return double Exponential Moving Average (EMA).
 *          -1.0 if the buffer is not initialized or empty.
 */
double hr_calculate_ema(double smoothing_factor);

/**
 * @brief Update the buffer with a new heart rate value.
 *
 * @param heart_rate New heart rate value.
 */
void hr_update_buffer(int heart_rate);

#endif // __HEART_RATE_GEN_H__
