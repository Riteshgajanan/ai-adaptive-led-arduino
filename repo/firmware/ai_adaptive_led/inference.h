// inference.h - tiny 2-4-1 MLP forward pass (tanh hidden, sigmoid output)
#pragma once
#include <math.h>
#include "model_weights.h"

// light : normalised ambient light, 0.0 (dark) .. 1.0 (bright)
// occ   : occupancy flag, 0.0 or 1.0
// returns LED brightness 0.0 .. 1.0
inline float predictBrightness(float light, float occ) {
  float z2 = pgm_read_float(&B2);
  for (uint8_t j = 0; j < N_HIDDEN; j++) {
    float z1 = pgm_read_float(&B1[j])
             + pgm_read_float(&W1[j][0]) * light
             + pgm_read_float(&W1[j][1]) * occ;
    z2 += pgm_read_float(&W2[j]) * tanhf(z1);
  }
  return 1.0f / (1.0f + expf(-z2));
}
