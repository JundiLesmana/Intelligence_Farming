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
#define TrigPin 3
#define EchoPin 4

int duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  // ID 20 Ultrasonic WaterProf 1-4
  regBank.setId(22);

  //Add Analog Input
  regBank.add(30001);

  slave._device = &regBank;  

  slave.setBaud(&Serial,RS485Baud,RS485Format,RS485TxEnablePin); 

  //Define Pin
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  

  // Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Clear the trigPin by setting it LOW:
  digitalWrite(TrigPin, LOW);

  delayMicroseconds(2);
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(TrigPin, HIGH);
  
  delayMicroseconds(20);
  digitalWrite(TrigPin, LOW);

  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(EchoPin, HIGH, 26000);

  // Calculate the distance:
  distance = duration/58.2;

  //Modbus Mapping
  regBank.set(30001, distance * 100);

  slave.run(); 

  //Print Distance on the Serial Monitor
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);

}
