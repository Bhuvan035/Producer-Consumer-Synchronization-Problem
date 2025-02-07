# Producer-Consumer Synchronization in C

## Overview

This project implements the **Producer-Consumer Synchronization** problem using **C**, **POSIX threads (pthread)**, **mutexes**, and **condition variables**. The program demonstrates how multiple producer and consumer threads interact safely using a shared buffer with a fixed size, preventing race conditions and ensuring synchronization.

## Features

- Supports up to **16 producers** and **16 consumers**.
- Implements **mutex locks and condition variables** for synchronization.
- Uses a **bounded buffer** of a maximum size of **10**.
- Prevents deadlocks and race conditions.
- Supports a delay mode to simulate real-world behavior.

## Compilation and Execution

### Compilation

Use the following command to compile the program:

```sh
gcc -o producer_consumer main.c -lpthread
```

### Execution

Run the program with the following arguments:

```sh
./producer_consumer <number_of_producers> <number_of_consumers> <items_per_producer> <buffer_size> <delay>
```

Example:

```sh
./producer_consumer 4 2 5 10 1
```

This means:

- **3 producers**
- **2 consumers**
- **Each producer creates 5 items**
- **Buffer size is 10**
- **Delay is enabled (1)**

## How It Works

1. The **producers** generate items and place them into the shared buffer.
2. The **consumers** remove items from the buffer for processing.
3. **Mutex locks** ensure that only one thread modifies the buffer at a time.
4. **Condition variables** handle synchronization:
   - If the buffer is full, producers wait.
   - If the buffer is empty, consumers wait.
5. The program ensures that consumers do not exceed the number of produced items.

## Constraints

- Maximum **16 producers** and **16 consumers**.
- Maximum **buffer size of 10**.
- The number of consumers should be **less than** `producers × items_per_producer`.

## Future Improvements

- Implementing a graphical representation of the buffer state.
- Adding more robust error handling.
- Implementing a priority-based consumption strategy.

## Contact

For any questions or issues, feel free to reach out via GitHub Issues or email **[bhuvanchandra3008@gmail.com](mailto:bhuvanchandra3008@gmail.com)**.

