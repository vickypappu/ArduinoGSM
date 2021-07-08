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
int relay = 12; // Output for Relay Control

void setup() 
{
      pinMode(relay, OUTPUT);
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(relay, LOW); // Initial state of the relay
      Serial.begin(9600);
      mySerial.begin(9600); 

     while(!mySerial.available()){
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
  if(mySerial.available()){
      delay(100);

      // Serial Buffer
      while(mySerial.available()){
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        }

        delay(10);      
        Serial.println("here1");
        Serial.println(inputString);
        Serial.println("here2");
        inputString.toUpperCase(); // Uppercase the Received Message

        //turn RELAY ON or OFF
        if (inputString.indexOf("OFF") > -1){
          digitalWrite(relay, LOW);
          digitalWrite(LED_BUILTIN, LOW);
          }
        if (inputString.indexOf("ON") > -1)
          digitalWrite(relay, HIGH);
          digitalWrite(LED_BUILTIN, HIGH);
          }          

        delay(50);

        //Delete Messages & Save Memory
        if (inputString.indexOf("OK") == -1){
        mySerial.println("AT+CMGDA=\"DEL ALL\"");

        delay(1000);}

        inputString = "";
  }
}
