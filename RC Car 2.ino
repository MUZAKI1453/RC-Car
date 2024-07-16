#include <AFMotor.h>
#include <Servo.h>

// Deklarasi Motor dan Servo
AF_DCMotor motor1(2); // Motor pada channel 2
AF_DCMotor motor2(3); // Motor pada channel 3
Servo myservo;

// Deklarasi Pin Sensor Ultrasonik
#define Echo1 A0 // sensor tengah
#define Trig1 A1
#define Echo2 A2 // sensor kiri
#define Trig2 A3
#define Echo3 A4 // sensor kanan
#define Trig3 A5

// Variabel untuk menyimpan nilai jarak
long distance1, distance2, distance3;

// Fungsi untuk menghitung jarak dari sensor ultrasonik
long readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) / 58.2; // Menghitung jarak dalam cm
}

void setup() {
  Serial.begin(9600);

  // Inisialisasi pin sensor ultrasonik
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);

  // Inisialisasi Servo
  myservo.attach(10);
  myservo.write(90); // Posisi awal lurus

  // Inisialisasi Motor
  motor1.setSpeed(255); // Kecepatan maksimum
  motor2.setSpeed(255); // Kecepatan maksimum
}

void loop() {
  // Membaca jarak dari masing-masing sensor ultrasonik
  distance1 = readUltrasonicDistance(Trig1, Echo1);
  distance2 = readUltrasonicDistance(Trig2, Echo2);
  distance3 = readUltrasonicDistance(Trig3, Echo3);

  // Menampilkan jarak pada serial monitor
  Serial.print("Tengah: "); Serial.print(distance1);
  Serial.print(" cm, Kiri: "); Serial.print(distance2);
  Serial.print(" cm, Kanan: "); Serial.print(distance3);
  Serial.println(" cm");

  // Logika pengambilan keputusan
  if (distance1 > 30) {
    // Jika jarak di depan lebih dari 30 cm, terus maju
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    myservo.write(90); // Lurus
  } else {
    // Jika jarak di depan kurang dari 30 cm, cek kiri dan kanan
    if (distance2 > distance3) {
      // Jika jarak di kiri lebih besar, belok kiri
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      myservo.write(40); // Belok kiri
    } else {
      // Jika jarak di kanan lebih besar, belok kanan
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      myservo.write(140); // Belok kanan
    }
  }

  delay(100);
}
