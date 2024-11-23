#include "heart_rate_gen.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

static bool is_initialized = false;
static double previous_ema = 0.0;     // Store previous EMA value
static bool first_measurement = true; // Flag for first measurement

int
hr_generate_heart_rate() {
    if (!is_initialized) {
        srand(time(NULL)); // Initialize random seed
        is_initialized = true;
    }

    // Generate random heart rate within the range
    int range = HR_MAX_HEART_RATE - HR_MIN_HEART_RATE + 1;
    return HR_MIN_HEART_RATE + (rand() % range);
}

void
hr_update_buffer(int heart_rate) {
    if (!rb_is_initialized())
        return;

    // Validate heart rate before adding
    if (heart_rate < HR_MIN_HEART_RATE || heart_rate > HR_MAX_HEART_RATE)
        return;

    rb_add_element(heart_rate);
}

double
hr_calculate_ema(double smoothing_factor) {
    if (!rb_is_initialized() || rb_is_empty() || smoothing_factor <= 0.0 || smoothing_factor > 1.0)
        return -1.0;

    int current_value;
    int result = rb_get_last_element(&current_value);
    if (result != 0)
        return -1.0;

    // For first measurement, initialize EMA with first value
    if (first_measurement) {
        previous_ema = current_value;
        first_measurement = false;
        return previous_ema;
    }

    // Calculate EMA using exponential smoothing formula
    // St = α * xt + (1 - α) * St-1
    double new_ema = smoothing_factor * current_value + (1.0 - smoothing_factor) * previous_ema;

    // Store for next calculation
    previous_ema = new_ema;

    return new_ema;
}
