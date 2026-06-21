# Codexion Test Suite

A comprehensive bash-based tester for the Codexion concurrency simulation.
It validates argument handling, burnout detection, successful completion,
scheduling policies, dongle cooldown, output formatting, memory safety, and
stress tolerance — **47 tests** across **9 suites**.

---

## Usage

```bash
./tester.sh                  # run all test suites
./tester.sh --args           # argument validation only
./tester.sh --burnout        # burnout detection only
./tester.sh --completion     # successful completion only
./tester.sh --scheduler      # scheduler policy tests only
./tester.sh --cooldown       # dongle cooldown tests only
./tester.sh --stress         # stress tests only
./tester.sh --output         # output format validation only
./tester.sh --leaks          # valgrind memory check only
./tester.sh --zeros          # zero-value edge cases only
./tester.sh --help           # show usage
```

All suites require the `codexion` binary to be built (`make`).

---

## Test Suites

### 1. Argument Validation (12 tests)

| ID | Description | Expected |
|---|---|---|
| TC-ARGS-01 | No arguments at all | Exit 1 |
| TC-ARGS-02 | Only 1 argument | Exit 1 |
| TC-ARGS-03 | 8 arguments (missing scheduler) | Exit 1 |
| TC-ARGS-04 | 10 arguments (too many) | Exit 1 |
| TC-ARGS-05 | Non-numeric argument (letters) | Exit 1 |
| TC-ARGS-06 | Non-numeric argument (`@`) | Exit 1 |
| TC-ARGS-07 | Negative sign as argument | Exit 1 |
| TC-ARGS-08 | Zero coders (`coder_count < 1`) | Exit 1 |
| TC-ARGS-09 | Invalid scheduler type (`lifo`) | Exit 1 |
| TC-ARGS-10 | Empty string argument | Exit 1 |
| TC-ARGS-11 | Zero `time_to_burnout` (valid, instant burnout) | Exit 0 |
| TC-ARGS-12 | Large numeric values (5000) | Exit 0 |

### 2. Burnout Detection (6 tests)

| ID | Description | Expected |
|---|---|---|
| TC-BURN-01 | Burnout time = 0ms (instant burnout) | Exit 0, log contains `burned out` |
| TC-BURN-02 | Tight burnout (1ms) + long compile (500ms), FIFO | Burnout occurs |
| TC-BURN-03 | Same as BURN-02 with EDF scheduler | Burnout occurs |
| TC-BURN-04 | Generous times, few compiles | No burnout, all complete |
| TC-BURN-05 | Single coder (can never acquire 2 dongles) | Burnout occurs |
| TC-BURN-06 | Verify no log lines appear after `burned out` | Last line is the burnout |

### 3. Successful Completion (5 tests)

| ID | Description | Expected |
|---|---|---|
| TC-COMP-01 | 3 coders, 2 compiles each, FIFO | Exit 0, >=6 compilations |
| TC-COMP-02 | 10 coders, 3 compiles each, FIFO | Exit 0, >=30 compilations |
| TC-COMP-03 | 5 coders, 2 compiles each, EDF | Exit 0, >=10 compilations |
| TC-COMP-04 | 4 coders, 1 compile required | Exit 0, exactly 4 compilations |
| TC-COMP-05 | 0 required compiles (immediate success) | Exit 0 |

### 4. Scheduler Policies (4 tests)

| ID | Description | Expected |
|---|---|---|
| TC-SCHED-01 | Basic FIFO run | Exit 0 |
| TC-SCHED-02 | Basic EDF run | Exit 0 |
| TC-SCHED-03 | Both FIFO and EDF run without crash | Both exit 0 |
| TC-SCHED-04 | FIFO determinism — same args twice | Same compile count on both runs |

### 5. Dongle Cooldown (3 tests)

| ID | Description | Expected |
|---|---|---|
| TC-COOL-01 | Zero cooldown | Exit 0 |
| TC-COOL-02 | Cooldown = 100ms | Exit 0 |
| TC-COOL-03 | Cooldown = 500ms | Exit 0 |

### 6. Stress Tests (6 tests)

| ID | Description | Expected |
|---|---|---|
| TC-STRESS-01 | 50 coders, FIFO | Exit 0 |
| TC-STRESS-02 | 50 coders, EDF | Exit 0 |
| TC-STRESS-03 | 1 coder, all zero times (burnout) | Log contains `burned out` |
| TC-STRESS-04 | 100 coders, quick compile | Exit 0 |
| TC-STRESS-05 | Tight borderline (200ms cycle, 300ms burnout) | Exit 0 (or burnout) |
| TC-STRESS-06 | 10 consecutive runs (2 coders, 2 compiles) | All 10 exit 0 |

### 7. Output Validation (3 tests)

| ID | Description | Expected |
|---|---|---|
| TC-OUT-01 | All lines match `<timestamp> <cid> <message>` | 100% format match |
| TC-OUT-02 | Only valid state messages | No unknown messages |
| TC-OUT-03 | Timestamps are monotonically non-decreasing | No timestamp goes backwards |

### 8. Memory Leaks (1 test, requires `valgrind`)

| ID | Description | Expected |
|---|---|---|
| TC-LEAK-01 | Valgrind `--leak-check=full` on a standard run | No errors, no leaks |

If `valgrind` is not installed this suite is silently skipped.

### 9. Zero-value Edge Cases (2 tests)

| ID | Description | Expected |
|---|---|---|
| TC-ZERO-01 | All times zero, 0 required compiles, 2 coders | Exit 0 |
| TC-ZERO-02 | Zero cooldown, 5 coders, normal run | Exit 0 |

---

## Implementation Details

- Each test invokes `codexion` with a **30-second timeout** to prevent hanging on
  pathological inputs.
- Stdout and stderr are captured to temporary files; output assertions run
  **before** cleanup to avoid race-on-read bugs.
- Coloured output distinguishes `✓` passed, `✗` failed, and `~` skipped.
- Exit code is `0` when all tests pass, non-zero otherwise.
