/*
 * SIM800L SMS RELAY v1.0
 * Arduino Hardware (Author): Nano V3 (ATmega328)
 * Arduino IDE (Author): 1.6.9
 * T.K.Hareendran/2018
 */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2);  // (Rx,Tx  > Tx,Rx)

char incomingByte;
String inputString;

int relay3ph = 12;


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay3ph, OUTPUT);
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
}

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
    }
  if (inputString.indexOf("ON1") > -1){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(relay3ph, HIGH);
    }          
  delay(50);
  //Delete Messages & Save Memory
  //if (inputString.indexOf("OK") == -1){
  //mySerial.println("AT+CMGDA=\"DEL ALL\"");
  //delay(1000);
  }
  inputString = "";
}
