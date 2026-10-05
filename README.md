*This project has been created by Zakka01*

# Codexion

## Description

Codexion is a multithreaded C simulation where multiple coders compete for shared dongles under strict time constraints.

Each coder must repeatedly compile, debug, and refactor. To compile, a coder needs to acquire two specific dongles. Since multiple coders may request the same dongles at the same time, the program must coordinate access safely while avoiding deadlocks, starvation, race conditions.

The project implements two scheduling policies:

* **FIFO (First In, First Out):** requests are ordered by arrival order.
* **EDF (Earliest Deadline First):** requests are ordered by their burnout deadline.

A separate monitor thread continuously checks coder deadlines and stops the simulation when a coder burns out.

## Features

* Multithreaded simulation using POSIX threads.
* Concurrent execution when coders do not compete for the same dongles.
* FIFO and EDF scheduling.
* A separate priority heap for each dongle.
* Deadlock prevention.
* Starvation handling through FIFO/EDF ordering.
* Precise burnout detection.
* Clean thread and resource cleanup.

## Instructions

### Compilation

Compile the project with:

```bash
make
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles dongle_cooldown scheduler
```

The scheduler must be either:

```text
fifo
```

or:

```text
edf
```

Example:

```bash
./codexion 5 600 100 100 100 3 0 fifo
```

```bash
./codexion 5 600 100 100 100 3 0 edf
```

### Parameters

| Parameter            | Description                                              |
| -------------------- | -------------------------------------------------------- |
| `number_of_coders`   | Number of coder threads                                  |
| `time_to_burnout`    | Maximum allowed waiting time before burnout              |
| `time_to_compile`    | Time spent compiling                                     |
| `time_to_debug`      | Time spent debugging                                     |
| `time_to_refactor`   | Time spent refactoring                                   |
| `number_of_compiles` | Number of required compile cycles                        |
| `dongle_cooldown`    | Cooldown time before a released dongle becomes available |
| `scheduler`          | `fifo` or `edf`                                          |

## Scheduling

### Per-dongle queues

Instead of using one global queue, each dongle owns its own heap of requests.

A coder always needs two dongles, so its request is inserted into both corresponding heaps.

A coder can acquire its two dongles only when:

1. It is the highest-priority request in the first dongle's heap.
2. It is the highest-priority request in the second dongle's heap.
3. Both dongles are available.

This allows independent coders to work in parallel.

### FIFO

For FIFO, each request receives an arrival priority.

The smaller the priority value, the earlier the request arrived.

```text
C1 → priority 1
C2 → priority 2
C3 → priority 3
```

When coders compete for the same dongle, the earliest request is served first.

### EDF

For EDF, each request is assigned a deadline:

```text
deadline = last_action_time + time_to_burnout
```

The request with the earliest deadline has the highest priority.

If two requests have the same deadline, their arrival priority is used as a tie-breaker.

```text
1. Earliest deadline
2. Earliest arrival if deadlines are equal
```

The deadline is kept fixed while the request is waiting and recalculated for the coder's next compile request.

## Blocking cases handled

### Deadlock prevention

A coder requires two dongles to compile. The scheduler checks both requested dongles before allowing the coder to proceed, preventing a coder from permanently holding one dongle while waiting for another.

The scheduling logic also ensures that both dongles are handled as one request.

### Coffman's conditions

The implementation avoids the conditions that can create a deadlock by controlling how dongles are granted and ensuring that a coder does not remain in a state where it holds one required dongle indefinitely while waiting for the other.

### Starvation prevention

FIFO prevents a coder from being continuously overtaken by newer requests competing for the same dongle.

EDF gives priority to the coder with the earliest deadline, preventing a waiting coder from being ignored indefinitely when the timing parameters are feasible.

### Cooldown handling

After a dongle is released, the cooldown period is respected before that dongle can be acquired again.

This prevents immediate reuse when a cooldown is required.

### Precise burnout detection

A dedicated monitor thread continuously checks unfinished coders against their burnout deadlines.

The monitor uses millisecond timestamps and regularly checks whether:

```text
current_time - last_action_time >= time_to_burnout
```

When burnout occurs, the monitor marks the simulation as finished and wakes waiting threads.

### Log serialization

Multiple coder threads can produce output at nearly the same time. A dedicated print mutex protects every log operation so that complete messages are written without interleaving.

## Thread synchronization mechanisms

### `pthread_mutex_t`

Mutexes protect shared state from concurrent access.

The project uses mutexes for:

* scheduler state and heap operations;
* individual dongle access;
* log output.

For example, scheduler data is protected by `scheduler_lock` so that two coder threads cannot modify the scheduling state simultaneously.

The print mutex ensures that messages such as:

```text
104 2 is compiling
104 3 is compiling
```

are written as complete messages instead of being mixed together.

### `pthread_cond_t`

The condition variable is used to coordinate coder threads with changes in scheduler state.

A coder that cannot currently acquire its required dongles waits on the condition variable instead of continuously performing active work.

When a dongle becomes available or the scheduler state changes, waiting coders are notified with:

```c
pthread_cond_broadcast(&data->scheduler_cond);
```

The monitor also broadcasts when burnout stops the simulation, allowing waiting coders to wake up and terminate.

### Monitor communication

The monitor thread reads shared coder state while holding `scheduler_lock`.

When it detects burnout, it:

1. Sets `scheduler_over`.
2. Signals the condition variable.
3. Causes waiting coder threads to stop.

This provides thread-safe communication between the monitor and coder threads.

### Interruptible waits

Timed condition-variable waits can be used for operations that must stop early when the simulation ends. This allows waiting threads to react to `scheduler_over` instead of always waiting for the full operation duration.

## Architecture

The main components are:

```text
main
 ├── argument parsing
 ├── initialization
 ├── thread creation
 ├── monitor thread
 └── thread joining
```

Each coder runs in its own thread:

```text
coder thread
 └── scheduler
      ├── FIFO
      └── EDF
           └── acquire two dongles
                └── compile
                     └── debug
                          └── refactor
```

Dongles maintain their own request heaps:

```text
D1 → heap
D2 → heap
D3 → heap
...
```

## Resources

* 42 Codexion project subject.
* POSIX Threads documentation: `pthread_create`, `pthread_join`, `pthread_mutex_*`, `pthread_cond_*`.
* CodeVault | Unix Threads in C, playlist on youtube 
* Some Documentation about mutexes, condition variables, deadlocks, and race conditions.

### AI usage

AI tools were used as a learning and debugging aid during development.

They were used for:

* understanding POSIX threads, mutexes, and condition variables;
* understanding FIFO and EDF scheduling concepts;
* reasoning about concurrency and deadlock prevention;
* debugging synchronization and timing issues;
* investigating integer overflow in timestamp/deadline calculations;
* reviewing test outputs and identifying scheduling problems;
* improving code structure and simplifying helper functions.


## Testing

Examples used during development:

```bash
./codexion 5 600 100 100 100 3 0 fifo
```

```bash
./codexion 5 600 100 100 100 3 0 edf
```

```bash
./codexion 30 1200 100 100 100 1 0 fifo
```

Memory leaks can be checked with tools such as:

```bash
leaks --atExit -- ./codexion ...
```

The scheduler was tested with different coder counts, burnout times, cooldowns, compile counts, and both FIFO and EDF modes.
