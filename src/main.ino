/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  AUTOMATED CHICKEN COOP                                                     //
//  Protect your beloved chickens from the wild animals out there!             //
//  Using Arduino Nano; DC motor controller; DS3231 RTC; PIR movement sensor   //
//                                                                             //
//  Futur functions to add                                                     //
//    - Power saving functions using 'Low power' library                       //
//                                                                             //
//  © 2018 Created by J3fPatat                                                 //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <DS3231.h>
#include <Dusk2Dawn.h>
RTClib RTC;
DS3231 temp;

////////////////////////
//  Define Variables  //
////////////////////////

const int pinLedGreen = 3;
const int pinLedRed =  2;

const int pinMotorUp = 8;
const int pinMotorDown = 9;
int motorUp;
int motorDown;
unsigned long motorStart;

const int pinMicroUp = 12;
const int pinMicroDown = 11;
int microUp;
int microDown;
unsigned long microUptime;
unsigned long microDowntime;

const int pinMovement = 5;
int movementDetected;
unsigned long movementDetectedStarttime;

unsigned long timestamp;
int timeMinutes;
int error = 0;



void setup() {
  Serial.begin (115200);  // Start the serial port
  Wire.begin();           // Start the I2C interface
  
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT); 
  pinMode(13, OUTPUT);
  
  // Show we are initialising
  digitalWrite(pinLedGreen, HIGH);
  digitalWrite(pinLedRed, HIGH);
  delay(500);
  digitalWrite(pinLedGreen, LOW);
  digitalWrite(pinLedRed, LOW);
  delay(200); 
  digitalWrite(pinLedGreen, HIGH);
  digitalWrite(pinLedRed, HIGH);
  delay(1000);
  // Blink show over :)
  
  pinMode(pinMotorUp, OUTPUT);
  pinMode(pinMotorDown, OUTPUT);

  pinMode(pinMicroUp, INPUT_PULLUP);
  pinMode(pinMicroDown, INPUT_PULLUP);

  pinMode(pinMovement, INPUT);
  
  // initialising ended
  digitalWrite(pinLedGreen, LOW);  
  digitalWrite(pinLedRed, LOW);
  digitalWrite(13, LOW);
}


void loop() {
 
//////////////////
//  Error Loop  //
//////////////////

  while(error) {
    digitalWrite(pinMotorUp, LOW);
    digitalWrite(pinMotorDown, LOW);
  
    delay(500);
    digitalWrite(pinLedRed, HIGH);
    digitalWrite(pinLedGreen, HIGH);
    delay(500);
    digitalWrite(pinLedRed, LOW);
    digitalWrite(pinLedGreen, LOW);
      
    Serial.println("ERROR");
  }
  
  ////////////////////////// 
  //  Defining slow variables  //
  ////////////////////////// 

  // UNIX Timestamp (long)
  // Store current timedata in variable t
  DateTime t = RTC.now();
  timestamp = t.unixtime();
  
  // Sunrise & Sunset 
  Dusk2Dawn locatie(50.882743, 4.717356, +1);                             // Leuven(50.882743, 4.717356); Koksijde(51.102372, 2.620951)
  int timeSunrise  = locatie.sunrise(t.year(), t.month(), t.day(), false);  // value of sunrise in minutes since 00:00 UTC+1
  int timeSunset   = locatie.sunset(t.year(), t.month(), t.day(), false);   // value of sunset in minutes since 00:00UTC+1
  
  // UNIX Timestamp (long)
  timestamp = t.unixtime();
  
  // Sup up or down?
  timeMinutes = t.hour()*60 + t.minute();
  //timeMinutes = 900;  //Debugging purposes only, you don't want your chicken to be eaten!
  
  int sunlight;
  if (timeMinutes >= timeSunrise && timeMinutes <= (timeSunset + 30)) sunlight = 1;   // take 30 min after sunset so all chickens are inside
    else sunlight = 0;
    
  
  ////////////////// 
  // Check input  //
  ////////////////// 
  
  setVariables();
  
  ////////////////////
  //  Motorsturing  //
  ////////////////////
  
  if(sunlight && !microUp){                                   // go up when: sunlight && upper microswitch not active
    motorStart = millis();
    while ((!error && !microUp)==1){
        if ((millis() - motorStart)>30000) error = 1;         // no reaction after 30 seconds? Something went wrong!
        setVariables();
        digitalWrite(pinMotorUp, HIGH);
    }
    digitalWrite(pinMotorUp, LOW);
    motorStart = 0;
  }
  
  
  if(!sunlight && !microDown && !movementDetected){           // go down when: no sunlight + 30min && lower microswitch not active && no movement during last 5 minutes (already in variable)
    motorStart = millis();
    while (!error && !microDown){
        if ((millis() - motorStart)>30000) error = 1;         // no reaction after 30 seconds? Something went wrong!
        setVariables();
        digitalWrite(pinMotorDown, HIGH);
    }
    digitalWrite(pinMotorDown, LOW);
    motorStart = 0;
  }
  
  ////////////////// 
  //  Debugging   //
  ////////////////// 

  Serial.print("; timeSunrise = ");
  Serial.print(timeSunrise);
  Serial.print("; timeSunset = ");
  Serial.print(timeSunset);
  Serial.print("; timeMinutes: ");
  Serial.println(timeMinutes);
}

void setVariables() {
  // Movement detected thrue PIR sensor
  if(digitalRead(pinMovement) == 1) {
      digitalWrite(pinLedGreen, HIGH);
      movementDetected = 1;
      movementDetectedStarttime = timestamp;
  }
  
  if(movementDetected == 1 && ((timestamp - movementDetectedStarttime)> 300)) {     // After movement detection, wait 5 minutes to reset movement detection
    digitalWrite(pinLedGreen, LOW);
    movementDetected = 0;
  }
  
  // Microswitch UP activated
  if(digitalRead(pinMicroUp) == 0) {
    microUp = 1;
    microUptime = timestamp;
  }
  if(microUp == 1 && ((timestamp - microUptime)>1 )) {
    microUp = 0;
  }
  
  // Microswitch DOWN activated
  if(digitalRead(pinMicroDown) == LOW) {
    microDown = 1;
    microDowntime = timestamp;
  }
  
  if(microDown == 1 && ((timestamp - microDowntime)>1 )) {
    microDown = 0;
  }
}