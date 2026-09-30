# AI-Adaptive LED Controller (Arduino Uno)

A small embedded-AI project for the *Project Management* course (2307476T, E&TC, MIT AOE).
A tiny neural network (2 inputs, 4 hidden neurons, 1 output) decides how bright an LED
should be, based on ambient light (LDR) and room occupancy (PIR sensor).

| Situation            | LED behaviour        |
|----------------------|----------------------|
| Dark room, occupied  | bright               |
| Dim room, occupied   | medium               |
| Bright room          | off                  |
| Empty room           | off                  |

## Hardware
| Part            | Pin / value                       |
|-----------------|-----------------------------------|
| Arduino Uno     | -                                 |
| LDR + 10 kΩ     | 5V - LDR - **A0** - 10 kΩ - GND   |
| PIR (HC-SR501)  | OUT -> **D2**, VCC 5V, GND        |
| LED + 220 Ω     | anode -> **D9** (PWM) via 220 Ω   |

## Repository layout
```
firmware/ai_adaptive_led/   Arduino sketch, inference.h, model_weights.h
training/train_model.py     trains the network and exports the weights
tests/host_test.cpp         checks the C++ inference against Python
docs/                       QA log, planning notes
```

## How to run
1. `cd training && python train_model.py` (only if you want to retrain)
2. Open `firmware/ai_adaptive_led/ai_adaptive_led.ino` in Arduino IDE, select *Arduino Uno*, upload.
3. Open Serial Monitor at 9600 baud to watch `light`, `occ` and `pwm`.
4. Host test: `g++ -Itests/stub tests/host_test.cpp -o host_test && ./host_test`

## Reporting problems
Please use **Issues** with the labels `bug`, `hardware`, `ml`, `documentation`
and add the severity label `sev-high` / `sev-medium` / `sev-low`.
