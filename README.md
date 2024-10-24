# multithreaded-generator

A multithreaded C++ application that generates random numbers within a specified range using a producer-consumer pattern. 

## Task Overview

The main objective of this project is to generate all unique random numbers in a specified range (1 to N) using a multithreaded approach. The application implements a producer-consumer pattern where the producer generates random numbers and puts them into the queue, and the consumer processes them.

### Implementation Approaches

This project was implemented in two ways:

1. **Standard Approach with Condition Variables**: This method uses condition variables for thread synchronization, ensuring that the producer and consumer threads communicate effectively.

2. **No Condition Variables Approach**: This alternative method eliminates the need for condition variables, aiming to improve performance by reducing synchronization overhead.

### Performance Testing

Testing both approaches showed that, in this specific case—where the producer works quickly by simply generating random numbers—using condition variables introduces unnecessary overhead. For example, tests conducted with 1,000,000 elements demonstrated that the cv approach took about **1 second longer** to generate all numbers compared to the approach without condition variables.

### Conclusions

- The results suggest that for scenarios where the producer operates rapidly, minimizing synchronization can lead to better performance.
- The difference in execution time highlights the potential benefits of optimizing thread communication in multithreaded applications.

## Project Structure

The project is organized as follows:

- **src/**: Contains the source code files.
- **include/**: Contains header files.
- **external/**: Contains external dependencies, including Google Test as a submodule for unit testing.
- **build/**: The directory where the project is built.

### Google Test

Google Test is included as a submodule. To clone and initialize the project with the submodule, use the following commands:

```bash
git clone --recurse-submodules https://github.com/tbelaya/multithreaded-generator.git
```

## Building

To build the project, navigate to the build directory and run:

```bash
mkdir -p build && cd build
cmake -G Ninja ..
ninja
```

## Continuous Integration

This project uses a CI workflow that includes:

- **Clang-Tidy**: For static code analysis and ensuring code quality.
- **Clang-Format**: For maintaining consistent code formatting.
- **Testing Steps**: Automated tests using Google Test.