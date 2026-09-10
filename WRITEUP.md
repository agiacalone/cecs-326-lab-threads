---
# WHO THIS IS. Fill both in before you submit.
#
# This repository is PRIVATE — you and I are the only people who can read it.
# I need these two lines to put your grade in Canvas against the right person:
# GitHub knows you as a username, Canvas knows you as a student, and this is
# the only place those two meet. A blank or wrong ID means a grade that lands
# on nobody, and I have to come find you to fix it.
name: "YOUR NAME AS IT APPEARS IN CANVAS"
student_id: "YOUR 9-DIGIT CSULB ID"
---

# The Torn Map — writeup

Three questions, 15 points. A paragraph each is plenty. Code snippets are
welcome. Write them in your own words: I am grading whether you can explain the
mechanism, not whether you can restate the README.

---

## 1. The interleaving (7 points)

Two runners call `record_leaf()` at the same time. Walk through it step by step
and show how one increment gets lost.

*"I added a mutex and it worked" earns nothing here.* Show the sequence — who
reads what, who writes what, and in which order.

<!-- your answer -->


---

## 2. Why the map survived (5 points)

All seven runners write into the same `g_map` buffer with no lock at all, and
it comes out correct every time. All seven touch one `int` and it breaks
constantly.

Both are shared. Only one breaks. Explain the difference.

<!-- your answer -->


---

## 3. The timing (3 points)

Roughly how long did Phase 1 take, and Phase 2? Any measurement is fine — a
stopwatch, `time`, your phone.

You had seven runners and did not get seven times the speed. What is the
ceiling, and what sets it?

<!-- your answer -->


---

## Sources

If you used anything beyond the README and the OSTEP chapters — a manual page,
a blog post, a classmate you talked it through with, an AI assistant that
explained a concept — list it here. Citing is never penalised. Not citing is.

<!-- your sources -->
