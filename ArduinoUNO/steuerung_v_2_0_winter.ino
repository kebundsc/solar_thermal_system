
#include <OneWire.h>            // OneWire-Bibliothek einbinden
#include <DallasTemperature.h>  // DS18B20-Bibliothek einbinden
#include <avr/wdt.h>



#define Puffer1 2   
#define Puffer2 3
#define Brauchwasser 4
#define Dach A0
#define Stoerung_LED A4
#define AUS HIGH
#define AN LOW
#define Pumpe_Solar 5
#define Pumpe_Ventil_Laden_Puffer 7
#define Ventil_Puffer1 8
#define Ventil_Puffer2 9


OneWire oneWirePuffer1(Puffer1);
OneWire oneWirePuffer2(Puffer2);
OneWire oneWireBrauchwasser(Brauchwasser);
              
DallasTemperature sensorsPuffer1(&oneWirePuffer1);   
DallasTemperature sensorsPuffer2(&oneWirePuffer2); 
DallasTemperature sensorsBrauchwasser(&oneWireBrauchwasser);

float tempPuffer1oben;
float tempPuffer1unten;
float tempPuffer2oben;
float tempPuffer2unten;
float tempBrauchwasseroben;
float tempBrauchwasserunten;
float tempDach;
String status_laden ="noheating";
int grenze;
int i=0;
boolean brauchwasserLaden;

void setup() {

  wdt_enable(WDTO_8S);
  // Serielle Ausgabe starten 
  Serial.begin(9600);
  sensorsPuffer1.begin();  // DS18B20 starten
  sensorsPuffer2.begin();  // DS18B20 starten
  sensorsBrauchwasser.begin();  // DS18B20 starten
  
  pinMode(Stoerung_LED, OUTPUT);
  pinMode(Ventil_Puffer1,OUTPUT);
  pinMode(Pumpe_Solar,OUTPUT);
  pinMode(Pumpe_Ventil_Laden_Puffer,OUTPUT);
  pinMode(Ventil_Puffer2,OUTPUT);
  
  digitalWrite(Ventil_Puffer1,AUS);
  digitalWrite(Ventil_Puffer2,AUS);
  digitalWrite(Pumpe_Ventil_Laden_Puffer,AUS);
  digitalWrite(Pumpe_Solar,AUS);
  grenze = 1;
  
  
 
}


void loop(){

 
  sensorsPuffer1.requestTemperatures(); // Temperatursensor(en) auslesen
  tempPuffer1unten = sensorsPuffer1.getTempCByIndex(0);
  tempPuffer1oben = sensorsPuffer1.getTempCByIndex(1);
 
  sensorsPuffer2.requestTemperatures(); // Temperatursensor(en) auslesen
  tempPuffer2unten = sensorsPuffer2.getTempCByIndex(0);
  tempPuffer2oben = sensorsPuffer2.getTempCByIndex(1);
 
  sensorsBrauchwasser.requestTemperatures(); // Temperatursensor(en) auslesen
  tempBrauchwasserunten = sensorsBrauchwasser.getTempCByIndex(0);
  tempBrauchwasseroben = sensorsBrauchwasser.getTempCByIndex(1);

  tempDach=getTemp1_ptc();
  
  analogWrite(Stoerung_LED,255);
  i++;
  if(i==5)
  {
    analogWrite(Stoerung_LED,0);
    steuern();
    i=0;
  }
  printValues();
  
     
  wait(100);
   
 wdt_reset();
}


void steuern()
{
 // Serial.println("steuern");
 /* if(tempDach-tempBrauchwasseroben>grenze&&tempBrauchwasseroben<=65&&tempBrauchwasserunten<=60)
  {
   // Serial.println("laden");
   brauchwasserladen();
    brauchwasserLaden = true;
  }
  else */
 
    //Serial.println("nichtladen");
    brauchwasserLaden = false;
  //}
  if(!brauchwasserLaden)
  {
    if(tempPuffer1oben>tempPuffer2oben)
    {
      if(tempPuffer1unten<56&&tempPuffer1oben<60)
      {
        if(tempDach-tempPuffer1oben>grenze)
        {
          puffer1Laden();
        }
        else
        {
          stopLaden();
        }
      }
      else
      {
         if(tempPuffer2unten<56&&tempPuffer2oben<60)
         {
           if(tempDach-tempPuffer2oben>grenze)
           {
              puffer2Laden();
           }
           else
           {
             stopLaden();
           }
         }
        else
        {
         stopLaden();//STOP
        }
      }
    }
    else
    {
      if(tempPuffer2unten<56&&tempPuffer2oben<60)
      {
        if(tempDach-tempPuffer2oben>grenze)
        {
          puffer2Laden();
        }
        else
        {
          stopLaden();
        }
      }
      else
      {
         if(tempPuffer1unten<56&&tempPuffer1oben<60)
         {
           if(tempDach-tempPuffer1oben>grenze)
           {
             puffer1Laden();
           }
         }
         else
         {
          stopLaden(); //STOP
         }
        
      }
      
    }
  }
  else
  {
    if(!brauchwasserLaden)
    {
      stopLaden(); //STOP
    }
  }
}

void brauchwasserladen()
{
  digitalWrite(Pumpe_Ventil_Laden_Puffer,AUS);
  wait(100);
  digitalWrite(Ventil_Puffer1,AUS);
  wait(100);
  digitalWrite(Ventil_Puffer2,AUS);
  wait(100);
  digitalWrite(Pumpe_Solar,AN);
  status_laden="fwheating";
}

void puffer1Laden()
{
  digitalWrite(Ventil_Puffer1,AN);
  wait(100);
  digitalWrite(Ventil_Puffer2,AUS);
  wait(500);
  digitalWrite(Pumpe_Ventil_Laden_Puffer,AN);
  wait(100);
  digitalWrite(Pumpe_Solar,AN);
  status_laden="b1heating";
  
}

void puffer2Laden()
{
  digitalWrite(Ventil_Puffer2,AN);
  wait(100);
  digitalWrite(Ventil_Puffer1,AUS);
  wait(500);
  digitalWrite(Pumpe_Ventil_Laden_Puffer,AN);
  wait(100);
  digitalWrite(Pumpe_Solar,AN);
  status_laden="b2heating";
}

void stopLaden()
{
  //Serial.println("stopLaden");
   digitalWrite(Pumpe_Solar,AUS);
   wait(100);
   digitalWrite(Pumpe_Ventil_Laden_Puffer,AUS);
   wait(100);
   digitalWrite(Ventil_Puffer1,AUS);
   wait(100);
   digitalWrite(Ventil_Puffer2,AUS);
   status_laden="noheating";
}



void printValues()
{
  
  Serial.print(tempPuffer1oben);
  Serial.print("&&");
  Serial.print(tempPuffer1unten);
  Serial.println("&&Puffer1");
  wait(50);
  Serial.print(tempPuffer2oben);
  Serial.print("&&");
  Serial.print(tempPuffer2unten);
  Serial.println("&&Puffer2");
  wait(50);
  Serial.print(tempBrauchwasseroben);
  Serial.print("&&");
  Serial.print(tempBrauchwasserunten);
  Serial.println("&&Brauchwasser");
  wait(50);
  Serial.print(tempDach);
  Serial.println("&&Dach");
  wait(50);
  Serial.print(status_laden);
  Serial.println("&&Status");
  wait(50);
  
}
float getTemp1_ptc()
{
  float temp=0;
  for(int i=0;i<30;i++)
  {
  int sensorValue = analogRead(Dach);
  float voltage = sensorValue *(5.0/1023.0);
 
  //voltage-=0.1;
  float v1=4.9-voltage;
  
  float r1=2450;
  float current=v1/r1;
  float rptc=voltage/current;
  //rptc-=155;
  temp=temp+(rptc-1630.0)/18.74;
  wait(30);
  }
  return temp/30;
}
 
 void wait (unsigned long milliseconds)
 {
   unsigned long start = millis();
   while(millis()<=(start+milliseconds))
   {
   }
 }
 



 
 
