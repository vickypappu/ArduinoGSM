/*GSM Motor Control Code
 * 
 * Developed using Arduino 1.8.15 Windows 10 64 Bit
 * 
 * Board: Arduino UNO r3 SMD, 800L GSM Module
 * Programmer: AVRISP mkll
 */

/*Arduio Pinouts
 * 2,3 - Tx and Rx for 800L GSM
 * Digital 9 - Power ON 800L
 * Digital 11 - 2Phase Relay
 * Digital 13 - 3Phase Relay
 */

//#1 - Commented to add 2 Phase Relay Function
 
#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial mySerial(3, 2);
char incomingByte;
String inputString;
int address = 0; //Address for EEPROM to store previous state

int Power800L = 9;  //Power ON GSM Module 
int Relay2Ph = 11;  //2Phase Relay
int Relay3Ph = 13;  //3Phase Relay

void setup()
{
pinMode(Power800L, OUTPUT);  //Set Pin9 as 3PH Relay Output
pinMode(Relay2Ph, OUTPUT);  //Set Pin11 as 2PH Relay Output
pinMode(Relay3Ph, OUTPUT);  //Set Pin13 as 3PH Relay Output

if (EEPROM.read(address) == 'H') {
  digitalWrite(Relay3Ph, HIGH);
  //#1digitalWrite(Relay2Ph, HIGH);
  //#1delay(5000);
  //#1digitalWrite(Relay2Ph, LOW);  //Release ON Button after 5 Seconds
  }
  else {
  digitalWrite(Relay3Ph, LOW);
  }

digitalWrite(Power800L, HIGH); //Power ON GSM Module and Wait 5 Seconds

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
}

void loop()
{
pollSms();
}

void pollSms()
{
delay(500);

while (Serial.available()) {
  mySerial.write(Serial.read()); //Print Everything to Serial Monitor
  }

while(mySerial.available()) {
  incomingByte = mySerial.read();
  inputString += incomingByte;     //Build sms String from Character Byte
  }

//Serial.println(inputString);  //SMS
inputString.toUpperCase();      //Convert sms to Caps letter

if (inputString.indexOf("OFF1") > -1){
  digitalWrite(Relay3Ph, LOW);
  EEPROM.update(address, 'L');
  mySerial.println("AT+CMGDA=\"DEL ALL\"");
  sendsms1();
  sendsms2();
  }

else if (inputString.indexOf("ON1") > -1){
  EEPROM.update(address, 'H');
  mySerial.println("AT+CMGDA=\"DEL ALL\"");
  digitalWrite(Relay3Ph, HIGH);
  //#1digitalWrite(Relay2Ph, HIGH);
  //#1delay(5000);                          //Wait 5 Seconds for 2Ph Relay
  //#1digitalWrite(Relay2Ph, LOW);
  sendsms1();
  sendsms2();
  }

else {
  //Send Error sms
  }
  
inputString = ""; //Clears String for next sms
}  //Poll Function ends

void sendsms1()
{
  delay(1000);
  mySerial.println("AT+CMGS=\"+919095981331\"\r");
  delay(500);
  if (EEPROM.read(address) == 'H') {
    mySerial.print("O_N Success");
    }
    else {
    mySerial.print("O_F_F Success");
    }
  delay(500);
  mySerial.write(26);
  }

void sendsms2()
{
  delay(1000);
  mySerial.println("AT+CMGS=\"+919790039301\"\r");
  delay(500);
  if (EEPROM.read(address) == 'H') {
    mySerial.print("O_N Success");
    }
    else {
    mySerial.print("O_F_F Success");
    }
  delay(500);
  mySerial.write(26);
  }
