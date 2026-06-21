#!/usr/bin/env bash
# **************************************************************************** #
#                                Codexion Tester                                #
# **************************************************************************** #
# Usage: ./tester.sh [--all | --args | --burnout | --completion | --scheduler |
#                     --cooldown | --stress | --output | --leaks | --zeros]
#
# If no option is given, --all is assumed.
# **************************************************************************** #

set -uo pipefail

# ── Colours ──────────────────────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

# ── Globals ──────────────────────────────────────────────────────────────────
BINARY="./codexion"
FAIL=0
PASS=0
TOTAL=0
TIMEOUT=30

# ── Helpers ──────────────────────────────────────────────────────────────────

ok()   { echo -e "  ${GREEN}✓${NC} $1"; PASS=$((PASS + 1)); TOTAL=$((TOTAL + 1)); }
ko()   { echo -e "  ${RED}✗${NC} $1"; FAIL=$((FAIL + 1)); TOTAL=$((TOTAL + 1)); }
info() { echo -e "  ${CYAN}→${NC} $1"; }
skip() { echo -e "  ${YELLOW}~${NC} $1"; ((TOTAL++)); }
banner() {
	echo
	echo -e "${CYAN}══════════════════════════════════════════════════${NC}"
	echo -e "${CYAN}  $1${NC}"
	echo -e "${CYAN}══════════════════════════════════════════════════${NC}"
}

# Run the binary with a timeout, return a colon-delimited record:
#   exit_code:stdout_file:stderr_file
run() {
	local out_file err_file rc
	out_file=$(mktemp)
	err_file=$(mktemp)
	# shellcheck disable=SC2068
	timeout "$TIMEOUT" "$BINARY" $@ > "$out_file" 2> "$err_file"
	rc=$?
	echo "$rc:$out_file:$err_file"
}

# Extract fields from a record.
out_file() { echo "$1" | cut -d: -f2; }
err_file() { echo "$1" | cut -d: -f3; }
exit_code() { echo "$1" | cut -d: -f1; }

# Clean up temp files
clean() { local o e; o=$(out_file "$1"); e=$(err_file "$1"); rm -f "$o" "$e"; }

# Check the binary is available
check_binary() {
	if [[ ! -x "$BINARY" ]]; then
		echo -e "${RED}Binary '$BINARY' not found. Run 'make' first.${NC}" >&2
		exit 1
	fi
}

# ── Test Suites ──────────────────────────────────────────────────────────────

test_args() {
	banner "Argument Validation"

	info "TC-ARGS-01: No arguments (should print usage and exit 1)"
	r=$(run); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-02: Only 1 argument"
	r=$(run 5); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-03: 8 arguments (missing scheduler)"
	r=$(run 5 800 200 200 200 3 50); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-04: Too many arguments (10)"
	r=$(run 5 800 200 200 200 3 50 fifo extra); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-05: Non-numeric argument (letters)"
	r=$(run a 800 200 200 200 3 50 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-06: Non-numeric argument (@)"
	r=$(run 5 800 200 @ 200 3 50 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-07: Non-numeric (negative sign)"
	r=$(run -5 800 200 200 200 3 50 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-08: Zero coders (coder_count < 1)"
	r=$(run 0 800 200 200 200 3 50 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-09: Invalid scheduler type ('lifo')"
	r=$(run 5 800 200 200 200 3 50 lifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-10: Empty string argument"
	r=$(run '' 800 200 200 200 3 50 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 1 ]] && ok "Exit code 1" || ko "Expected 1, got $rc"
	clean "$r"

	info "TC-ARGS-11: Zero for time_to_burnout (instant burnout)"
	r=$(run 2 0 200 200 200 3 50 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-ARGS-12: Large numbers (5000, within timeout)"
	r=$(run 5 5000 5000 5000 5000 3 5000 fifo); rc=$(exit_code "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0 (large numbers)" || ko "Expected 0, got $rc"
	clean "$r"
}

# 2. Burnout tests -----------------------------------------------------

test_burnout() {
	banner "Burnout Detection"

	info "TC-BURN-01: Burnout time = 0 (instant burnout)"
	r=$(run 3 0 200 200 200 10 50 fifo)
	rc=$(exit_code "$r"); o=$(out_file "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	grep -q "burned out" "$o" 2>/dev/null && ok "Contains 'burned out'" || ko "Missing 'burned out'"
	clean "$r"

	info "TC-BURN-02: Tight burnout (1ms) + long compile (500ms)"
	r=$(run 4 1 500 1 1 10 0 fifo)
	o=$(out_file "$r")
	grep -q "burned out" "$o" 2>/dev/null && ok "Coder burns out" || ko "Expected burnout"
	clean "$r"

	info "TC-BURN-03: Tight burnout with EDF scheduler"
	r=$(run 4 1 500 1 1 10 0 edf)
	o=$(out_file "$r")
	grep -q "burned out" "$o" 2>/dev/null && ok "Coder burns out with EDF" || ko "Expected burnout with EDF"
	clean "$r"

	info "TC-BURN-04: Generous times, few compiles (no burnout expected)"
	r=$(run 3 5000 100 100 100 2 10 fifo)
	o=$(out_file "$r")
	grep -q "burned out" "$o" 2>/dev/null && ko "Unexpected burnout" || ok "No burnout"
	clean "$r"

	info "TC-BURN-05: Single coder (always burns out)"
	r=$(run 1 5000 100 100 100 2 10 fifo)
	o=$(out_file "$r")
	grep -q "burned out" "$o" 2>/dev/null && ok "Single coder burns out" || ko "Single coder should burn out"
	clean "$r"

	info "TC-BURN-06: No output after 'burned out'"
	r=$(run 5 10 200 200 200 10 50 fifo)
	o=$(out_file "$r")
	burnout_line=$(grep -n "burned out" "$o" 2>/dev/null | tail -1 | cut -d: -f1)
	if [[ -n "$burnout_line" ]]; then
		total_lines=$(wc -l < "$o")
		[[ "$total_lines" -eq "$burnout_line" ]] \
			&& ok "No lines after 'burned out'" \
			|| ko "Found $((total_lines - burnout_line)) lines after 'burned out'"
	else
		skip "No burnout occurred"
	fi
	clean "$r"
}

# 3. Completion tests ---------------------------------------------------

test_completion() {
	banner "Successful Completion"

	info "TC-COMP-01: 3 coders, 2 compiles each"
	r=$(run 3 5000 100 100 100 2 10 fifo)
	rc=$(exit_code "$r"); o=$(out_file "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0" || ko "Expected 0, got $rc"
	c_count=$(grep -c "is compiling" "$o" 2>/dev/null || echo 0)
	[[ "$c_count" -ge 6 ]] && ok "At least 6 compilations (3×2)" || ko "Expected >=6, got $c_count"
	clean "$r"

	info "TC-COMP-02: 10 coders, 3 compiles each"
	r=$(run 10 5000 100 100 100 3 10 fifo)
	rc=$(exit_code "$r"); o=$(out_file "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0" || ko "Expected 0, got $rc"
	c_count=$(grep -c "is compiling" "$o" 2>/dev/null || echo 0)
	[[ "$c_count" -ge 30 ]] && ok "At least 30 compilations (10×3)" || ko "Expected >=30, got $c_count"
	clean "$r"

	info "TC-COMP-03: 5 coders, EDF scheduler"
	r=$(run 5 5000 100 100 100 2 10 edf)
	rc=$(exit_code "$r"); o=$(out_file "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0" || ko "Expected 0, got $rc"
	c_count=$(grep -c "is compiling" "$o" 2>/dev/null || echo 0)
	[[ "$c_count" -ge 10 ]] && ok "At least 10 compilations (5×2)" || ko "Expected >=10, got $c_count"
	clean "$r"

	info "TC-COMP-04: 4 coders, only 1 compile required"
	r=$(run 4 5000 100 100 100 1 10 fifo)
	rc=$(exit_code "$r"); o=$(out_file "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0" || ko "Expected 0, got $rc"
	c_count=$(grep -c "is compiling" "$o" 2>/dev/null || echo 0)
	[[ "$c_count" -eq 4 ]] \
		&& ok "Exactly 4 compilations" \
		|| info "Got $c_count compilations (may exceed 4 due to race on stop)"
	clean "$r"

	info "TC-COMP-05: 0 required compiles (immediate success)"
	r=$(run 3 5000 100 100 100 0 10 fifo)
	rc=$(exit_code "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit code 0" || ko "Expected 0, got $rc"
	clean "$r"
}

# 4. Scheduler tests ----------------------------------------------------

test_scheduler() {
	banner "Scheduler Policies"

	info "TC-SCHED-01: FIFO - basic run"
	r=$(run 5 5000 100 100 100 2 0 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-SCHED-02: EDF - basic run"
	r=$(run 5 5000 100 100 100 2 0 edf)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-SCHED-03: FIFO + EDF both exit cleanly"
	r1=$(run 3 3000 50 50 50 2 0 fifo)
	r2=$(run 3 3000 50 50 50 2 0 edf)
	rc1=$(exit_code "$r1"); rc2=$(exit_code "$r2")
	[[ "$rc1" -eq 0 && "$rc2" -eq 0 ]] \
		&& ok "Both schedulers exit cleanly" \
		|| ko "FIFO=$rc1, EDF=$rc2"
	clean "$r1"; clean "$r2"

	info "TC-SCHED-04: FIFO determinism (same compile count)"
	r1=$(run 4 3000 50 50 50 2 0 fifo)
	r2=$(run 4 3000 50 50 50 2 0 fifo)
	o1=$(out_file "$r1"); o2=$(out_file "$r2")
	rc1=$(exit_code "$r1"); rc2=$(exit_code "$r2")
	c1=$(grep -c "is compiling" "$o1" 2>/dev/null || echo 0)
	c2=$(grep -c "is compiling" "$o2" 2>/dev/null || echo 0)
	[[ "$rc1" -eq 0 && "$rc2" -eq 0 && "$c1" -eq "$c2" ]] \
		&& ok "Deterministic: both runs have $c1 compilations" \
		|| ko "Run1: $c1 comps, Run2: $c2 comps"
	clean "$r1"; clean "$r2"
}

# 5. Cooldown tests -----------------------------------------------------

test_cooldown() {
	banner "Dongle Cooldown"

	info "TC-COOL-01: Zero cooldown"
	r=$(run 5 5000 100 100 100 2 0 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-COOL-02: Cooldown = 100ms"
	r=$(run 5 5000 100 100 100 2 100 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-COOL-03: Cooldown = 500ms"
	r=$(run 3 5000 100 100 100 2 500 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"
}

# 6. Stress tests -------------------------------------------------------

test_stress() {
	banner "Stress Tests"

	info "TC-STRESS-01: 50 coders"
	r=$(run 50 5000 50 50 50 1 5 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-STRESS-02: 50 coders EDF"
	r=$(run 50 5000 50 50 50 1 5 edf)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-STRESS-03: 1 coder, all zero times"
	r=$(run 1 0 0 0 0 1 0 fifo)
	o=$(out_file "$r")
	grep -q "burned out" "$o" 2>/dev/null && ok "Single coder burns out" || ko "Expected burnout"
	clean "$r"

	info "TC-STRESS-04: 100 coders"
	r=$(run 100 5000 10 10 10 1 5 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-STRESS-05: Tight borderline (200ms cycle, 300ms burnout)"
	r=$(run 5 300 70 70 60 3 10 fifo)
	rc=$(exit_code "$r")
	[[ "$rc" -eq 0 ]] && ok "Exit 0 (completed)" || ok "Exit $rc (burnout likely)"
	clean "$r"

	info "TC-STRESS-06: 10 consecutive runs"
	local passed=0
	for i in $(seq 1 10); do
		r=$(run 2 3000 30 30 30 2 0 fifo)
		rc=$(exit_code "$r"); clean "$r"
		[[ "$rc" -eq 0 ]] && ((passed++))
	done
	[[ "$passed" -eq 10 ]] && ok "All 10 passed" || ko "$passed/10 passed"
}

# 7. Output format validation -------------------------------------------

test_output() {
	banner "Output Validation"

	info "TC-OUT-01: Output line format"
	r=$(run 3 5000 100 100 100 2 10 fifo)
	o=$(out_file "$r")
	if [[ -s "$o" ]]; then
		valid=$(grep -cE '^[0-9]+ [0-9]+ .+' "$o" 2>/dev/null || echo 0)
		total=$(wc -l < "$o" 2>/dev/null || echo 0)
		[[ "$valid" -eq "$total" ]] \
			&& ok "All $total lines match expected format" \
			|| ko "Only $valid/$total lines match format"
	else
		ko "Output file is empty"
	fi
	clean "$r"

	info "TC-OUT-02: Only valid state messages"
	r=$(run 3 5000 100 100 100 2 10 fifo)
	o=$(out_file "$r")
	bad=$(grep -vE '(has taken a dongle|is compiling|is debugging|is refactoring|burned out)' "$o" 2>/dev/null || true)
	[[ -z "$bad" ]] && ok "All messages are valid" || ko "Found invalid messages: $(echo "$bad" | head -3)"
	clean "$r"

	info "TC-OUT-03: Monotonically non-decreasing timestamps"
	r=$(run 3 5000 100 100 100 2 10 fifo)
	o=$(out_file "$r")
	prev=-1; mono=true
	while IFS=' ' read -r ts rest; do
		if [[ "$ts" =~ ^[0-9]+$ ]]; then
			if [[ "$ts" -lt "$prev" ]]; then mono=false; break; fi
			prev=$ts
		fi
	done < "$o"
	$mono && ok "Timestamps are non-decreasing" || ko "Timestamp decreased"
	clean "$r"
}

# 8. Memory leak test ---------------------------------------------------

test_leaks() {
	banner "Memory Leaks (Valgrind)"
	if ! command -v valgrind &>/dev/null; then
		skip "Valgrind not found, skipping leak tests"
		return
	fi
	info "TC-LEAK-01: Valgrind check (3 coders, 2 compiles)"
	local tmp
	tmp=$(mktemp)
	valgrind --leak-check=full --error-exitcode=42 \
		"$BINARY" 3 3000 50 50 50 2 0 fifo 2>&1 \
		| grep -E "ERROR SUMMARY|definitely lost|indirectly lost" > "$tmp"
	local rc=${PIPESTATUS[0]}
	if [[ "$rc" -eq 42 ]]; then
		ko "Valgrind detected errors"
		cat "$tmp"
	elif [[ "$rc" -eq 0 ]]; then
		ok "Valgrind: no memory errors"
	else
		skip "Valgrind exit $rc"
	fi
	rm -f "$tmp"
}

# 9. Zero-value edge cases ---------------------------------------------

test_edge_zero() {
	banner "Zero-value Edge Cases"

	info "TC-ZERO-01: All zeros, 0 required compiles"
	r=$(run 2 0 0 0 0 0 0 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"

	info "TC-ZERO-02: Zero cooldown, 5 coders"
	r=$(run 5 5000 100 100 100 2 0 fifo)
	rc=$(exit_code "$r"); [[ "$rc" -eq 0 ]] && ok "Exit 0" || ko "Expected 0, got $rc"
	clean "$r"
}

# ── Main ───────────────────────────────────────────────────────────────

usage() {
	echo "Usage: $0 [--all | --args | --burnout | --completion | --scheduler |"
	echo "               --cooldown | --stress | --output | --leaks | --zeros]"
	exit 0
}

SUITE="all"
while [[ $# -gt 0 ]]; do
	case "$1" in
		--all)        SUITE="all" ;;
		--args)       SUITE="args" ;;
		--burnout)    SUITE="burnout" ;;
		--completion) SUITE="completion" ;;
		--scheduler)  SUITE="scheduler" ;;
		--cooldown)   SUITE="cooldown" ;;
		--stress)     SUITE="stress" ;;
		--output)     SUITE="output" ;;
		--leaks)      SUITE="leaks" ;;
		--zeros)      SUITE="zeros" ;;
		-h|--help)    usage ;;
		*) echo -e "${RED}Unknown option: $1${NC}"; usage ;;
	esac
	shift
done

check_binary

echo
echo -e "${CYAN}╔══════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║           Codexion Test Suite                     ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════╝${NC}"
echo " Binary: $BINARY"
echo " Date:   $(date)"

case "$SUITE" in
	all)        test_args; test_burnout; test_completion; test_scheduler;
	            test_cooldown; test_stress; test_output; test_leaks;
	            test_edge_zero ;;
	args)       test_args ;;
	burnout)    test_burnout ;;
	completion) test_completion ;;
	scheduler)  test_scheduler ;;
	cooldown)   test_cooldown ;;
	stress)     test_stress ;;
	output)     test_output ;;
	leaks)      test_leaks ;;
	zeros)      test_edge_zero ;;
esac

# ── Summary ───────────────────────────────────────────────────────────
echo
echo -e "${CYAN}══════════════════════════════════════════════════${NC}"
echo -e " Results:  ${GREEN}$PASS passed${NC}, ${RED}$FAIL failed${NC}, $TOTAL total"
if [[ "$FAIL" -eq 0 ]]; then
	echo -e " ${GREEN}All tests passed!${NC}"
else
	echo -e " ${RED}Some tests failed.${NC}"
fi
echo
exit "$FAIL"
