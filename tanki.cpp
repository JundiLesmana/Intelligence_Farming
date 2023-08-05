#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;

#define RS485TxEnablePin 2
#define RS485Baud 9600
#define RS485Format SERIAL_8N1
//Sensor Ultrasonic 1
#define TrigPin1 3
#define EchoPin1 4
//Sensor Ultrasonic 2
#define TrigPin2 5
#define EchoPin2 6
//Sensor Ultrasonic 3
#define TrigPin3 7
#define EchoPin3 8
//Sensor Ultrasonic 4
#define TrigPin4 9
#define EchoPin4 10


int duration1;
int duration2;
int duration3;
int duration4;
int distance1;
int distance2;
int distance3;
int distance4;

void setup() {
  // put your setup code here, to run once:
  // ID 20 Ultrasonic WaterProf 1-4
  regBank.setId(20);

  //Add Analog Input
  regBank.add(30001);
  regBank.add(30002);
  regBank.add(30003);
  regBank.add(30004);

  slave._device = &regBank;  

  slave.setBaud(&Serial,RS485Baud,RS485Format,RS485TxEnablePin); 

  //Define Pin
  pinMode(TrigPin1, OUTPUT);
  pinMode(TrigPin2, OUTPUT);
  pinMode(TrigPin3, OUTPUT);
  pinMode(TrigPin4, OUTPUT);
  pinMode(EchoPin1, INPUT);
  pinMode(EchoPin2, INPUT);
  pinMode(EchoPin3, INPUT);
  pinMode(EchoPin4, INPUT);

  // Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Clear the trigPin by setting it LOW:
  digitalWrite(TrigPin1, LOW);
  digitalWrite(TrigPin2, LOW);
  digitalWrite(TrigPin3, LOW);
  digitalWrite(TrigPin4, LOW);
  
  delayMicroseconds(2);
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(TrigPin1, HIGH);
  digitalWrite(TrigPin2, HIGH);
  digitalWrite(TrigPin3, HIGH);
  digitalWrite(TrigPin4, HIGH);
  
  delayMicroseconds(20);
  digitalWrite(TrigPin1, LOW);
  digitalWrite(TrigPin2, LOW);
  digitalWrite(TrigPin3, LOW);
  digitalWrite(TrigPin4, LOW);
  
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration1 = pulseIn(EchoPin1, HIGH, 26000);
  duration2 = pulseIn(EchoPin2, HIGH, 26000);
  duration3 = pulseIn(EchoPin3, HIGH, 26000);
  duration4 = pulseIn(EchoPin4, HIGH, 26000);
  
  // Calculate the distance:
  distance1 = duration1/58.2;
  distance2 = duration2/58.2;
  distance3 = duration3/58.2;
  distance4 = duration4/58.2;
  

  //Modbus Mapping
  regBank.set(30001, distance1 * 100);
  regBank.set(30002, distance2 * 100);
  regBank.set(30003, distance3 * 100);
  regBank.set(30004, distance4 * 100);

  slave.run(); 

  //Print Distance on the Serial Monitor
  Serial.print("Distance1 = ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("Distance2 = ");
  Serial.print(distance2);
  Serial.println(" cm");
  Serial.print("Distance3 = ");
  Serial.print(distance3);
  Serial.println(" cm");
  Serial.print("Distance4 = ");
  Serial.print(distance4);
  Serial.println(" cm");

  delay(500);

}
