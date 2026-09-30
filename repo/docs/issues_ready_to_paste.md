# QA issues - ready to paste into GitHub Issues

Create these labels first: bug, hardware, ml, documentation, enhancement, sev-high, sev-medium, sev-low.
Create a milestone "v1.0 - working prototype". Open each issue, add labels + milestone, then post the comments.

---
## Issue #1 - LED flickers when the room light is steady
Labels: bug, hardware, sev-medium
**Steps:** upload sketch v0.1, keep the LDR under constant light, watch the LED and Serial Monitor.
**Expected:** stable brightness. **Actual:** `light` jumps between 0.41 and 0.47 and the LED visibly flickers.
**Root cause (5-Why):** flicker <- output changes each loop <- input changes each loop <- single raw analogRead() per loop <- no filtering, ADC + LDR divider noise not considered.
**Fix plan:** average 8 ADC samples. Branch `fix/issue-1-ldr-averaging`.

**Comment (reviewer):** "Reproduced. Serial log attached, spread is about 0.06 with the lamp on. A moving average should be enough; no need for a capacitor yet."
**Comment (author):** "Fixed in PR #6 - 8-sample average, spread now under 0.01. Closing."

---
## Issue #2 - LED switches on by itself for the first half minute after power-up
Labels: bug, hardware, sev-high
**Root cause (5-Why):** LED lights in an empty room <- occupancy=1 <- PIR pin HIGH <- PIR is still calibrating after power-on <- datasheet warm-up time (30-60 s) was ignored in the code.
**Fix plan:** ignore PIR for the first 30 s. Branch `fix/issue-2-pir-warmup`.

**Comment (reviewer):** "Confirmed on my board too. Suggest a constant, not a magic number, so it can be tuned."
**Comment (author):** "Added PIR_WARMUP_MS. Fixed in PR #7."

---
## Issue #3 - Model gives almost-full brightness in a bright room (train/deploy mismatch)
Labels: bug, ml, sev-high
**Observed:** in a bright room with motion the LED stays on. Python test gives ~0.05, board gives ~0.8.
**Root cause (5-Why):** wrong output on board <- input to the network is different <- firmware fed raw ADC 0..1023 <- model was trained on 0..1 <- scaling step was never written into the firmware.
**Fix plan:** divide by 1023 before inference; add tests/host_test.cpp to compare C++ vs Python. Branch `fix/issue-3-input-scaling`.

**Comment (reviewer):** "Can we add an automated check so this cannot come back?"
**Comment (author):** "Good point - added tests/host_test.cpp, 10/10 reference points match to 1e-4. Fixed in PR #8."

---
## Issue #4 - Weights consume SRAM (Uno has only 2 KB)
Labels: enhancement, ml, sev-low
**Cause:** const float arrays are copied to SRAM at start-up. **Fix:** store in flash with PROGMEM and read with pgm_read_float(). Branch `fix/issue-4-progmem`.
**Comment (reviewer):** "Fine for 21 floats, but it will matter when the network grows. Keep it."

---
## Issue #5 - README has no wiring table or resistor values
Labels: documentation, sev-low
**Cause:** documentation was written after the code and the wiring stayed only in my notebook. **Fix:** add the hardware table (LDR divider 10k, LED 220 ohm, pin map). Branch `docs/issue-5-wiring`.
**Comment (reviewer):** "Followed the new table and it worked first time. Thanks."

---
## Pull requests to open (each with "Closes #N" in the description)
PR #6 -> #1, PR #7 -> #2, PR #8 -> #3, PR #9 -> #4, PR #10 -> #5. Ask your reviewer to approve before you merge.
