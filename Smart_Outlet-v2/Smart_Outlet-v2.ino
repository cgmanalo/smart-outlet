//Home Automation
//EE200D- Capstone Project
//Copyright (c) 2019-2020, A. Malgapo, G. Navarro, Cesar G. Manalo, Jr. (Adviser)

#include <EEPROM.h>
#include <avr/wdt.h>

#define START_FLAG '~'
#define END_FLAG '^'
#define VALID true
unsigned long Start_Timer;
unsigned long Start_Timer2;
char Data_Field[20];
char Data_Char;
String Device_Name;
String Device_Command;

String serialData;
const byte Outlet1 = 2;
const byte Outlet2 = 3;

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Outlet1,OUTPUT);
  pinMode(Outlet2,OUTPUT);
  digitalWrite(Outlet1,LOW);
  digitalWrite(Outlet2,LOW);
  Preamble();
}

void loop() {

  if (GetSerialData() == VALID) {
    serialData = (String)Data_Field;
    Device_Name = extractField(serialData,1);
    Device_Command = extractField(serialData,2);
    if(Device_Name == "OUTLET1"){
      if(OPEROutlet1(Device_Command)){
        sendReply("\n" + Device_Name + " is ON.");
      }
      else{
        sendReply("\n" + Device_Name + " is OFF.");
      }
    }
    else if(Device_Name == "OUTLET2"){
      if(OPEROutlet2(Device_Command)){
        sendReply("\n" + Device_Name + " is ON.");
      }
      else{
        sendReply("\n" + Device_Name + " is OFF.");
      }
    }
    else if(Device_Name == "CONTROL1"){
      if(Device_Command == "RST"){
        resetFunc();
      }
        sendReply("\n" + Device_Name + " Ok.");
    }
    else{
        Serial.println("\nUnrecognize Device Name or Instruction.");
    }
  }
}

///////////////////////////////////////////////
//***** SUBROUTINES *****//
bool OPEROutlet1(String Operation){
  if (Operation == "ON"){
      digitalWrite(Outlet1,HIGH);
  }
  else if(Operation == "OFF"){
      digitalWrite(Outlet1,LOW);
  }
  else if(Operation == "TOG"){
      digitalWrite(Outlet1,!digitalRead(Outlet1));
  }
  delay(10);
  return(digitalRead(Outlet1));
}
bool OPEROutlet2(String Operation){
  if (Operation == "ON"){
      digitalWrite(Outlet2,HIGH);
  }
  else if(Operation == "OFF"){
      digitalWrite(Outlet2,LOW);
  }
  else if(Operation == "TOG"){
      digitalWrite(Outlet2,!digitalRead(Outlet2));
  }
  delay(10);
  return(digitalRead(Outlet2));
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
bool GetSerialData(){
  if (Serial.read() == START_FLAG){
    Start_Timer = millis();
    while(millis()-Start_Timer < 100){
      if(Serial.read() == START_FLAG){
        Start_Timer = millis();
        while(millis()-Start_Timer < 100){
          if(Serial.read() == START_FLAG){ //<<<<<

            int i=0;
            Data_Field[0] = '\0';
            Start_Timer2 = millis();
            while(millis()-Start_Timer2 < 3000){
              Start_Timer = millis();
              while(millis()-Start_Timer < 100){
                Data_Char = Serial.read();
                if(Data_Char == END_FLAG){
                  Start_Timer = millis();
                  while(millis()-Start_Timer < 100){
                    Data_Char = Serial.read();
                    if(Data_Char == END_FLAG){
                      Start_Timer = millis();
                      while(millis()-Start_Timer < 100){
                        Data_Char = Serial.read();
                        if(Data_Char == END_FLAG){
                          Data_Field[i++] = '\0';
                          return true;
                        }
                        else if(Data_Char != -1){
                          Data_Field[i++] = Data_Char;
                          break;
                        }
                      }
                    }
                    else if(Data_Char != -1){
                      Data_Field[i++] = Data_Char;
                      break;
                    }
                  }
                  break;
                }
                else if(Data_Char != -1){
                  Data_Field[i++] = Data_Char;
                  break;
                }
              }
            }
            break;
          } //<<<<<
        }
        break;
      }
    }
  }
  return false;
}


String extractField(String rawString, int fieldNum){
   char dataArray[30];
   char fieldArray[25];   
   char inChar;
   
   rawString.toCharArray(dataArray,rawString.length()+1);

   //first field
   if (fieldNum == 1){
     int i=0;
     for(i;i<rawString.length();i++){
       inChar = dataArray[i];
       if (inChar == '|'){
         break;
       }
       fieldArray[i] = inChar;
     } 
     fieldArray[i]='\0';
     return fieldArray;
   }

   //second field
   else if (fieldNum == 2) {
     int i = rawString.indexOf('|')+1;
     int j=i;
     for(i;i<rawString.length();i++){
       inChar = dataArray[i];
       if (inChar == '|'){
         break;
       }
       fieldArray[i-j] = inChar;
     }
     fieldArray[i-j]='\0';
     return fieldArray;
   }

  //third field
   else if (fieldNum == 3) {
     int i = rawString.indexOf('|',rawString.indexOf('|')+1)+1;
     int j=i;
     for(i;i<rawString.length();i++){
       inChar = dataArray[i];
       if (inChar == '|'){
         break;
       }
       fieldArray[i-j] = inChar;
     }
     fieldArray[i-j]='\0';
     return fieldArray;
   }
   else if (fieldNum == 4) {
     int i = rawString.indexOf('|',rawString.indexOf('|',rawString.indexOf('|')+1)+1)+1;
     int j=i;
     for(i;i<rawString.length();i++){
       inChar = dataArray[i];
       if (inChar == '|'){
         break;
       }
       fieldArray[i-j] = inChar;
     }
     fieldArray[i-j]='\0';
     return fieldArray;
   }
     
   return "";
}
