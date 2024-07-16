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

int pos = 90; // Posisi lurus servo

void setup() {
  Serial.begin(9600);
  myservo.attach(10);
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);
  myservo.write(pos); // Set servo to straight position
}

void loop() {
  long distanceCenter = getDistance(Trig1, Echo1);
  long distanceLeft = getDistance(Trig2, Echo2);
  long distanceRight = getDistance(Trig3, Echo3);

  Serial.print("Center: ");
  Serial.print(distanceCenter);
  Serial.print(" Left: ");
  Serial.print(distanceLeft);
  Serial.print(" Right: ");
  Serial.println(distanceRight);

  if (distanceCenter > 50) {
    // Jalan lurus dengan kecepatan maksimal
    motor1.setSpeed(255);
    motor2.setSpeed(255);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    myservo.write(90); // Set servo to straight position
  } else if (distanceCenter <= 50 && distanceCenter > 20) {
    // Kurangi kecepatan saat mendekati objek di depan
    motor1.setSpeed(150);
    motor2.setSpeed(150);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    myservo.write(90); // Set servo to straight position
  } else if (distanceCenter <= 20) {
    // Mundur jika terlalu dekat dengan objek di depan
    motor1.setSpeed(150);
    motor2.setSpeed(150);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    delay(1000); // Mundur selama 1 detik

    // Belok sesuai jarak sensor kiri dan kanan
    if (distanceLeft > distanceRight) {
      myservo.write(135); // Belok kiri
      motor1.setSpeed(200);
      motor2.setSpeed(200);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(1000); // Belok kiri selama 1 detik
    } else {
      myservo.write(45); // Belok kanan
      motor1.setSpeed(200);
      motor2.setSpeed(200);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      delay(1000); // Belok kanan selama 1 detik
    }
    myservo.write(90); // Kembali ke posisi lurus
  }

  // Penyesuaian tambahan untuk menghindari tabrakan
  if (distanceLeft < 20) {
    myservo.write(135); // Belok kiri
  } else if (distanceRight < 20) {
    myservo.write(45); // Belok kanan
  } else {
    myservo.write(90); // Posisi lurus
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
