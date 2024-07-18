#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor1(2); // Motor pada channel 2
AF_DCMotor motor2(3); // Motor pada channel 3
Servo myservo;

#define Echo1 A0 // sensor tengah
#define Trig1 A1
#define Echo2 A2 // sensor kiri
#define Trig2 A3
#define Echo3 A4 // sensor kanan
#define Trig3 A5

long distanceCenter, distanceLeft, distanceRight;

void setup() {
  Serial.begin(9600);
  myservo.attach(10);
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);
  myservo.write(90); // Posisi awal lurus

  motor1.setSpeed(255); // Kecepatan maksimum
  motor2.setSpeed(255); // Kecepatan maksimum
}

void loop() {
  distanceCenter = getDistance(Trig1, Echo1);
  distanceLeft = getDistance(Trig2, Echo2);
  distanceRight = getDistance(Trig3, Echo3);

  Serial.print("Center: ");
  Serial.print(distanceCenter);
  Serial.print(" cm, Left: ");
  Serial.print(distanceLeft);
  Serial.print(" cm, Right: ");
  Serial.println(distanceRight);

  // Keputusan berdasarkan jarak dari sensor tengah
  if (distanceCenter > 100) {
    // Jika jalan di depan cukup jauh, kecepatan maksimum
    moveForward(255);
    myservo.write(90); // Posisi lurus
  } else if (distanceCenter > 50) {
    // Jika jalan di depan agak dekat, kurangi kecepatan
    moveForward(200);
    myservo.write(90); // Posisi lurus
  } else if (distanceCenter > 20) {
    // Jika jalan di depan dekat, kurangi kecepatan lebih jauh
    moveForward(150);
    myservo.write(90); // Posisi lurus
  } else {
    // Jika sangat dekat dengan objek di depan, mundur dan belok
    moveBackward(150);
    delay(500); // Mundur selama 0.5 detik

    if (distanceLeft > distanceRight) {
      turnLeft(200);
      delay(500); // Belok kiri selama 0.5 detik
    } else {
      turnRight(200);
      delay(500); // Belok kanan selama 0.5 detik
    }
  }

  // Penyesuaian dinamis berdasarkan sensor kiri dan kanan
  if (distanceLeft < 20) {
    // Belok kanan lebih tajam jika terlalu dekat dengan dinding kiri
    turnRight(150);
    delay(100); // Belok selama 0.1 detik
  } else if (distanceRight < 20) {
    // Belok kiri lebih tajam jika terlalu dekat dengan dinding kanan
    turnLeft(150);
    delay(100); // Belok selama 0.1 detik
  } else if (distanceLeft < 50) {
    // Belok kanan jika agak dekat dengan dinding kiri
    myservo.write(70);
  } else if (distanceRight < 50) {
    // Belok kiri jika agak dekat dengan dinding kanan
    myservo.write(110);
  } else {
    // Tetap lurus jika tidak ada objek yang terlalu dekat
    myservo.write(90);
  }

  delay(100);
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;
  return distance;
}

void moveForward(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void moveBackward(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void turnLeft(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  myservo.write(135); // Belok kiri
}

void turnRight(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  myservo.write(45); // Belok kanan
}
