#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial mySerial(3, 2);
char incomingByte;
String inputString;

int relay = 12;
int address = 0;
void setup()
{
pinMode(relay, OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);
char device_status = EEPROM.read(address);
if (device_status == 'H') {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(relay, HIGH);
  }
  else {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(relay, LOW);
  }
Serial.begin(9600);
mySerial.begin(9600);
Serial.println("Starting..."); 
delay(1000);
mySerial.println("AT");
pollSms();
mySerial.println("AT+CMGF=1"); 
pollSms();
mySerial.println("AT+CNMI=1,2,0,0,0");
pollSms();
digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
pollSms();
}

void pollSms()
{
delay(500);
while (Serial.available()) 
{
mySerial.write(Serial.read());
}
while(mySerial.available()) 
{
incomingByte = mySerial.read();
inputString += incomingByte;
}
Serial.println(inputString);
inputString.toUpperCase();
if (inputString.indexOf("OFF") > -1){
digitalWrite(relay, LOW);
digitalWrite(LED_BUILTIN, LOW);
EEPROM.write(address, 'L');
mySerial.println("AT+CMGDA=\"DEL ALL\"");
sendsms1();
sendsms2();
}
if (inputString.indexOf("ON") > -1){
digitalWrite(relay, HIGH);
digitalWrite(LED_BUILTIN, HIGH);
EEPROM.write(address, 'H');
mySerial.println("AT+CMGDA=\"DEL ALL\"");
sendsms1();
sendsms2();
}
inputString = "";
}

void sendsms1()
{
char device_status = EEPROM.read(address);
mySerial.println("AT+CMGS=\"+919095981331\"\r");
delay(500);
if (device_status == 'H') {
  mySerial.print("ON ");
  }
  else {
  mySerial.print("OFF ");
  }
mySerial.print("Success ");
delay(500);
mySerial.write(26);
}

void sendsms2()
{
char device_status = EEPROM.read(address);
mySerial.println("AT+CMGS=\"+919790039301\"\r");
delay(500);
if (device_status == 'H') {
  mySerial.print("ON ");
  }
  else {
  mySerial.print("OFF ");
  }
mySerial.print("Success ");
delay(500);
mySerial.write(26);
}
