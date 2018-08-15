
/*

  What kind of sensors/applications make sense here?
  
    - Temperature monitoring of fermentors
    - Temperature monitoring of boil, etc...
    - Fermentation rates
    - Absolute density of wort (pre and post ferment), monitor over time
    - Flow meters to monitor wort volume, etc...
    - Keg volume
    - Mash tun temperature monitoring
    
   

//Future growth
http://www.amazon.com/MEGAshield-KIT-Arduino-MEGA-2560/dp/B006SJSBBM/ref=pd_sim_e_34

http://www.amazon.com/SainSmartTM-Display-Interface-MicroSD-Arduino/dp/B008HWTVQ2/ref=pd_sim_e_42


//General information
https://www.sparkfun.com/tutorials/131

https://www.oscsys.com/projects/brewtroller

http://middlewaresensing.wordpress.com/2010/08/04/wine-monitoring/

http://atlas-scientific.com/product_pages/sensors/env-tmp.html

http://www.aliexpress.com/item/New-5V-2-Channel-Relay-Module-Shield-for-Arduino-ARM-PIC-AVR-DSP-Electronic-10A/716233877.html

http://playground.arduino.cc/Learning/SerialLCD

http://www.protocase.com/

http://www.adafruit.com/products/482

http://playground.arduino.cc/Learning/SerialLCD

http://learn.parallax.com/4-digit-7-segment-led-display-arduino-demo

http://arduino-info.wikispaces.com/ArduinoPower

http://www.homebrewtalk.com/f36/brewing-yuri-video-series-49645/

https://www.sparkfun.com/tutorials/131

http://www.homebrewtalk.com/f51/arduino-based-beer-monitor-system-343018/

http://arduino-info.wikispaces.com/ArduinoPower

https://kegbot.org/

hydrometer:
http://arduino.cc/forum/index.php?topic=122307.0

http://homebrew.stackexchange.com/questions/5424/homebrewing-and-arduino


it seems that a hall sensor would do this.  Place a tube in wort, sterilized, and read the value from outside fermentor
http://www.homebrewtalk.com/f51/digital-hydrometer-100412/index8.html

http://www.measurit.com/pdf/flexim/piox/measurit-flexim-piox-r-project-rothaus-0906/

http://www.ias.ac.in/pramana/v74/p661/fulltext.pdf

http://www.sciencedirect.com/science/article/pii/S0925400502000126

http://www.zindello.com.au/wordpress/2012/12/18/beer-mmmmm-beeeeeer-and-arduino/

http://www.sciencedirect.com/science/article/pii/S0925400502000126

other topics:
http://www.homebrewing.com/articles/home-brewing-automation.php


CO2 Monitor - carb perfectly, every time.


*/

#include <LiquidCrystal.h>

#include <SimpleTimer.h>
#include <EEPROM.h>

//Inc/Dec buttons
int IncreaseButtonPin         = 15;
int DecreaseButtonPin         = 16;
//Temperature Probe
int Temp                      = 0 ;

int temperaturePin            = 0;
int SetTemp                   = 0;
int DefaultSetTemp            = 40;
int TempRefreshRate           = 200;
SimpleTimer timer;
SimpleTimer DebugTimer;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//Status LED
int LedRed                   = 6;
int LedGreen                 = 7;
int LedBlue                  = 9;


void setup(){
  //Serial.begin(9600);
  setupLCD();
  setupTemp();
  //setupIRRemote();
  //setupButtons();
}

void setupButtons(){
  pinMode(IncreaseButtonPin, INPUT);
  digitalWrite(IncreaseButtonPin, HIGH); // connect internal pull-up 
  pinMode(DecreaseButtonPin, INPUT);
  digitalWrite(DecreaseButtonPin, HIGH); // connect internal pull-up 

}

void setupLCD(){
  lcd.begin(16, 2);
}

void setupTemp(){
  analogReference(INTERNAL);
  int StoredSetTemp = EEPROM.read(0);
  //int StoredSetTemp = 0;
  //Serial.print("Stored Temp: ");
  //Serial.println(StoredSetTemp);
  
  //RESET EEPROM
  //EEPROM.write(0,0);
  if(StoredSetTemp!=0){
    
    SetTemp = (int)StoredSetTemp;  
  }else{
    
    SetTemp = DefaultSetTemp;
    EEPROM.write(0, (byte)DefaultSetTemp);
  }
  //ReadTemp();
  //timer.setInterval(TempRefreshRate, ReadTemp);
  DebugTimer.setInterval(TempRefreshRate, DebugTemp);
}
void loop(){
  
  //WriteLed(Temp);

  //RegulateTemperature(Temp);

  char string[16]="";
  lcd.setCursor(0, 0);
  sprintf(string, "Temperature:  %i", Temp);
  lcd.print(string);

  lcd.setCursor(0, 1);
  
  if(SetTemp>Temp){
    sprintf(string, "Heating to:   %i", SetTemp);
  }else if(SetTemp<Temp){
     sprintf(string, "Cooling to:   %i", SetTemp);
  }else if(SetTemp==Temp){
      sprintf(string, "Maintain at:     %i", SetTemp); 
  }

  lcd.print(string);

  //timer.run();  
  DebugTimer.run();  
 // CheckButtons();
}

//Buttons


void CheckButtons(){

 if(CheckIncrease()==true){
    //Serial.print("Increasing Set Temp: ");
    //Serial.println(SetTemp);
    IncreaseTemp();
    //DecreaseTemp();
    
  }
  if(CheckDecrease()==true){
    //Serial.print("Decreasing Set Temp: ");
    //Serial.println(SetTemp);
    DecreaseTemp();
  } 
}
boolean CheckIncrease()
{
  int value = analogRead(IncreaseButtonPin);// * (5.0 / 1024.0); // pin low -> pressed
    //Serial.print("CheckIncrease: ");
    //Serial.println(analogRead(IncreaseButtonPin));
//  return (value>200);  
  return (value<100);
}

boolean CheckDecrease()
{
  int value = analogRead(DecreaseButtonPin);// * (5.0 / 1024.0); // pin low -> pressed
  
    //Serial.print("CheckDecrease: ");
    //Serial.println(value);

  return (value<100);
}


void RegulateTemperature(long Temp){
   if(Temp>SetTemp){
      analogWrite(LedBlue, 255);
     
   }else if(Temp<SetTemp){
     
     //Serial.println("Turning on Red");
     analogWrite(LedRed, 255);     
   }else if(Temp==SetTemp){
     //Serial.println("Turning on Green");
     analogWrite(LedGreen, 255);     
   }
}

void IncreaseTemp(){
  SetTemp++;
  EEPROM.write(0, (byte)SetTemp);
  delay(200);
}

void DecreaseTemp(){
  SetTemp--;
  EEPROM.write(0, (byte)SetTemp);
  delay(200);
}
void DebugTemp(){
 
   if(Temp<SetTemp+20){
      Temp++; 

   }else if(Temp>SetTemp){
     //Temp--; 
   }
}
void ReadTemp1(){

  int reading;
  
  int analogVal = 0;
  //analogVal = analogRead(tempPin); 
  //First reading is bad, discard it
  analogRead(temperaturePin);
  
  // takes 10 samples to make sure we get a good value
  for(int i = 0; i < 10; i++) { 
    analogVal += analogRead(temperaturePin); 
    delayMicroseconds(500);
  }
  analogVal = analogVal/10;
  //Serial.println(analogVal);
  //Temp = round(((9*((5.0 * analogVal * 10) / 1023))/5)+32);
  
  Temp = analogVal/10; //10mV per deg C
  //Temp = 110 * (float)analogVal / 1024;
  Temp = (Temp*9/5)+32;
   
  //Temp = 52;
  //Serial.print("Read Temp: ");
  //Serial.println(Temp);
}


