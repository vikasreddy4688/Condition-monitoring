#include <SPI.h> //for the SD card module
#include <SD.h> // for the SD card
#include <DHT.h> // for the DHT sensor
#include <Wire.h>//i2c will be used for both timer and acceleration sensors
#include <DS3231.h>
//define DHT pin
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
long previousmillis=0;
unsigned long currentmillies;
long prreviousmillis=0;
unsigned long cuurrentmillies;
long cas;
// change this to match your SD shield or module;
// Arduino Ethernet shield and modules: pin 4
// Data loggin SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4; 
String data="";
int mark = 0;
boolean Mark_Start=false;
boolean valid=false;
String GGAUTCtime,GGAlatitude,GGAlongitude,GPStatus,SatelliteNum,HDOPfactor,Height,
PositionValid,RMCUTCtime,RMClatitude,RMClongitude,Speed,Direction,Date,Declination,Mode;
DS3231  rtc(SDA1, SCL1);


// Create a file to store the data
File myFile;
void setup() {
  //initializing Serial monitor
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.println(0);
  delay(1000);
   //initializing the DHT sensor
  dht.begin();
  // setup for the SD card
  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
   rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(15,05, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(23, 3, 2018);   // Set the date to January 1st, 2014
    
  //open file
  myFile=SD.open("DATA10.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (myFile) {
    Serial.println("File opened ok");
    // print the headings for our data
    myFile.println("Date,Temperature,humidity ºC");
  }
  myFile.close();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loggingTemperatureHumidity() {
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
    Serial.print("Humidity exceed event detected  ");
    myFile.println("Humidity exceed event detected  ");
    myFile.print("Temperature:");
    myFile.print(tempe);
    myFile.println("C");
     myFile.print("Humidity:");
      myFile.println(humi);
    myFile.print("Latitude:");
    myFile.print(RMClatitude);
    myFile.print("   ");
      myFile.println(GGAlatitude);
    myFile.print(",");
     myFile.print("Longitude:");
    myFile.print(RMClongitude);
    myFile.print("   ");
      myFile.println(GGAlongitude);
     
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
    Serial.println(" *C");
    Serial.print("Temperature exceed event detected  ");
    myFile.print("Temperature exceed event detected  ");
    myFile.print("Temperature:");
    myFile.println(tempe);
    myFile.print(",");
    myFile.print("Humidity:");
     myFile.print(humi);
    myFile.print("Latitude:");
    myFile.print(RMClatitude);
    myFile.print("   ");
      myFile.println(GGAlatitude);
    myFile.print(",");
     myFile.print("Longitude:");
    myFile.print(RMClongitude);
    myFile.print("   ");
      myFile.println(GGAlongitude);
    
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
  if (tempe>15 & humi<55)
  {
    Serial.println("Temperature and Humidity exceed event detected  ");
  }
}
   //---------------------Data to be stored in SD card-------------------------------------------------------------------------------
  //myFile = SD.open("DATA.txt", FILE_WRITE);
  //if (myFile) {
    //Serial.println("open with success");
    
  //} 
  //myFile.close();
  //--------------------------Accelerometer setting datarate---------------------------------------------------------------

 
void acceleration(){
   
}

//----------------------------------------------------------------------------------------------------------------------
void datetime()
{
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
  
  // Wait one second before repeating :)
  delay (1000);
}


void loop() {

  /* Get a new sensor event */ 
  acceleration();
  loggingTemperatureHumidity();
  datetime();
  while (Serial1.available()> 0){
    if(Mark_Start){
      data=reader();
      Serial.println(data);
      if(data.equals("GPGGA")){
        //Serial.println(1);
        GGAUTCtime=reader();
        GGAlatitude=reader();
        GGAlatitude+=reader();
        GGAlongitude=reader();
        GGAlongitude+=reader();
        GPStatus=reader();
        SatelliteNum=reader();
        HDOPfactor=reader();
        Height=reader();
        Mark_Start=false;
        valid=true;
        data="";

      }
     
      else if(data.equals("GPRMC")){
        //Serial.println(4);
        RMCUTCtime=reader();
        PositionValid=reader();
        RMClatitude=reader();
        RMClatitude+=reader();
        RMClongitude=reader();
        RMClongitude+=reader();
        Speed=reader();
        Direction=reader();
        Date=reader();
        Declination=reader();
        Declination+=reader();
        Mode=reader();
        valid=true;
        Mark_Start=false;
        data="";
      }
      else if(data.equals("GPVTG")){
        Serial.println(5);
        Mark_Start=false;
        data="";
      }
      else{
        Serial.println(6);
        Mark_Start=false;
        data="";
      }
    }
    if(valid){
      if(PositionValid=="A"){
        Serial.println("Position Valid");
      }
      else{
        Serial.println("Your position is not valid.");
      }
     
 
      Serial.print("Latitude:");
      Serial.print(RMClatitude);
      Serial.print("   ");
      Serial.println(GGAlatitude);
      Serial.print("Longitude:");
      Serial.print(RMClongitude);
      Serial.print("   ");
      Serial.println(GGAlongitude);
    
      
      valid=false;
      delay(3000);
    }
    if(Serial1.find("$")){
      Serial.println("capture");
      Mark_Start=true;
    }
  }

}

String reader(){
  String value="";
  int temp;
startover:
  while (Serial1.available() > 0){
    delay(2);
    temp=Serial1.read();
    if((temp==',')||(temp=='*')){
      if(value.length()){
        //Serial.println("meaningful message");
        return value;
      }
      else {
        //Serial.println("empty");
        return "";
      }     
    }
    else if(temp=='$'){
      //Serial.println("failure");
      Mark_Start=false;
    }
    else{
      //Serial.println("add");
      value+=char(temp);
    }
  }
  while (!(Serial1.available()>0)){
  }
  goto startover;
}
