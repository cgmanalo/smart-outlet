//Home Automation
//EE200D- Capstone Project
//Copyright (c) 2019-2020, A. Malgapo, G. Navarro, Cesar G. Manalo, Jr. (Adviser)

#include <EEPROM.h>
#include <avr/wdt.h>

#define FLAG '~'

char ctlrAddr;
char ctlrAddrRcvd;
char firstCharRcvd;
char secondCharRcvd;
char thirdCharRcvd;
char fourthCharRcvd;
char devAddr;
char devInstn;
byte ctlrStat;
int brightness = 0;
int fadeamount = 1;
int ctlrAddrCount;
int ctlrAddrMap = 10; //address of memory where controller address is stored
int ctlrAddrCountMap = 15; //address of number of addresses in this Controller
int timeDelayLightingAddr = 30;
int timeDelayACUAddr = 31;
int transmitWaitAddr = 32;
int receiveWaitAddr = 33;
int timeDelayLighting = 0;
int timeDelayACU = 0;
int transmitWait = 0;
int receiveWait = 200;


//ISR variables
volatile unsigned long last_micros;
long debouncingTime = 200;
const byte lighting1Pin = 3;
const byte ACU1A = 4;
const byte ACU1B = 5;
const byte lighting2Pin = 6;
const byte ACU2A = 7;
const byte ACU2B = 8;
const byte lighting3Pin = 9;
const byte ACU3A = 10;
const byte ACU3B = 11;
const byte interruptPin = 2;
void(* resetFunc) (void) = 0;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


void setup() {
 Serial.begin(9600);
 pinMode(lighting1Pin, OUTPUT);
 pinMode(ACU1A, OUTPUT);
 pinMode(ACU1B, OUTPUT);
 pinMode(lighting2Pin, OUTPUT);
 pinMode(ACU2A, OUTPUT);
 pinMode(ACU2B, OUTPUT);
 pinMode(lighting3Pin, OUTPUT);
 pinMode(ACU3A, OUTPUT);
 pinMode(ACU3B, OUTPUT);
 digitalWrite(lighting1Pin,HIGH);
 digitalWrite(ACU1A,HIGH);
 digitalWrite(ACU1B,HIGH);
 digitalWrite(lighting2Pin,HIGH);
 digitalWrite(ACU2A,HIGH);
 digitalWrite(ACU2B,HIGH);
 digitalWrite(lighting3Pin,HIGH);
 digitalWrite(ACU3A,HIGH);
 digitalWrite(ACU3B,HIGH); 
 getConfiguration();
 Preamble();
 watchdogSetup();
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void loop() {
  wdt_reset();
  brightness = brightness + fadeamount;
  analogWrite(ACU3B,brightness);
  delay(5);
  if (brightness == 1 || brightness == 255){
    fadeamount = -fadeamount;
  }

  if (Serial.available()){
    if (Serial.read() == FLAG){
      delay(receiveWait);
      if (Serial.read() == FLAG){
        delay(receiveWait);
        if (Serial.read() == FLAG){
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 
          delay(receiveWait);         
          firstCharRcvd = 0x00;
          firstCharRcvd = firstCharRcvd + Serial.read();
          
          for(int i=0; i<ctlrAddrCount; i++){
            if (firstCharRcvd == ctlrAddr+i){
              delay(receiveWait);
              ctlrAddrRcvd = firstCharRcvd;
              devAddr = Serial.read();
              if (devAddr != 'A' && devAddr != 'Q' && devAddr != '1' && devAddr != '2' && devAddr != '3' && devAddr != '4' && devAddr != '5'){devAddr = '?';}
              devInstn = Serial.read();
              if (devInstn != '0' && devInstn != '1') {devInstn = '?';}
                if (devAddr == 'Q'){
                  ctlrStat = 0x00;
                  if (digitalRead(lighting1Pin+3*i)){ctlrStat = 0x01;}
                  if (digitalRead(ACU1A+3*i)){ctlrStat = ctlrStat|0x02;}
                  if (digitalRead(ACU1B+3*i)){ctlrStat = ctlrStat|0x04;}
                  devInstn = ctlrStat + 48;
                }
                Serial.print("~~~003");
                Serial.print(ctlrAddrRcvd);
                Serial.print(devAddr);  
                Serial.print(devInstn);
                switch (devAddr){
                  case '1':
                    delay(timeDelayLighting);
                    if (devInstn == '1'){
                      digitalWrite(lighting1Pin+3*i,HIGH);
                    }
                    else if(devInstn == '0'){
                      digitalWrite(lighting1Pin+3*i,LOW);
                    }
                    break;
                  case '2':
                    delay(timeDelayACU);            
                    if (devInstn == '1'){
                      digitalWrite(ACU1A+3*i,HIGH);
                    }
                    else if(devInstn == '0'){
                      digitalWrite(ACU1A+3*i,LOW);
                    }
                    break;
                  case '3':
                    delay(timeDelayACU);
                    if (devInstn == '1'){
                      digitalWrite(ACU1B+3*i,HIGH);
                    }
                    else if(devInstn == '0'){
                      digitalWrite(ACU1B+3*i,LOW);
                    }
                    break;
                  case '4':
                  delay(0.5*(timeDelayACU+timeDelayLighting));
                    if (devInstn == '1'){
                      digitalWrite(lighting1Pin+3*i,HIGH);
                      digitalWrite(ACU1A+3*i,HIGH);
                      digitalWrite(ACU1B+3*i,LOW);
                    }
                    else if(devInstn == '0'){
                      digitalWrite(lighting1Pin+3*i,LOW);
                      digitalWrite(ACU1A+3*i,LOW);
                    }
                    break;
                  case '5':
                  delay(0.5*(timeDelayACU+timeDelayLighting));
                    if (devInstn == '1'){
                      digitalWrite(lighting1Pin+3*i,HIGH);
                      digitalWrite(ACU1A+3*i,LOW);
                      digitalWrite(ACU1B+3*i,HIGH);
                    }
                    else if(devInstn == '0'){
                      digitalWrite(lighting1Pin+3*i,LOW);
                      digitalWrite(ACU1B+3*i,LOW);
                    }
                    break;
                  case 'A':
                  delay(0.5*(timeDelayACU+timeDelayLighting));
                    if (devInstn == '1'){
                      digitalWrite(lighting1Pin+3*i,HIGH);
                      digitalWrite(ACU1A+3*i,HIGH);
                      digitalWrite(ACU1B+3*i,HIGH);
                    }
                    else if(devInstn == '0'){
                      digitalWrite(lighting1Pin+3*i,LOW);
                      digitalWrite(ACU1A+3*i,LOW);
                      digitalWrite(ACU1B+3*i,LOW);
                    }
                    break;
                  case 'Q':
                    break;
                  default:
                    break;
                }
            }//end of 2nd if
          }
      
          ///CONFIGURATION BLOCK ///
          if (firstCharRcvd == 'X'){ //configuration character
            delay(receiveWait);
            secondCharRcvd = Serial.read();
            if (secondCharRcvd == 'R'){
              String defaultSetting = "";
              defaultSetting.concat("\r\n***Active Config Settings***\r\nNumber of rooms: ");
              defaultSetting.concat(ctlrAddrCount);
              defaultSetting.concat("\r\nRoom Addresses:");
              for (int i=0; i<ctlrAddrCount; i++){
                defaultSetting.concat(" ");
                defaultSetting.concat(char(ctlrAddr+i));
                if (i<ctlrAddrCount-1){defaultSetting.concat(",");}
              }
              defaultSetting.concat("\r\nLight delay: ");
              defaultSetting.concat(timeDelayLighting);
              defaultSetting.concat(" ms.\r\nACU delay: ");
              defaultSetting.concat(timeDelayACU);
              defaultSetting.concat(" ms.\r\nTransmit delay: ");
              defaultSetting.concat(transmitWait);
              defaultSetting.concat(" ms.\r\nReceive delay: ");
              defaultSetting.concat(receiveWait);
              defaultSetting.concat(" ms.\r\n****************************");
              Serial.print("~~~");
              Serial.print((String)(defaultSetting.length()));
              Serial.print(defaultSetting);
            }
            
            if (secondCharRcvd == 'W'){ 
              //configuration address of first device group
              //address given must be any letter from B to Z, otherwise it defaults to B.
              thirdCharRcvd = Serial.read();
              if (thirdCharRcvd < 'A' || thirdCharRcvd > 'U'){thirdCharRcvd = 'A';}
              for(int i=0; i<ctlrAddrCount; i++){
                EEPROM.write(ctlrAddrMap+i, thirdCharRcvd+i);
              }
              ctlrAddr = thirdCharRcvd;  
              String roomAddressing = "";
              roomAddressing.concat("\r\nOK rm addr");
              for (int i=0; i<ctlrAddrCount; i++){
                roomAddressing.concat(" ");
                roomAddressing.concat(char(ctlrAddr+i));
                if (i<ctlrAddrCount-1){roomAddressing.concat(",");}
              }
              Serial.print("~~~0");
              Serial.print((String)(roomAddressing.length()));
              Serial.print(roomAddressing);
            }  
            if (secondCharRcvd == 'C'){ //6 s
              //configure number of devices
              //must be any number from 1 to 3, otherwise it defaults to 3.
              thirdCharRcvd = Serial.read();
              if (thirdCharRcvd < '1' || thirdCharRcvd > '3'){thirdCharRcvd = '3';}
              EEPROM.write(ctlrAddrCountMap, thirdCharRcvd);
              ctlrAddrCount = thirdCharRcvd-48;
              String countSetting = "";
              countSetting.concat("\r\nOK ");
              countSetting.concat(char(thirdCharRcvd));
              countSetting.concat(" rms. Use XW command to config rm. addr.");
              Serial.print("~~~0");
              Serial.print((String)(countSetting.length()));
              Serial.print(countSetting);
            } 
            if (secondCharRcvd == 'D'){ //6 s
              //configure time delay of lighting and ACU in seconds. must be any number from 0 to 5.
              //'L' refers to lighting delay while 'A' refers to ACU
              //if neither L or A is given, delay for both defaults to 0 seconds.
              //if delay given is outside 0 to 5, it defaults to 0.
              thirdCharRcvd = Serial.read();
              delay(receiveWait);
              fourthCharRcvd = Serial.read();
              if (fourthCharRcvd < '0' || fourthCharRcvd > '5'){fourthCharRcvd = '0';}
              String deviceActivationDelay;
              switch(thirdCharRcvd){
                case 'L':
                  EEPROM.write(timeDelayLightingAddr, fourthCharRcvd);
                  timeDelayLighting = (fourthCharRcvd-48)*500;
                  deviceActivationDelay = "";
                  deviceActivationDelay.concat("\r\nOK ");
                  deviceActivationDelay.concat(timeDelayLighting);
                  deviceActivationDelay.concat(" ms delay for light");
                  break;
                case 'A':
                  EEPROM.write(timeDelayACUAddr, fourthCharRcvd);
                  timeDelayACU = (fourthCharRcvd-48)*500;
                  deviceActivationDelay = "";
                  deviceActivationDelay.concat("\r\nOK ");
                  deviceActivationDelay.concat(timeDelayACU);
                  deviceActivationDelay.concat(" ms delay for ACU");
                  break;
                default:
                  fourthCharRcvd = '0';
                  EEPROM.write(timeDelayLightingAddr, fourthCharRcvd);
                  timeDelayLighting = (fourthCharRcvd-48)*500;
                  EEPROM.write(timeDelayACUAddr, fourthCharRcvd);
                  timeDelayACU = (fourthCharRcvd-48)*500;
                  deviceActivationDelay = "";
                  deviceActivationDelay.concat("\r\nOK ");
                  deviceActivationDelay.concat(timeDelayLighting);
                  deviceActivationDelay.concat(" and ");
                  deviceActivationDelay.concat(timeDelayACU);
                  deviceActivationDelay.concat(" ms delay for light and ACU");
                  break;
              }
              Serial.print("~~~0");
              Serial.print((String)(deviceActivationDelay.length()));
              Serial.print(deviceActivationDelay);
            }
            if (secondCharRcvd == 'S'){ //6 s
              thirdCharRcvd = Serial.read();
              if (thirdCharRcvd < '0' || thirdCharRcvd > '9'){thirdCharRcvd = '4';}
              EEPROM.write(transmitWaitAddr, thirdCharRcvd);
              transmitWait = (thirdCharRcvd-48)*200;
              String transmitDelaySetting = "";
              transmitDelaySetting.concat("\r\nOK ");
              transmitDelaySetting.concat(transmitWait);
              transmitDelaySetting.concat(" ms delay bet char transmit");
              Serial.print("~~~0");
              Serial.print((String)(transmitDelaySetting.length()));
              Serial.print(transmitDelaySetting);
            }
            if (secondCharRcvd == 'G'){ //6 s
              thirdCharRcvd = Serial.read();
              if (thirdCharRcvd < '1' || thirdCharRcvd > '5'){thirdCharRcvd = '1';}
              EEPROM.write(receiveWaitAddr, thirdCharRcvd);
              receiveWait = (thirdCharRcvd-48)*200;
              String receiveDelaySetting = "";
              receiveDelaySetting.concat("\r\nOK ");
              receiveDelaySetting.concat(receiveWait);
              receiveDelaySetting.concat(" ms delay bet char receive");
              Serial.print("~~~0");
              Serial.print((String)(receiveDelaySetting.length()));
              Serial.print(receiveDelaySetting);
            }
            if (secondCharRcvd == 'H'){
              thirdCharRcvd = Serial.read();
              switch(thirdCharRcvd){
                case 'C':
                  Serial.println();
                  Serial.println("No. of rms: XC<1-3>");
                  Serial.println("Addr of 1st rm: XW<A-U>");
                  Serial.println("Delay to oper dev: XD<L, A)><1-5 x 500 ms>");
                  Serial.println("Delay bet char transmit : XS<0-9 x 200 ms>");
                  Serial.println("Delay bet char receive : XG<1-5 x 200 ms>");          
                  Serial.println("Read config : XR");
                  break;
                case 'O':
                  Serial.println();
                  Serial.println("Oper rm dev: <A-U)><1-5, A)><1=ON,0=OFF)>");
                  Serial.println("Query rm dev stat: <A-U>Q");
                  break;
              }
            }
            if (secondCharRcvd == 'B'){
              thirdCharRcvd = Serial.read();
              if (thirdCharRcvd == 'T'){
                Serial.println("\nResetting...");
                delay(1000);
                resetFunc();
              }
            }
          }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        }
      }
    }
  }
}
//insert initiates here


//***** SUBROUTINES *****//
void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

void sendCharToPowerLine(char charToSend){
  Serial.print(charToSend);
  delay(transmitWait);
}

void sendCharDirect(char charToSend){
  Serial.print(charToSend);
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
  Serial.println("\nSite Controller ver. 2.4 - Model B\n");
  Serial.println("EE200D-2018 Capstone Project");
  Serial.println("Copyright (c) 2018-2019, K. Caringal, R. Burgos, P. Castillo, Cesar G. Manalo, Jr. (Adviser)");
  Serial.println("\nType 'XHC' for configuration commands help.");
  Serial.println("Type 'XHO' for operation commands help.");
  delay(500);
}

void getConfiguration()
{
 ctlrAddr = EEPROM.read(ctlrAddrMap); //retrieve first address
 if (ctlrAddr < 65 || ctlrAddr > 90){ctlrAddr = 65;}
 ctlrAddrCount = EEPROM.read(ctlrAddrCountMap)-48; //retrieve number of addresses
 if (ctlrAddrCount < 1 || ctlrAddrCount > 3){ctlrAddrCount = 3;}
 timeDelayLighting = (EEPROM.read(timeDelayLightingAddr)-48)*500;
 if (timeDelayLighting < 0 || timeDelayLighting > 2500){timeDelayLighting = 0;}
 timeDelayACU = (EEPROM.read(timeDelayACUAddr)-48)*500;
 if (timeDelayACU < 0 || timeDelayACU > 2500){timeDelayACU = 0;}
 transmitWait = (EEPROM.read(transmitWaitAddr)-48)*200;
 if (transmitWait < 0 || transmitWait > 1800){transmitWait = 0;}
 receiveWait = (EEPROM.read(receiveWaitAddr)-48)*200;
 if (receiveWait < 200 || receiveWait > 1000){receiveWait = 200;}
}
/*

boolean onDemandMode(){
  delay(1000);
  if (Serial.available()){
    delay(10);
    //empty receive buffer 
    while (Serial.available()) {
        Serial.read(); 
        delay(1);
    }
  }
  //send on-demand command once buffer is empty
  Serial.write(0x02); // correct way to send [STX]
  Serial.print("M3"); // ON-DEMAND
  Serial.write(0x03); // [ETX] marker
  delay(1000);
  if (!Serial.available()){
    return 1;
  }
  else{
    return 0; 
  }
}

String getVoltRMS(){
  int i;
  String tempString1 = "";
  String tempString2 = getPAParameter(VoltRMS);
  char tempCharArray[tempString2.length()];
  tempString2.toCharArray(tempCharArray,tempString2.length());
  for (i=0; i < tempString2.length(); i++){
    if (tempCharArray[i] == ','){
      break;
    }
  }
  for (i=i+1; i < tempString2.length()-4; i++){
    tempString1 +=  tempCharArray[i];
  }
  //return (String)  (((float) round(1000*tempString1.toFloat()))/1000.000);
  return tempString1;
}

String getAmpRMS(){
  int i;
  String tempString1 = "";
  String tempString2 = getPAParameter(AmpRMS);
  char tempCharArray[tempString2.length()];
  tempString2.toCharArray(tempCharArray,tempString2.length());
  for (i=0; i < tempString2.length(); i++){
    if (tempCharArray[i] == ','){
      break;
    }
  }
  for (i=i+1; i < tempString2.length()-2; i++){
    tempString1 +=  tempCharArray[i];
  }
  return tempString1;
}

String getPAParameter(String request){
  String serialString = "";
  Serial.write(0x02); // correct way to send [STX]
  Serial.print(request); // send request
  Serial.write(0x03); // [ETX] marker
  delay(1000);
  if (Serial.available()){
    delay(10);
    while (Serial.available()) {
        serialString = serialString + (char)Serial.read();
        delay(1);
    }
 }
 return serialString;
}

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sun";
    case Time::kMonday: return "Mon";
    case Time::kTuesday: return "Tue";
    case Time::kWednesday: return "Wed";
    case Time::kThursday: return "Thu";
    case Time::kFriday: return "Fri";
    case Time::kSaturday: return "Sat";
  }
  return "???";  //unknown day
}
String getWeekday (int dayNumber){
  String Weekday;  
  switch (dayNumber) {
      case 0:
        Weekday = "Sun";
        break;
      case 1:
        Weekday = "Mon";
        break;
      case 2:
        Weekday = "Tue";
        break;
      case 3:
        Weekday = "Wed";
        break;
      case 4:
        Weekday = "Thu";
        break;
      case 5:
        Weekday = "Fri";
        break;
      case 6:
        Weekday = "Sat";
        break;
      default:
        break;
  }
  return Weekday;
}
String getAMPM(int hourNumber){
  if (hourNumber < 12)
  {
      return "AM";
  }
  else
  {
      return "PM";
  }
}
String getNormalizedHour(int hourNumber){
  String hourString;
 
  if (hourNumber < 13)
  {
    hourString = String(hourNumber); 
  }
  else
  {
    hourString = String(hourNumber-12); 
  }
  if (hourString.length()==1)
  {
      return "0"+hourString;
  }
  else
  {
      return hourString;
  }
}

String getNormalizedMinute(int minuteNumber){
  String minuteString;
 
  minuteString = String(minuteNumber);
  if (minuteString.length()==1)
  {
      return "0"+minuteString;
  }
  else
  {
      return minuteString;
  }
}

String getNormalizedSecond(int secondNumber){
  String secondString;
 
  secondString = String(secondNumber);
  if (secondString.length()==1)
  {
      return "0"+secondString;
  }
  else
  {
      return secondString;
  }
}

unsigned int timeValue(String timeString){

   int AMPMAddress = timeString.length()-2;
   //return timeString.substring(3,5).toInt();

   if (timeString.substring(AMPMAddress)=="AM"){
     return 60*timeString.substring(0,2).toInt()+timeString.substring(3,5).toInt();
   }
   else{
     if (timeString.substring(0,2).toInt()==12){
       return 60*12+timeString.substring(3,5).toInt();
     }
     else{
       return 60*(12+timeString.substring(0,2).toInt())+timeString.substring(3,5).toInt();
     }
   }
}

void blink() {
  if ((long)(micros()-last_micros) > debouncingTime*1000){
    digitalWrite(lighting1Pin,!digitalRead(lighting1Pin));
    last_micros = micros();
  }
}
*/
