#include <Ethernet.h>
//#include <SPI.h>
#include <Servo.h>
#include <UbidotsEthernet.h>
#define ID_enable  "59500b8f7625422cac36ab32"  // Put here your Ubidots variable ID
#define ID_sensor  "59c84444c03f9726abb63ed5" 
#define TOKEN  "3cCXdtQuPWNol0DtxNeqKpLR30hbHK"  // Put here your Ubidots TOKEN
#include <Ultrasonic.h>

Ultrasonic ultrasonic(9,8); // (Trig PIN,Echo PIN)
Servo myservo; 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 200);

int distance;
int enabled;
int von=1;

Ubidots client(TOKEN);

void setup()
{
 // 
    Serial.begin(9600);
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);

    myservo.attach(7);
} 

int getData(char* ID)
{
  Serial.println("getting enable value");
  int value = client.getValue(ID_enable);
  return value;
}

void sendData()
{ 
// send data to cloud
  distance= (ultrasonic.Ranging(CM));
  client.add(ID_sensor, distance);
  client.sendAll();
}

void logData()
{
  Serial.println("Printing for debug");
  Serial.print(" / Distance: ");
  Serial.print(distance);
  Serial.print(" / enabled: ");
  Serial.print(enabled);
}

void loop() 
{
// get cloud data
enabled = getData(ID_enable);
// log sensor and cloud data
logData();
// send data to the cloud
sendData();

if(enabled==von)
    {
       myservo.write(170);
       delay(15);
    }
    else
    {
      myservo.write(0);
    }

delay(2000);


}
