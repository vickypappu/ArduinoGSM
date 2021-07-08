#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  mySerial.begin(9600);
  delay(1000);
  mySerial.println("AT");
  delay(500);
  mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS=\"+918012503113\"\r");
  delay(500);
  mySerial.print("TEST by dharani gsm module!");
  delay(500);
  mySerial.write(26);
  digitalWrite(LED_BUILTIN, HIGH);
}
 
void loop()
{
}
