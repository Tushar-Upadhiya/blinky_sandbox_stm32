# ARM (Cortex-M4 / Thumb) Assembly Practice

A progression from pure register basics up to the exact patterns you'll need
for CellOS's context-switching code. Do these using inline assembly inside a
small C file, compiled with `arm-none-eabi-gcc`, and check results either by
printing a register's value over UART, or by watching registers in a debugger
(GDB/OpenOCD, breakpoint after each block, `info registers`) — the debugger
route is actually the faster feedback loop while practicing.

Inline asm skeleton to build each answer into:
```c
int result;
asm volatile (
    "MOVS r0, #5   \n\t"
    "MOVS r1, #3   \n\t"
    "ADDS r0, r0, r1 \n\t"
    "MOV %0, r0    \n\t"
    : "=r" (result)
    :
    : "r0", "r1"
);
```

---

## Tier 1 — Registers & basic arithmetic

1. Load `7` into r0 and `12` into r1. Add them, store the result in r2.
2. Load `20` into r0. Subtract `8` from it using `SUBS`. What does `SUBS`
   (vs `SUB`) set as a side effect, and why does that matter?
3. Multiply two small numbers using `MULS`. Predict the result before running it.
4. Copy the value of r0 into r3 without touching memory at all (no LDR/STR).
5. Load a *negative* number into a register. What instruction did you need,
   and why doesn't `MOVS r0, #-5` work directly? (Hint: look at `MVN`.)

## Tier 2 — Memory access (LDR/STR)

6. Declare `volatile int x = 42;` in C. Write inline assembly that reads `x`
   into r0, adds `1`, and writes it back to `x`. Confirm in C that `x` is now 43.
7. Declare an array `int arr[5] = {1,2,3,4,5};`. Load `arr[2]` into a register
   using `LDR r0, [r1, #8]` — work out for yourself why the offset is `#8` and
   not `#2`.
8. Same array — write a value into `arr[4]` using `STR`.
9. Load a **byte** (not a word) using `LDRB` instead of `LDR`. What's
   different about the destination register's upper bits afterward?

## Tier 3 — Control flow (CMP, branches, loops)

10. Write a loop that counts from 0 to 9 in a register, using `CMP` and `BNE`
    (this is almost exactly your `.bss` zeroing loop's skeleton — reuse it).
11. Sum the values of a 5-element array into one register, using a loop
    (this is almost exactly your `.data` copy loop, but accumulating instead
    of copying).
12. Write a loop that finds the **largest** value in an array. You'll need a
    second comparison inside the loop body, not just the loop-exit check.
13. Reverse an array in place — read from both ends, swap, move inward,
    stop when the two pointers cross. (This introduces two moving pointers
    approaching each other, instead of one pointer walking to a fixed end.)
14. Implement a countdown using `SUBS` + `BNE` instead of `ADDS` + `CMP`.
    Why can this version skip the separate `CMP` entirely? (Look at what
    `SUBS` sets automatically.)

## Tier 4 — Functions, stack, and calling convention

15. Write a `BL` call to a function, and inside that function `PUSH {r4-r7}`
    at the start, `POP {r4-r7}` before returning. Why do only *some*
    registers need saving here, not all of r0-r12?
16. Write a recursive-style function call by hand: function A calls function
    B using `BL`, B does something, then `BX LR` returns to A. Confirm A's
    execution actually continues correctly after the call.
17. Deliberately forget to balance a `PUSH`/`POP` pair (push 2 registers, pop
    only 1) and observe what breaks. This is the exact class of bug that
    causes silent stack corruption in real firmware — worth seeing once,
    safely, on purpose.
18. Pass a value into a function via r0 (per ARM calling convention), and
    return a value via r0. Verify from the C side that this matches how C
    itself uses r0 for arguments/return values.

## Tier 5 — Bit manipulation (directly relevant to register-level embedded work)

19. Set a single specific bit in a register using `ORR` (e.g. set bit 5 of r0
    without disturbing other bits).
20. Clear a single specific bit using `BIC` (bit clear) — note this is
    different from `AND` with a plain mask; work out why `BIC` exists as its
    own instruction.
21. Toggle a bit using `EOR`.
22. Shift a value left by 3 using `LSL`, then right by 3 using `LSR`. Confirm
    you get back the original value — then try it with a value that loses
    bits during the left shift, and see that you *don't* get the original
    value back. Understand why.
23. Extract a 4-bit field from the middle of a register (e.g. bits 8-11)
    using a shift + mask combination. This is exactly the pattern you'll use
    reading fields out of STM32 peripheral registers later.

## Tier 6 — RTOS-relevant patterns (the actual payoff tier)

24. Push multiple registers at once using `STMDB` (Store Multiple, Decrement
    Before) instead of individual `PUSH` instructions — this is literally
    what `PendSV_Handler` will do to save a task's context.
25. Pop multiple registers back using `LDMIA` (Load Multiple, Increment
    After) and confirm the values come back in the same order they went in.
26. Simulate a tiny "context switch": write two register sets to two
    different memory locations (pretend they're two tasks' saved stacks),
    then write code that loads *the other* one back. This is PendSV's core
    job, done manually and without any interrupt involved yet.
27. Read the current value of SP into a general-purpose register (`MOV r0,
    SP`), and separately write a new value into SP (`MOV SP, r0`). This is
    the exact mechanic behind switching between MSP and PSP later.
28. Write a small function that deliberately pushes enough data to run past
    a fixed, small buffer you set aside — trigger a real stack overflow into
    adjacent memory, and see what breaks. Understand *why* this is exactly
    the failure mode MPU-based stack protection (MemManage) exists to catch.

---

## How to pace this

Tiers 1-2 are pure syntax fluency — do these in one or two short sessions,
they should start feeling automatic quickly. Tiers 3-4 are where real
understanding builds — don't rush these, trace through registers by hand
before running anything, then confirm your prediction was right. Tier 5 is
useful any time, doesn't need to be done in order. **Save Tier 6 for right
before you actually start writing `PendSV_Handler`** — doing it as a warm-up
immediately beforehand will make the real thing feel like a natural next
step instead of a jump.
