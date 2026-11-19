# Arduino Robotic Arm – Automatic Pickup

## Project Overview

This project is a 3D-printed robotic arm controlled by an Arduino Uno. It uses seven servo motors, including two synchronized shoulder servos, to perform manual movements and an automated object pickup demonstration.

An HC-SR04 ultrasonic sensor detects a nearby hand to initiate a programmed pickup sequence. The arm then approaches a predetermined location, grips an object, lifts it, lowers it, and releases it.

The project demonstrates embedded programming, servo motor control, sensor integration, and robotic automation.

## Hardware

- Arduino Uno R3
- 7 × Miuzei MS18 9g servo motors
- HC-SR04 ultrasonic sensor
- External regulated 5V servo power supply
- 3D-printed PLA robotic arm
- Jumper wires and electrical connections

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| Base servo | D2 |
| Left shoulder servo | D3 |
| Right shoulder servo | D4 |
| Elbow servo | D5 |
| HC-SR04 TRIG | D6 |
| HC-SR04 ECHO | D7 |
| Wrist tilt servo | D8 |
| Wrist rotation servo | D9 |
| Gripper servo | D10 |

The shoulder servos rotate in opposite directions using complementary angles. The base remains at 90° during automatic pickup because of mechanical friction.

## Software Features

- Manual control of six joints through the Serial Monitor
- Seven independently connected servo motors
- Synchronized dual-shoulder movement
- Smooth movement between calibrated positions
- Ultrasonic hand detection
- Automatic gripping, lifting, lowering, and release
- Adjustable servo movement speeds

## Serial Monitor Commands

Set the Serial Monitor to **9600 baud** with **No line ending**.

| Command | Function |
|---|---|
| R | Raise arm to vertical |
| P | Move arm to parked position |
| A | Enable or disable automatic detection |
| D | Measure ultrasonic distance |
| 1–6 | Select joint |
| + / - | Adjust selected joint |
| p | Display current positions |
| H | Display commands |

## Automatic Pickup Demonstration

1. Place a lightweight object at the calibrated pickup location.
2. Raise the arm vertically.
3. Enable automatic detection using `A`.
4. Hold a hand approximately 5–20 cm in front of the ultrasonic sensor.
5. After three consecutive detections, the arm begins its programmed sequence.
6. The arm opens its gripper, moves to the pickup location, and closes the gripper.
7. The arm lifts the object, lowers it, and releases it.
8. The arm returns to its vertical position.

## Gripper Calibration

- Open: 80°
- Closed: 94°

## Limitations and Future Improvements

- The HC-SR04 is more reliable for detecting larger objects than small objects suitable for gripping.
- Automatic pickup currently uses a fixed, calibrated location rather than locating the object dynamically.
- Mechanical friction limits base rotation.
- Future improvements could include infrared sensing, improved object localization, and stronger joint actuators.

## Safety

The servo motors use an external regulated 5V power supply with a common ground shared with the Arduino. Avoid powering all seven servos directly from the Arduino's 5V pin.

The automatic movement sequence must be supervised, as the current program does not provide an emergency stop through the Serial Monitor while the arm is moving.

## Development Status

Functional prototype with manual control, ultrasonic detection, and a programmed automatic pickup sequence. Further testing and mechanical improvements are planned.

## Video Demonstration

Watch the robotic arm demonstration on YouTube:

[![Robotic Arm Demonstration](https://img.youtube.com/vi/fwJvGjtJHF4/0.jpg)](https://youtu.be/fwJvGjtJHF4)

[Watch on YouTube](https://youtu.be/fwJvGjtJHF4)
