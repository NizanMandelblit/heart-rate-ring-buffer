#include "heart_rate_gen.h"
#include "ring_buffer.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep

#define SAMPLE_INTERVAL 1 // Interval in seconds to sample the heart rate

static volatile int keep_running_g = 1; // Signal flag, volatile to prevent optimization

void
signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        printf("\nShutting down...\n");
        keep_running_g = 0;
    }
}

int
main(int argc, char *argv[]) {
    printf("Heart Rate Exponential Moving Average Monitor\n");

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <buffer_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse the buffer size
    int buffer_size = atoi(argv[1]);
    if (buffer_size <= 0) {
        fprintf(stderr, "Buffer size must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    // handle graceful exit
    signal(SIGINT, signal_handler);

    // Initialize the ring buffer
    rb_init_buffer(buffer_size);

    // Smoothing factor for EMA calculation
    double smoothing_factor = 0.1;

    printf("Starting heart rate monitor with buffer size: %d\n", buffer_size);

    while (keep_running_g) {
        // Generate a new random heart rate
        int heart_rate = hr_generate_heart_rate();

        // Add the heart rate to the buffer
        hr_update_buffer(heart_rate);

        // Calculate the Exponential Moving Average (EMA)
        double ema = hr_calculate_ema(smoothing_factor);
        
        if (ema == -1.0) {
            fprintf(stderr, "Error: EMA calculation failed\n");
            break;
        }

        // Print the current heart rate and EMA
        printf("New Heart Rate: %d, Current EMA: %.2f\n", heart_rate, ema);

        // Wait for SAMPLE_INTERVAL seconds before generating the next heart rate
        sleep(SAMPLE_INTERVAL);
    }

    // Free the memory allocated for the ring buffer
    rb_free_buffer();

    return EXIT_SUCCESS;
}
