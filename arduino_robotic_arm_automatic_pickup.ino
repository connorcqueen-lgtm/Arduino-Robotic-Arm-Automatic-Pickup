
#include <Servo.h>

// ==========================================
// ROBOTIC ARM - AUTOMATIC PICKUP
// Arduino Uno - 7 Servos + HC-SR04
// ==========================================

// Servo objects
Servo base;
Servo shoulderLeft;
Servo shoulderRight;
Servo elbow;
Servo wristTilt;
Servo wristRotate;
Servo gripper;

// Servo pins
const int basePin = 2;
const int shoulderLeftPin = 3;
const int shoulderRightPin = 4;
const int elbowPin = 5;
const int wristTiltPin = 8;
const int wristRotatePin = 9;
const int gripperPin = 10;

// HC-SR04 pins
const int trigPin = 6;
const int echoPin = 7;

// Six controllable joints
const int JOINTS = 6;

// Joint index:
// 0 = Base
// 1 = Shoulder (two synchronized servos)
// 2 = Elbow
// 3 = Wrist Tilt
// 4 = Wrist Rotation
// 5 = Gripper

// Calibrated parked/pickup position
const int parkPose[JOINTS] = {
  90, 118, 170, 100, 75, 90
};

// Calibrated vertical/lift position
const int verticalPose[JOINTS] = {
  90, 81, 85, 87, 70, 96
};

// Joint limits
const int minAngle[JOINTS] = {
  70, 75, 80, 75, 60, 80
};

const int maxAngle[JOINTS] = {
  110, 125, 175, 110, 100, 100
};

// Current positions
int angles[JOINTS] = {
  90, 118, 170, 100, 75, 90
};

// Gripper calibration
const int gripperOpen = 80;
const int gripperClosed = 94;

// Movement speed
const int startupDelay = 100;
const int normalDelay = 30;
const int autoMoveDelay = 40;

// Detection settings
const float detectionMin = 5.0;
const float detectionMax = 20.0;

bool autoDetection = false;

unsigned long lastScan = 0;
int detectionCount = 0;

int selectedJoint = 0;

const char* names[JOINTS] = {
  "Base",
  "Shoulder",
  "Elbow",
  "Wrist Tilt",
  "Wrist Rotation",
  "Gripper"
};

// ==========================================
// WRITE JOINT POSITION
// ==========================================

void writeJoint(int joint, int angle) {

  switch (joint) {

    case 0:
      base.write(angle);
      break;

    case 1:
      shoulderLeft.write(angle);
      shoulderRight.write(180 - angle);
      break;

    case 2:
      elbow.write(angle);
      break;

    case 3:
      wristTilt.write(angle);
      break;

    case 4:
      wristRotate.write(angle);
      break;

    case 5:
      gripper.write(angle);
      break;
  }
}

// ==========================================
// MOVE ONE JOINT
// ==========================================

void moveJoint(int joint, int target, int stepDelay) {

  target = constrain(
    target,
    minAngle[joint],
    maxAngle[joint]
  );

  while (angles[joint] != target) {

    if (angles[joint] < target) {
      angles[joint]++;
    }
    else {
      angles[joint]--;
    }

    writeJoint(joint, angles[joint]);
    delay(stepDelay);
  }

  Serial.print(names[joint]);
  Serial.print(": ");
  Serial.println(angles[joint]);
}

// ==========================================
// MOVE ALL JOINTS TO A POSE
// ==========================================

void moveToPose(const int target[JOINTS],
                int stepDelay) {

  int safeTarget[JOINTS];

  for (int i = 0; i < JOINTS; i++) {

    safeTarget[i] = constrain(
      target[i],
      minAngle[i],
      maxAngle[i]
    );
  }

  bool moving = true;

  while (moving) {

    moving = false;

    for (int i = 0; i < JOINTS; i++) {

      if (angles[i] < safeTarget[i]) {
        angles[i]++;
        moving = true;
      }
      else if (angles[i] > safeTarget[i]) {
        angles[i]--;
        moving = true;
      }

      writeJoint(i, angles[i]);
    }

    delay(stepDelay);
  }

  Serial.println("Movement complete.");
}

// ==========================================
// MOVE ARM WHILE KEEPING GRIPPER FIXED
// ==========================================

void moveArmToPose(const int target[JOINTS],
                   int stepDelay) {

  int newPose[JOINTS];

  for (int i = 0; i < JOINTS; i++) {
    newPose[i] = target[i];
  }

  // Do not rotate the base
  newPose[0] = 90;

  // Preserve current gripper position
  newPose[5] = angles[5];

  moveToPose(newPose, stepDelay);
}

// ==========================================
// MEASURE ULTRASONIC DISTANCE
// ==========================================

float measureDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration =
    pulseIn(echoPin, HIGH, 30000UL);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

// ==========================================
// AUTOMATIC PICKUP DEMONSTRATION
// ==========================================

void automaticPickup() {

  Serial.println();
  Serial.println("========================");
  Serial.println("AUTOMATIC PICKUP STARTED");
  Serial.println("========================");

  // Allow time to remove hand
  Serial.println("Remove your hand!");
  delay(2000);

  // Step 1: Open gripper
  Serial.println("Opening gripper...");

  moveJoint(
    5,
    gripperOpen,
    normalDelay
  );

  delay(500);

  // Step 2: Lower arm to pickup position
  Serial.println("Moving to pickup position...");

  moveArmToPose(
    parkPose,
    autoMoveDelay
  );

  delay(700);

  // Step 3: Close gripper
  Serial.println("Gripping object...");

  moveJoint(
    5,
    gripperClosed,
    normalDelay
  );

  delay(1000);

  // Step 4: Lift object
  Serial.println("Lifting object...");

  moveArmToPose(
    verticalPose,
    autoMoveDelay
  );

  delay(1500);

  // Step 5: Lower object
  Serial.println("Lowering object...");

  moveArmToPose(
    parkPose,
    autoMoveDelay
  );

  delay(700);

  // Step 6: Release object
  Serial.println("Releasing object...");

  moveJoint(
    5,
    gripperOpen,
    normalDelay
  );

  delay(1000);

  // Step 7: Return to vertical
  Serial.println("Returning to vertical...");

  moveArmToPose(
    verticalPose,
    autoMoveDelay
  );

  Serial.println();
  Serial.println("AUTOMATIC PICKUP COMPLETE");
  Serial.println("========================");
}

// ==========================================
// PRINT POSITIONS
// ==========================================

void printPositions() {

  Serial.println();
  Serial.println("CURRENT POSITIONS:");

  for (int i = 0; i < JOINTS; i++) {

    Serial.print(names[i]);
    Serial.print(": ");
    Serial.println(angles[i]);
  }

  Serial.print("Shoulder Right: ");
  Serial.println(180 - angles[1]);
}

// ==========================================
// PRINT COMMANDS
// ==========================================

void printHelp() {

  Serial.println();
  Serial.println("ROBOTIC ARM COMMANDS");
  Serial.println("----------------------");

  Serial.println("R = Raise to vertical");
  Serial.println("P = Park arm");
  Serial.println("1-6 = Select joint");
  Serial.println("+ = Increase angle");
  Serial.println("- = Decrease angle");
  Serial.println("p = Print positions");
  Serial.println("D = Measure distance");
  Serial.println("A = Toggle automatic detection");
  Serial.println("H = Show commands");
}

// ==========================================
// SETUP
// ==========================================

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, LOW);

  // Initialize parked position
  for (int i = 0; i < JOINTS; i++) {

    angles[i] = parkPose[i];
    writeJoint(i, angles[i]);
  }

  // Attach servos
  base.attach(basePin);
  shoulderLeft.attach(shoulderLeftPin);
  shoulderRight.attach(shoulderRightPin);
  elbow.attach(elbowPin);
  wristTilt.attach(wristTiltPin);
  wristRotate.attach(wristRotatePin);
  gripper.attach(gripperPin);

  Serial.println();
  Serial.println("ROBOTIC ARM INITIALIZED");
  Serial.println("Expected position: PARKED");

  printHelp();
}

// ==========================================
// MAIN LOOP
// ==========================================

void loop() {

  // ----------------------------------------
  // SERIAL COMMANDS
  // ----------------------------------------

  if (Serial.available() > 0) {

    char command = Serial.read();

    if (command >= '1' && command <= '6') {

      selectedJoint = command - '1';

      Serial.print("Selected: ");
      Serial.println(names[selectedJoint]);
    }

    else if (command == '+') {

      moveJoint(
        selectedJoint,
        angles[selectedJoint] + 1,
        normalDelay
      );
    }

    else if (command == '-') {

      moveJoint(
        selectedJoint,
        angles[selectedJoint] - 1,
        normalDelay
      );
    }

    else if (command == 'R') {

      autoDetection = false;
      detectionCount = 0;

      Serial.println("Raising arm...");

      moveToPose(
        verticalPose,
        startupDelay
      );

      printPositions();
    }

    else if (command == 'P') {

      autoDetection = false;
      detectionCount = 0;

      Serial.println("Parking arm...");

      moveToPose(
        parkPose,
        startupDelay
      );

      printPositions();
    }

    else if (command == 'p') {

      printPositions();
    }

    else if (command == 'D') {

      float distance = measureDistance();

      if (distance < 0) {

        Serial.println("No echo detected");
      }
      else {

        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
      }
    }

    else if (command == 'A') {

      autoDetection = !autoDetection;
      detectionCount = 0;
      lastScan = millis();

      if (autoDetection) {

        Serial.println(
          "Automatic detection ENABLED"
        );
      }
      else {

        Serial.println(
          "Automatic detection DISABLED"
        );
      }
    }

    else if (command == 'H') {

      printHelp();
    }
  }

  // ----------------------------------------
  // AUTOMATIC DETECTION
  // ----------------------------------------

  if (autoDetection &&
      millis() - lastScan >= 150) {

    lastScan = millis();

    float distance = measureDistance();

    if (distance >= detectionMin &&
        distance <= detectionMax) {

      detectionCount++;

      if (detectionCount >= 3) {

        Serial.println();
        Serial.println("HAND DETECTED!");

        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        // Disable detection during movement
        autoDetection = false;
        detectionCount = 0;

        // Run automatic pickup
        automaticPickup();
      }
    }
    else {

      detectionCount = 0;
    }
  }
}
