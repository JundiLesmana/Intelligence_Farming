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

int duration1;
int duration2;
int distance1;
int distance2;

void setup() {
  // put your setup code here, to run once:
  // ID 20 Ultrasonic WaterProf 1-4
  regBank.setId(21);

  //Add Analog Input
  regBank.add(30001);
  regBank.add(30002);

  slave._device = &regBank;  

  slave.setBaud(&Serial,RS485Baud,RS485Format,RS485TxEnablePin); 

  //Define Pin
  pinMode(TrigPin1, OUTPUT);
  pinMode(TrigPin2, OUTPUT);
  pinMode(EchoPin1, INPUT);
  pinMode(EchoPin2, INPUT);

  // Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Clear the trigPin by setting it LOW:
  digitalWrite(TrigPin1, LOW);
  digitalWrite(TrigPin2, LOW);

  delayMicroseconds(2);
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(TrigPin1, HIGH);
  digitalWrite(TrigPin2, HIGH);
  
  delayMicroseconds(20);
  digitalWrite(TrigPin1, LOW);
  digitalWrite(TrigPin2, LOW);

  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration1 = pulseIn(EchoPin1, HIGH, 26000);
  duration2 = pulseIn(EchoPin2, HIGH, 26000);

  // Calculate the distance:
  distance1 = duration1/58.2;
  distance2 = duration2/58.2;

  //Modbus Mapping
  regBank.set(30001, distance1 * 100);
  regBank.set(30002, distance2 * 100);

  slave.run(); 

  //Print Distance on the Serial Monitor
  Serial.print("Distance1 = ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("Distance2 = ");
  Serial.print(distance2);
  Serial.println(" cm");

  delay(500);

}
