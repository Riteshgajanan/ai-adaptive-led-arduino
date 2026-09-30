// Compiles the Arduino inference code on a PC and checks it against the
// Python reference outputs (tests/reference.csv).   Build:  see README
#include <cstdio>
#include <cmath>
#include "../firmware/ai_adaptive_led/inference.h"
int main() {
  FILE* f = fopen("tests/reference.csv", "r");
  if (!f) { puts("reference.csv missing"); return 2; }
  double l, o, ref; int fails = 0, n = 0;
  while (fscanf(f, "%lf,%lf,%lf", &l, &o, &ref) == 3) {
    float got = predictBrightness((float)l, (float)o);
    bool ok = fabs(got - ref) < 1e-4;
    printf("light=%.2f occ=%.0f  python=%.4f  c++=%.4f  %s\n", l, o, ref, got, ok ? "PASS" : "FAIL");
    fails += !ok; n++;
  }
  printf("%d/%d passed\n", n - fails, n);
  return fails ? 1 : 0;
}
