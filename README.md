*This project has been created as part of the 42 curriculum by alaktaou.*

# Codexion

> Master the race for resources before the deadline masters you.

## Description

Codexion is a concurrency simulation written in C using POSIX threads. It
models a circular co-working hub where several *coders* compete for a limited
number of shared USB *dongles* in order to compile quantum code.

- There are as many dongles as coders, arranged in a circle. Each coder has one
  dongle on their left and one on their right; each dongle is shared by the two
  coders sitting next to it.
- Compiling requires **two dongles held simultaneously** (one in each hand).
- A coder cycles through three mutually exclusive states: **compiling →
  debugging → refactoring → compiling → ...**
- If a coder does not *start compiling* within `time_to_burnout` milliseconds of
  the start of their previous compile (or of the simulation), they **burn out**
  and the simulation stops.
- The simulation also stops, successfully, once every coder has compiled at
  least `number_of_compiles_required` times.

Each coder is an independent thread. Dongles are protected by mutexes and
condition variables, a dedicated monitor thread watches for burnout, and a
priority queue (a small binary heap) arbitrates dongle access according to the
chosen scheduling policy.

## Instructions

### Build

```bash
make          # build the codexion binary
make clean    # remove object files
make fclean   # remove object files and the binary
make re       # rebuild from scratch
```

The project is compiled with `cc -Wall -Wextra -Werror -pthread`.

### Run

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument                       | Meaning                                                                 |
| ------------------------------ | ----------------------------------------------------------------------- |
| `number_of_coders`             | Number of coders (and dongles). Must be `>= 1`.                         |
| `time_to_burnout` (ms)         | Max delay before a coder must start a new compile, or they burn out.    |
| `time_to_compile` (ms)         | Time spent compiling (while holding two dongles).                       |
| `time_to_debug` (ms)           | Time spent debugging.                                                    |
| `time_to_refactor` (ms)        | Time spent refactoring.                                                  |
| `number_of_compiles_required`  | If every coder reaches this many compiles, the simulation stops.        |
| `dongle_cooldown` (ms)         | A released dongle stays unavailable for this long.                      |
| `scheduler`                    | Arbitration policy: exactly `fifo` or `edf`.                            |

All arguments are mandatory. Invalid inputs (missing arguments, negative
numbers, non-integers, or an unknown scheduler) are rejected with an error
message and a non-zero exit code.

#### Scheduler policies

- **`fifo`** — First In, First Out: a dongle is granted to the coder whose
  request arrived first.
- **`edf`** — Earliest Deadline First: the dongle is granted to the coder with
  the earliest burnout deadline (`last_compile_start + time_to_burnout`). Ties
  are broken by arrival order, then by coder id, guaranteeing a fully
  deterministic policy.

#### Example

```bash
$ ./codexion 5 800 200 200 200 3 50 fifo
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
...
```

State changes are logged as `timestamp_in_ms X <state>`, where `X` is the coder
number. The possible state messages are `has taken a dongle`, `is compiling`,
`is debugging`, `is refactoring`, and `burned out`.

## Feature list

- One thread per coder plus one monitor thread.
- Per-dongle mutex + condition variable.
- Mandatory dongle cooldown after release.
- FIFO and EDF arbitration backed by a hand-written binary heap (no standard
  library priority queue is used).
- Precise burnout detection (reported within 10 ms of the actual burnout).
- Fully serialized logging (no interleaved lines, nothing printed after
  `burned out`).
- No global variables and no memory leaks.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions).** The classic circular-wait
  scenario (every coder grabs their left dongle and waits forever for the right
  one) is broken by **resource ordering**: a coder always requests and acquires
  the lower-id dongle first and the higher-id dongle second. Breaking the
  circular-wait condition is sufficient to make deadlock impossible.
- **Starvation prevention / liveness.** Access to each dongle is arbitrated by
  a priority queue rather than by raw `pthread_mutex_lock` races. Under `fifo`
  the oldest request always wins; under `edf` the most endangered coder (the
  one closest to burning out) is served first, with deterministic tie-breaking,
  so no coder is starved when the parameters are feasible.
- **Cooldown handling.** When a dongle is released, its `cooldown_until`
  timestamp is set to `now + dongle_cooldown`. A waiting coder may only acquire
  the dongle once that absolute time has passed; the condition variable is
  re-evaluated on a short timed wait so the cooldown is respected without
  busy-spinning.
- **Precise burnout detection.** A dedicated monitor thread polls every coder's
  `last_compile_start` (read under the coder's mutex) at sub-millisecond
  granularity and reports a burnout within 10 ms of the deadline being missed.
- **Log serialization.** All output goes through a single logger guarded by a
  logging mutex, so two messages never interleave on one line. Once the
  simulation has stopped, non-fatal state messages are suppressed, guaranteeing
  that nothing is ever printed after a `burned out` line.
- **Clean shutdown.** Stopping the simulation sets a mutex-protected flag and
  broadcasts every dongle's condition variable, so coders blocked while waiting
  for a dongle wake up, observe the stop flag, release anything they hold, and
  exit. Every thread is joined and every allocation freed.
- **Single-coder edge case.** With one coder there is only one dongle. The coder
  takes it, can never acquire a second dongle, and therefore burns out — exactly
  the expected behaviour.

## Thread synchronization mechanisms

- **`pthread_mutex_t` (per dongle).** Protects each dongle's mutable state
  (`taken`, `owner`, `cooldown_until`, and its request queue). Acquiring,
  releasing, and arbitrating a dongle all happen while holding this mutex, which
  prevents two coders from ever "duplicating" the same dongle.
- **`pthread_cond_t` (per dongle).** Implements the waiting queue. A coder that
  cannot yet take a dongle waits on the condition variable via
  `pthread_cond_timedwait` (using an absolute deadline so it also wakes to
  re-check the cooldown and stop flag). Releasing a dongle, pushing a new
  request, or stopping the simulation all `pthread_cond_broadcast` the relevant
  dongles so waiters re-evaluate their predicate.
- **`pthread_mutex_t` (per coder).** Guards `last_compile_start` and
  `compile_count`. These fields are written by the coder thread and read by the
  monitor thread; the mutex prevents that read/write race, so burnout detection
  and the "all reached" check always observe consistent values.
- **`pthread_mutex_t` (logger).** Serializes all `printf` output so log lines
  never interleave and so the simulation can guarantee no message follows the
  `burned out` line.
- **`pthread_mutex_t` (stop flag).** Protects the boolean `stop` flag that
  signals termination. It is read through `is_stopped()` and written through
  `stop_simulation()`, which also broadcasts every dongle to wake sleeping
  coders.

### How race conditions are prevented

Every piece of shared state has exactly one mutex responsible for it, and that
state is only ever touched while the corresponding mutex is held:

- Dongle availability/ownership ⟶ the dongle mutex.
- A coder's compile bookkeeping ⟶ that coder's mutex.
- Program output ⟶ the logger mutex.
- The termination signal ⟶ the stop mutex.

Thread-safe communication between the coders and the monitor is therefore done
entirely through these mutex-protected fields: coders publish their progress by
updating `last_compile_start`/`compile_count` under their own mutex, and the
monitor consumes that information under the same mutex, then broadcasts the stop
condition to bring the whole system to a clean halt.

The implementation has been verified to be free of data races with
ThreadSanitizer and free of memory leaks with Valgrind.

## Resources

- [POSIX Threads Programming (LLNL tutorial)](https://hpc-tutorials.llnl.gov/posix/)
- `man` pages: `pthread_create`, `pthread_mutex_init`, `pthread_cond_wait`,
  `pthread_cond_timedwait`, `pthread_cond_broadcast`, `gettimeofday`, `usleep`.
- Edsger W. Dijkstra, *Hierarchical ordering of sequential processes* — the
  Dining Philosophers problem and resource-ordering deadlock avoidance.
- E. G. Coffman, M. Elphick, A. Shoshani, *System Deadlocks* — the four
  necessary conditions for deadlock (mutual exclusion, hold-and-wait, no
  preemption, circular wait).
- Liu & Layland, *Scheduling Algorithms for Multiprogramming in a Hard-Real-Time
  Environment* — Earliest Deadline First (EDF) scheduling.
- Binary heap / priority queue fundamentals (Cormen et al., *Introduction to
  Algorithms*).

### Use of AI

AI assistance was used as a reviewing and auditing aid rather than to author the
solution wholesale:

- **Code review / bug hunting:** auditing the threading logic for data races,
  deadlock conditions, the `usleep` millisecond/microsecond mismatch, the
  `pthread_cond_timedwait` absolute-time requirement, and use-after-free in the
  cleanup path.
- **Documentation:** drafting and structuring this README (Description,
  Instructions, Blocking cases handled, Thread synchronization mechanisms).
- **Testing strategy:** suggesting Valgrind and ThreadSanitizer runs and edge
  cases (single coder, zero cooldown, tight burnout) to validate the
  implementation.

All AI-suggested changes were reviewed, understood, and tested before being
kept.
