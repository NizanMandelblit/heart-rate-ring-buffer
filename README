# Circular Buffer and Heart Rate Generator

## Table of Contents

- [Circular Buffer and Heart Rate Generator](#circular-buffer-and-heart-rate-generator)
  - [Table of Contents](#table-of-contents)
  - [Objectives](#objectives)
  - [Implementation and Design](#implementation-and-design)
    - [layout](#layout)
    - [Circular Buffer](#circular-buffer)
    - [Heart Rate Generator](#heart-rate-generator)
    - [Flow of the Program](#flow-of-the-program)
  - [Building and Running](#building-and-running)
    - [Testing](#testing)
      - [Memory Leak Detection (Linux)](#memory-leak-detection-linux)
  - [License](#license)
    - [Author](#author)

## Objectives

* Implementing a circular buffer data structure
* Implementing a random heart-rate generator
* Writing a custom Makefile for compilation
* Documenting work with a comprehensive README.
* Use `Git` for version control
* Test the implementation with a test suite using `Google Test` framework


## Implementation and Design

### layout
The project layout is as follows:
```
$ tree
.
├── build                   # Generated build files (created after running Makefile)
├── Makefile                # Custom Makefile for building the project
├── README                  # Documentation
├── src                     
│   ├── heart_rate_gen.c    # Heart rate generator implementation
│   ├── heart_rate_gen.h    # Heart rate generator header
│   ├── main.c              # Entry point for the main program
│   ├── ring_buffer.c       # Circular buffer implementation
│   └── ring_buffer.h       # Circular buffer header
└── test                    # Unit tests
    ├── CMakeLists.txt      # CMake configuration for tests
    ├── heart_rate_gen_test.cpp  # Tests for heart rate generator
    └── ring_buffer_test.cpp     # Tests for circular buffer

```

### Circular Buffer


A circular buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end. This structure lends itself easily to buffering data streams. The circular buffer is a FIFO data structure that uses a single, fixed-size buffer as if it were connected end-to-end. This structure lends itself easily to buffering data streams. The circular buffer is implemented in the `ring_buffer.c` and `ring_buffer.h` files and is thread-safe.

I decided to implement the  ring buffer in a global static instance pattern, so that only one instance of the ring buffer can be created at a time, as I felt this is all that is needed for the heart rate generator.

Notes and examples of the API functions are as follows (please also refer to documentation in the header file for more details):
* `rb_init_buffer`: must be called before using any other API functions.
Failure to initialize the buffer will result in errors when attempting to use the API. initialing again has no effect besides error log, and the buffer must be freed before reinitializing.
`rb_init_buffer(10); // Initialize buffer with size 10`


* `rb_add_element`: Adds an element to the ring buffer. If the buffer is full, it overwrites the oldest element.
```c
rb_init_buffer(2);   // Initialize buffer with size 2
rb_add_element(42);  // Add element 42
rb_add_element(84);  // Add element 84
rb_add_element(126); // buffer is full, overwrites oldest element which is 42 in 
                     // FIFO order
```

* `rb_remove_element`: Removes the oldest element from the ring buffer. Returns the removed element through a pointer.
```c
int value;
rb_init_buffer(5);
rb_add_element(42);
if (rb_remove_element(&value)) {
    printf("Removed: %d\n", value); // "Removed: 42"
} else {
    printf("Buffer is empty\n");
}
```


* `rb_is_full`: Checks if the buffer is full.
```c
rb_init_buffer(2);
rb_add_element(10);
rb_add_element(20);
if (rb_is_full()) { // returns true
    printf("Buffer is full\n");
}
```


* `rb_is_empty`: Checks if the buffer is empty.
```c
rb_init_buffer(3);
if (rb_is_empty()) { // returns true
    printf("Buffer is empty\n");
}
rb_add_element(42);
if (!rb_is_empty()) { // returns false
    printf("Buffer is not empty\n");
}
```

* `rb_free_buffer`: the buffer must be freed using `rb_free_buffer()` before the program exits to avoid memory leaks. calling `rb_free_buffer()` more than once or before initializing has no effect besides error log.
* `rb_get_element_at` : Retrieves an element at a specific index from the buffer without removing it, with bounds checking.
* `rb_is_initialized` : returns true if the buffer is initialized, false otherwise.
* `rb_get_last_element` : Retrieves the last element added to the buffer without removing it.

### Heart Rate Generator

The heart rate generator simulates random heart rate values, providing a practical example of how to use the circular buffer for real-time data processing. The heart rate generator is implemented in the `heart_rate_gen.c` and `heart_rate_gen.h` files.


### Flow of the Program

``` mermaid
graph TD
    A[Start Program] --> B[Parse Command-Line Argument]
    B -->|Valid Buffer Size| C[Initialize Circular Buffer]
    B -->|Invalid Buffer Size| Z[Exit Program with Error]
    C --> D[Setup Signal Handlers]
    D --> E[Generate Heart Rate]
    E --> F[Update Buffer with value]
    F --> G[Calculate EMA]
    G --> H[Print Heart Rate and EMA]
    H --> I{Keep Running?}
    I -->|Yes| E
    I -->|SIGTERM or SIGINT| J[Free Ring Buffer Resources]
    J --> K[Exit Program]
    Z --> K
```

## Building and Running

The project uses a custom `Makefile` for compilation on different platforms.
We assume `MinGW` is installed on Windows for building and testing.
The `Makefile` has the following targets:
```
* all        - Build the main program (default)
* run ARGS=   - Build and run the main program with arguments
* test       - Build and run tests
* memcheck   - Run the tests with valgrind (Linux)
* clean      - Remove build files
* help       - Show this help message and exit
```

to run the program, simple run `make run ARGS=<buffer-size>` where `<buffer-size>` is the  desired size of the circular buffer, and is between the range of 1 and `INT_MAX`.

### Testing

The project uses the `Google Test` framework for testing. The tests are located in the `test` directory.
The tests can be run by executing the following commands:
```
$ make test
```

#### Memory Leak Detection (Linux)

The project uses `valgrind` for memory leak detection. The memory leak detection can be run by executing the following commands:
```
$ make memcheck
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Author

This project was written by **Nizan Mandelblit** (nizanm91@gmail.com) as part of an Embedded engineer role assignment.
