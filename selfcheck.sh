#!/usr/bin/env bash
#
# selfcheck.sh: your own check, not the grader.
#
# Passing this does not guarantee full marks and failing it does not mean
# zero. It runs the same four things the grader runs, so if this is green you
# have probably not left anything obvious broken.
#
#   ./selfcheck.sh          10 runs
#   ./selfcheck.sh 50       50 runs, which is what the grader uses

set -u
RUNS="${1:-10}"
EXPECTED_LEAVES=140
PASS=0; FAIL=0
say() { printf '%s\n' "$*"; }
ok()   { PASS=$((PASS+1)); say "  PASS  $1"; }
bad()  { FAIL=$((FAIL+1)); say "  FAIL  $1"; }

if [ ! -x ./fetch ]; then
    say "No ./fetch. Run 'make' first."
    exit 1
fi

say "Running ./fetch $RUNS times."
say ""

worst_conc=99; checksum_bad=0; leaves_bad=0
for _ in $(seq "$RUNS"); do
    out=$(./fetch out.map 2>/dev/null) || { bad "fetch exited non-zero"; break; }
    leaves=$(printf '%s' "$out" | awk '/^leaves recovered/ {print $4}')
    conc=$(printf '%s'   "$out" | awk '/^max concurrency/  {print $4}')
    sums=$(printf '%s'   "$out" | awk '/^checksum/         {print $4}')
    [ "${leaves:-0}" = "$EXPECTED_LEAVES" ] || leaves_bad=$((leaves_bad+1))
    [ "${sums:-MISMATCH}" = "OK" ]          || checksum_bad=$((checksum_bad+1))
    [ "${conc:-0}" -lt "$worst_conc" ] && worst_conc=${conc:-0}
done

if [ "$checksum_bad" -eq 0 ]; then
    ok "the assembled map is correct in all $RUNS runs"
else
    bad "the map was wrong in $checksum_bad of $RUNS runs"
fi

if [ "$worst_conc" -ge 7 ]; then
    ok "all 7 runners overlapped (worst run: $worst_conc)"
elif [ "$worst_conc" -le 1 ]; then
    bad "no overlap at all (worst run: $worst_conc) -- are you joining each thread right after creating it?"
else
    bad "only $worst_conc runners overlapped; 7 are expected"
fi

if [ "$leaves_bad" -eq 0 ]; then
    ok "leaves recovered == $EXPECTED_LEAVES in all $RUNS runs"
else
    bad "the leaf count was wrong in $leaves_bad of $RUNS runs -- this is the race"
fi

say ""
say "$PASS passed, $FAIL failed."
[ "$FAIL" -eq 0 ]
