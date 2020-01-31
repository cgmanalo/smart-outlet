//Home Automation
//EE200D- Capstone Project
//Copyright (c) 2019-2020, A. Malgapo, G. Navarro, Cesar G. Manalo, Jr. (Adviser)

#include <EEPROM.h>
#include <avr/wdt.h>

#define FLAG '~'
String serialData;
const byte Outlet1 = 2;
const byte Outlet2 = 3;

void setup() {
  Serial.begin(9600);
  pinMode(Outlet1,OUTPUT);
  pinMode(Outlet2,OUTPUT);
  digitalWrite(Outlet1,LOW);
  digitalWrite(Outlet2,LOW);
  Preamble();
}

void loop() {
  serialData = extractSerialData2();
  if (serialData.length()>0){
      if (serialData == "HB"){
          sendReply("Room Controller Ok.");
      }
      else if(serialData == "ON-OUTLET1"){
          if(OPEROutlet(Outlet1,"ON")){
              sendReply("OUTLET1 is ON.");
          }
          else{
              sendReply("OUTLET1 is OFF.");
          }
      }
      else if(serialData == "OFF-OUTLET1"){
          if(OPEROutlet(Outlet1,"OFF")){
              sendReply("OUTLET1 is ON.");
          }
          else{
              sendReply("OUTLET1 is OFF.");
          }
      }
      else if(serialData == "TOG-OUTLET1"){
          if(OPEROutlet(Outlet1,"TOG")){
              sendReply("OUTLET1 is ON.");
          }
          else{
              sendReply("OUTLET1 is OFF.");
          }
      }
      else{
          Serial.println(serialData);
      }
  }

}

///////////////////////////////////////////////
//***** SUBROUTINES *****//
bool OPEROutlet(int OutletNo, String Operation){
  if (Operation == "ON"){
      digitalWrite(OutletNo,HIGH);
  }
  else if(Operation == "OFF"){
      digitalWrite(OutletNo,LOW);
  }
  else if(Operation == "TOG"){
      digitalWrite(OutletNo,!digitalRead(OutletNo));
  }
  delay(10);
  return(digitalRead(OutletNo));
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

void sendReply(String replyMessage){
  Serial.println(replyMessage);
}

String extractSerialData2(){
  Serial.setTimeout(10);
  return Serial.readStringUntil('\n');
}

void watchdogSetup(void)
{
  cli();       // disable all interrupts
  wdt_reset(); // reset the WDT timer
  /*
   WDTCSR configuration:
   WDIE = 1: Interrupt Enable
   WDE = 1 :Reset Enable
   WDP3 = 1 :For 8000ms Time-out
   WDP2 = 0 :For 8000ms Time-out
   WDP1 = 0 :For 8000ms Time-out
   WDP0 = 1 :For 8000ms Time-out
  */
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Set Watchdog settings:
   WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0);
  sei();
}

void Preamble()
{
  delay(500);
  Serial.println("\nSmart Outlet ver. 1.1 - Model A\n");
  Serial.println("EE200D-2019 Capstone Project");
  Serial.println("Copyright (c) 2019-2020, A. Malgapo, G. Navarro, Cesar G. Manalo, Jr. (Adviser)");
  delay(500);
}
