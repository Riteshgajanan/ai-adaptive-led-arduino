/*
  AI-Adaptive LED Controller  (Arduino Uno)
  ------------------------------------------------------------
  An LED on a PWM pin glows according to a tiny neural network
  that looks at ambient light (LDR) and occupancy (PIR).
  Dark + occupied -> bright LED. Bright room or empty room -> off.

  Wiring (see README):
    LDR divider  : 5V - LDR - A0 - 10k - GND
    PIR OUT      : D2
    LED (+220R)  : D9 (PWM)
*/
#include "inference.h"

const uint8_t  PIN_LDR = A0;
const uint8_t  PIN_PIR = 2;
const uint8_t  PIN_LED = 9;

const uint8_t  ADC_SAMPLES     = 8;        // averaging window (fix for issue #1)
const uint32_t PIR_WARMUP_MS   = 30000UL;  // PIR settling time (fix for issue #2)
const uint32_t OCCUPANCY_HOLD  = 10000UL;  // stay "occupied" 10 s after last motion
const float    ADC_MAX         = 1023.0f;  // same scaling as training (fix for issue #3)

uint32_t lastMotionMs = 0;
bool     everMoved    = false;

float readLightNormalised() {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < ADC_SAMPLES; i++) {
    sum += analogRead(PIN_LDR);
    delay(2);
  }
  return (sum / (float)ADC_SAMPLES) / ADC_MAX;   // 0..1
}

void setup() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("AI Adaptive LED - warming up PIR..."));
}

void loop() {
  uint32_t now = millis();

  // ignore the PIR while it is still calibrating
  if (now > PIR_WARMUP_MS && digitalRead(PIN_PIR) == HIGH) {
    lastMotionMs = now;
    everMoved = true;
  }
  float occupied = (everMoved && (now - lastMotionMs) < OCCUPANCY_HOLD) ? 1.0f : 0.0f;

  float light  = readLightNormalised();
  float out    = predictBrightness(light, occupied);
  uint8_t pwm  = (uint8_t)(out * 255.0f + 0.5f);
  analogWrite(PIN_LED, pwm);

  Serial.print(F("light=")); Serial.print(light, 2);
  Serial.print(F(" occ="));  Serial.print((int)occupied);
  Serial.print(F(" pwm="));  Serial.println(pwm);
  delay(100);
}
