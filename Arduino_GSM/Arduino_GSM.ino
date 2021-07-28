/*
 * SIM800L SMS RELAY v1.0
 * Arduino Hardware (Author): Nano V3 (ATmega328)
 * Arduino IDE (Author): 1.6.9
 * T.K.Hareendran/2018
 */
#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2);  // (Rx,Tx  > Tx,Rx)

char incomingByte;   //SMS Byte by byte
String inputString;  //SMS String

int relay3ph = 12; //3 Phase Relay for 6 Series in Auto

/****************************************************************/
/***      Initialization        *********************************/
/***      Runs only once at the time of start********************/
/****************************************************************/
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay3ph, OUTPUT);
  if (EEPROM.read(0) == 'H') {
    digitalWrite(relay3ph, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(relay3ph, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.begin(9600);
  mySerial.begin(9600);
  while(!mySerial.available())
    {
    mySerial.println("AT");
    delay(1000); 
    Serial.println("Connecting...");
    }
  Serial.println("Connected!");  
  mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
  delay(1000);  
  mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
  delay(1000);
  mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages
  delay(1000);
}

/****************************************************************/
/***      Main Loop             *********************************/
/***      Runs infinitely; never quits       ********************/
/****************************************************************/

void loop()
{
if(mySerial.available())
  {
  delay(100);
  // Serial Buffer
  while(mySerial.available()){
    incomingByte = mySerial.read();
    inputString += incomingByte; 
    }
  delay(10);      
  Serial.println("###Start");
  Serial.println(inputString);
  Serial.println("####End");
  inputString.toUpperCase(); // Uppercase the Received Message

  //turn RELAY ON or OFF
  if (inputString.indexOf("OFF1") > -1){
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(relay3ph, LOW);
    EEPROM.update(0, 'L');
    sendsms(0);
    }
  else if (inputString.indexOf("ON1") > -1){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(relay3ph, HIGH);
    EEPROM.update(0, 'H');
    sendsms(1);
    }
  delay(50);
  //Delete Messages & Save Memory
  //if (inputString.indexOf("OK") == -1){
  //mySerial.println("AT+CMGDA=\"DEL ALL\"");
  //delay(1000);
  }
  inputString = "";
}

/****************************************************************/
/***      Send SMS Message      *********************************/
/****************************************************************/

void sendsms(int x)
{
  mySerial.println("AT");
  delay(500);
  mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS=\"+919095981331\"\r");
  delay(500);
  if (x == 0){
    mySerial.print("OFF Success");
    }
  else if (x == 1){
    mySerial.print("ON Success");
    }
  delay(500);
  mySerial.write(26);
}
