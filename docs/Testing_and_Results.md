# Testing, Results, and Engineering Challenges

## Project Overview

The Arduino Robotic Arm – Automatic Pickup project was developed to demonstrate the integration of mechanical design, embedded programming, servo motor control, and ultrasonic sensing.

The robotic arm consists of seven Miuzei MS18 servo motors, an Arduino Uno, an HC-SR04 ultrasonic sensor, and a custom 3D-printed PLA structure.

The project successfully demonstrated a sensor-triggered automatic pickup sequence in which the robotic arm detects a nearby hand, grips an object, lifts it, lowers it, and releases it.

This document describes the testing performed, the engineering challenges encountered, the solutions implemented, and potential future improvements.

---

## 1. Dual-Servo Shoulder Assembly

The robotic arm uses two Miuzei MS18 servos to operate the shoulder joint. This configuration was selected to distribute the mechanical load between two servos rather than relying on a single servo to lift the arm.

The original robotic arm design was modified in Autodesk Fusion 360 to accommodate the dual-servo configuration and shortened 60 mm arm sections.

### Implementation

The Arduino program controls both shoulder servos using mirrored angles:

```cpp
rightShoulderAngle = 180 - leftShoulderAngle;
```

This allows the servos to operate together while accounting for their opposite mounting orientations.

### Testing and Results

- Both shoulder servos responded to coordinated movement commands.
- The shoulder assembly successfully moved between the programmed parked and raised positions.
- The shoulder assembly supported the arm during the successful automatic pickup demonstration.

### Engineering Considerations

Mechanical alignment and servo synchronization are important to prevent the motors from working against each other.

Although the dual-servo configuration functioned successfully during testing, its maximum torque capacity has not been independently measured.

---

## 2. Base Rotation

The base rotation mechanism presented a mechanical challenge during development.

### Problem

Friction in the base assembly limited reliable rotation when using the installed servo motor.

### Solution

Rather than allowing the base rotation problem to prevent completion of the project, the automatic pickup routine was designed to maintain the base at a fixed position of 90 degrees.

The remaining joints perform the required movements for gripping, lifting, lowering, and releasing an object.

### Testing and Results

- The base servo responded to control commands during development.
- Mechanical friction limited dependable base movement.
- The automatic pickup sequence operated successfully with the base fixed at 90 degrees.

### Future Improvements

Reduce mechanical friction, improve bearing alignment, and evaluate whether a higher-torque servo would improve base rotation.

---

## 3. Ultrasonic Sensor Testing

An HC-SR04 ultrasonic sensor was mounted beneath the wrist rotation servo.

The sensor measures distance using ultrasonic pulses and provides input to the Arduino for automatic operation.

### Testing

Several objects were placed in front of the sensor to evaluate detection performance.

### Results

- Larger targets, including a hand or book, were detected successfully.
- Smaller objects that could fit inside the gripper were not detected consistently.
- The programmed detection range was approximately 5–20 cm.
- Three consecutive valid distance readings were used to reduce false triggering.

### Solution

Because the sensor could not reliably detect smaller objects, the automatic pickup routine uses a nearby hand to activate the sequence.

After detecting the hand, the Arduino executes a predefined series of movements to pick up an object placed at the calibrated location.

This approach allowed the project to demonstrate automatic operation without requiring the sensor to determine the object's exact position.

### Limitations

The system uses sensor-triggered automation rather than autonomous object localization. Objects must be positioned appropriately before the pickup sequence begins.

---

## 4. Arm Position Calibration

The robotic arm was calibrated using two predefined positions: a parked pickup position and a raised position.

### Calibrated Joint Positions

| Joint | Parked Position | Raised Position |
|---|---:|---:|
| Base | 90° | 90° |
| Left shoulder | 118° | 81° |
| Right shoulder | 62° | 99° |
| Elbow | 170° | 85° |
| Wrist tilt | 100° | 87° |
| Wrist rotation | 75° | 70° |

### Gripper Calibration

| Gripper State | Angle |
|---|---:|
| Open | 80° |
| Closed | 94° |

### Testing and Results

The parked position was adjusted so the gripper could surround an object placed at the pickup location.

The raised position was calibrated to allow the arm to lift the object while maintaining its grip.

Both positions were successfully used during the automatic pickup demonstration.

### Engineering Considerations

The system uses predefined servo angles rather than joint position feedback.

As a result, mechanical movement and positioning accuracy depend on servo performance, load, and the physical alignment of the arm.

---

## 5. Automatic Pickup Sequence

The primary objective of the project was to demonstrate an automatic object pickup and release operation using an Arduino-controlled robotic arm.

### Operating Sequence

1. The HC-SR04 ultrasonic sensor detects a hand within the programmed detection range.
2. The Arduino confirms the detection using consecutive distance measurements.
3. The program waits approximately two seconds for the hand to be removed.
4. The gripper opens and the arm moves into the parked pickup position.
5. The gripper closes around the object.
6. The shoulder, elbow, and wrist joints move the arm into the raised position.
7. The arm lowers the object back to the pickup position.
8. The gripper opens, releasing the object.
9. The arm returns to its raised position.

### Test Results

| Test | Result |
|---|---|
| Ultrasonic hand detection | Successful |
| Automatic sequence activation | Successful |
| Gripper opening and closing | Successful |
| Object pickup | Successful |
| Object lifting | Successful |
| Object lowering | Successful |
| Object release | Successful |
| Complete automatic sequence | Successful |

### Observations

The robotic arm successfully completed the full programmed pickup-and-release sequence.

The demonstration verified that the Arduino could coordinate the ultrasonic sensor and seven servo motors to complete a predefined object manipulation task.

The successful test also demonstrated that the dual-servo shoulder assembly could support the arm during the programmed movements.

### Limitations

Although the complete automatic sequence was successful, additional testing would be required to determine:

- Maximum payload capacity.
- Repeatability over multiple cycles.
- Positioning accuracy.
- Long-term servo reliability.
- Performance with objects of different sizes and weights.

---

## 6. Electrical System and Power Distribution

The robotic arm uses separate power sources for the Arduino and servo motors.

### Power Configuration

- **Arduino Uno:** Powered through USB from a laptop.
- **Seven servo motors:** Powered through an external USB power bank.
- **HC-SR04 ultrasonic sensor:** Powered through the Arduino's 5V and GND connections.
- **Common ground:** The Arduino and external servo power supply share a ground connection.

This arrangement prevents the servo motors from drawing their operating current directly through the Arduino's 5V supply.

### Arduino Pin Assignments

| Arduino Pin | Connected Component |
|---|---|
| D2 | Base servo |
| D3 | Left shoulder servo |
| D4 | Right shoulder servo |
| D5 | Elbow servo |
| D6 | HC-SR04 TRIG |
| D7 | HC-SR04 ECHO |
| D8 | Wrist tilt servo |
| D9 | Wrist rotation servo |
| D10 | Gripper servo |

### Documentation

The project repository includes electrical documentation developed using KiCad and Tinkercad.

The mechanical design and modified 3D-printable components are documented using Autodesk Fusion 360.

---

## 7. Engineering Challenges and Solutions

| Challenge | Solution | Outcome |
|---|---|---|
| Shoulder joint load | Implemented dual synchronized shoulder servos | Successful lifting during demonstration |
| Base rotation friction | Fixed base at 90° during automatic operation | Reliable operation without base rotation |
| Small-object detection | Used hand detection to trigger a predefined sequence | Successful automatic activation |
| Joint positioning | Calibrated parked and raised servo angles | Successful object manipulation |
| Servo power requirements | Used an external power bank with common ground | Functional seven-servo system |

These challenges required modifications to the mechanical design, electrical configuration, and Arduino software.

---

## 8. Future Improvements

Several improvements could increase the robotic arm's capabilities and reliability.

1. **Base Rotation:** Reduce mechanical friction and improve the base drive mechanism.
2. **Payload Capacity:** Evaluate higher-torque servos or alternative actuators.
3. **Object Detection:** Add a sensor capable of reliably detecting smaller objects.
4. **Position Feedback:** Implement feedback to improve joint positioning accuracy.
5. **Emergency Stop:** Add a software-controlled emergency-stop function and appropriate hardware safety provisions.
6. **Repeatability Testing:** Conduct repeated pickup cycles and record the success rate.
7. **Object Recognition:** Investigate computer vision for identifying and locating objects.
8. **Motion Planning:** Implement more flexible joint movement and object positioning.

---

## 9. Project Conclusion

The Arduino Robotic Arm – Automatic Pickup project successfully demonstrated a functional robotic system capable of automatically picking up, lifting, lowering, and releasing an object.

The project combined mechanical design, 3D printing, embedded programming, servo motor control, electrical wiring, and ultrasonic sensing.

Several engineering challenges were encountered during development, including shoulder joint loading, base rotation friction, and limitations in ultrasonic detection of smaller objects.

These challenges were addressed through mechanical modifications, calibration, and software changes.

The final demonstration confirmed that the robotic arm could successfully execute its intended automatic pickup sequence.

This project provided practical experience in integrating mechanical, electrical, and software systems into a working robotic platform.

### Project Resources

- [GitHub Repository](https://github.com/connorcqueen-lgtm/Arduino-Robotic-Arm-Automatic-Pickup)
- [Video Demonstration](https://youtu.be/fwJvGjtJHF4)
- [Original Robotic Arm Design by mobiobi](https://www.thingiverse.com/thing:4782837)

The original mechanical design is licensed under Creative Commons Attribution 4.0 (CC BY 4.0). Modifications were made for this project's hardware and operating requirements.