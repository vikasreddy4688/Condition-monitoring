#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
long previousmillis=0;
unsigned long currentmillies;
long prreviousmillis=0;
unsigned long cuurrentmillies;
void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
 
  dht.begin();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  float humi,tempe;
  tempe=1.11*temp; //Changing temperature according to the conditions inside the box
  humi=0.906*hum;
 if(humi>=55 )
  {
unsigned long currentmillies= millis();
if((currentmillies-previousmillis) >=5000)
{
 previousmillis=currentmillies; 
  Serial.print("Humidity: "); 
    Serial.print(humi);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(tempe);
    Serial.println(" *C");
    Serial.println("Humidity exceed event detected  ");
}
  }
else if(humi<55)
 {
   previousmillis=currentmillies;
  }
if(tempe>=15)
  {
unsigned long cuurrentmillies= millis();
if((cuurrentmillies-prreviousmillis) >=5000)
{
 prreviousmillis=cuurrentmillies; 
  Serial.print("Humidity: "); 
    Serial.print(humi);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(tempe);
    Serial.print(" *C");
    Serial.println("Temperature exceed event detected  ");
}
  
  }
 
  else if(tempe<15)
 {
   prreviousmillis=cuurrentmillies;
  }
 
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT");
  }
}
