//
// Autonomous RC Car
//
// All credit goes to CPARKTX and their Instructables guide here: https://www.instructables.com/Arduino-Powered-Autonomous-Vehicle/
//
// ARDUINO (UNO) SETUP:
// =======================
// Adafruit GPS = D7 & D8 (GPS Shield, but pins used internally) 
// Adafruit Magnetometer Adafruit_HMC5883 = I2C :  SCL (A5) &  SDA (A4)
// SD Card (D10, D11, D12, D13)
//


#include <Wire.h>                                 // used by: motor driver
#include <Adafruit_MotorShield.h>                 // motor driver
#include "utility/Adafruit_PWMServoDriver.h"      // motor driver
#include <Adafruit_Sensor.h>                      // part of mag sensor
#include <Adafruit_HMC5883_U.h>                   // mag sensor
#include <waypointClass.h>                        // custom class to manaage GPS waypoints
#include <Adafruit_GPS.h>                         // GPS
#include <SoftwareSerial.h>                       // used by: GPS
#include <math.h>                                 // used by: GPS
//#include <PString.h>                            // PString class, for "message" variable; LCD display


// Select optional features
// COMMENT OUT IF NOT DESIRED, don't just change to "NO"
#define DEBUG YES                   // debug mode; uses Serial Port, displays diagnostic information, etc. 
//#define USE_IR YES                // comment out to skip using the IR sensor/remote
//#define NO_GPS_WAIT YES           // define this for debugging to skip waiting for GPS fix


// Setup magnemeter  (compass); uses I2C
Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(12345);
sensors_event_t compass_event;


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 


// Setup motor controllers for both drive and steering (turn).   
Adafruit_DCMotor *turnMotor = AFMS.getMotor(1);
Adafruit_DCMotor *driveMotor = AFMS.getMotor(3);
#define TURN_LEFT 1
#define TURN_RIGHT 2
#define TURN_STRAIGHT 99


// Compass navigation
int targetHeading;              // where we want to go to reach current waypoint
int currentHeading;             // where we are actually facing now
int headingError;               // signed (+/-) difference between targetHeading and currentHeading
#define HEADING_TOLERANCE 5     // tolerance +/- (in degrees) within which we don't attempt to turn to intercept targetHeading


// GPS Navigation
#define GPSECHO false           // set to TRUE for GPS debugging if needed
//#define GPSECHO true           // set to TRUE for GPS debugging if needed
SoftwareSerial mySerial(8, 7);    // digital pins 7 & 8
Adafruit_GPS GPS(&mySerial);
boolean usingInterrupt = false;
float currentLat,
      currentLong,
      targetLat,
      targetLong;
int distanceToTarget,            // current distance to target (current waypoint)
    originalDistanceToTarget;    // distance to original waypoing when we started navigating to it


// Waypoints
#define WAYPOINT_DIST_TOLERANE  5   // tolerance in meters to waypoint; once within this tolerance, will advance to the next waypoint
#define NUMBER_WAYPOINTS 5          // enter the numebr of way points here (will run from 0 to (n-1))
int waypointNumber = -1;            // current waypoint number; will run from 0 to (NUMBER_WAYPOINTS -1); start at -1 and gets initialized during setup()
waypointClass waypointList[NUMBER_WAYPOINTS] = {waypointClass(30.508302, -97.832624), waypointClass(30.508085, -97.832494), waypointClass(30.507715, -97.832357), waypointClass(30.508422, -97.832760), waypointClass(30.508518,-97.832665) };


// Steering/turning 
enum directions {left = TURN_LEFT, right = TURN_RIGHT, straight = TURN_STRAIGHT} ;
directions turnDirection = straight;


// Speeds (range: 0 - 255)
#define FAST_SPEED 150
#define NORMAL_SPEED 125
#define TURN_SPEED 100
#define SLOW_SPEED 75
int speed = NORMAL_SPEED; 


// 
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) 
{
  GPS.read();
}


//
// turn interrupt on and off
void useInterrupt(boolean v) 
{
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}



void setup() 
{
  
  // turn on serial monitor 
  Serial.begin(115200);        // we need this speed for the GPS


  //
  // Start motor drives
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  driveMotor->setSpeed(NORMAL_SPEED);      
  turnMotor->setSpeed(255);                // full turn; only option with current RC car chassis


  //
  // start Mag / Compass
  if(!compass.begin())
    {
      #ifdef DEBUG
        Serial.println(F("COMPASS ERROR"));
      #endif
      loopForever();         // loop forever, can't operate without compass
    }
  

  //
  // start GPS and set desired configuration
  GPS.begin(9600);                                // 9600 NMEA default speed
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);     // turns on RMC and GGA (fix data)
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);       // 1 Hz update rate
  GPS.sendCommand(PGCMD_NOANTENNA);                // turn off antenna status info
  useInterrupt(true);                            // use interrupt to constantly pull data from GPS
  delay(1000);
      
  //
  // Wait for GPS to get signal
  #ifndef NO_GPS_WAIT
  unsigned long startTime = millis();
  while (!GPS.fix)                      // wait for fix, updating display with each new NMEA sentence received
    {
      lcd.setCursor(0, 1);
      lcd.print(F("Wait Time: "));
      lcd.print((int) (millis() - startTime) / 1000);     // show how long we have waited  
      if (GPS.newNMEAreceived())
        GPS.parse(GPS.lastNMEA());      
    } // while (!GPS.fix)
  //delay(1000);
  #endif
  


  //
  // get initial waypoint; also sets the distanceToTarget and courseToTarget varilables
  nextWaypoint();
  
} // setup()




void loop()
{      
    // Process GPS 
    if (GPS.newNMEAreceived())               // check for updated GPS information
      {                                      
        if(GPS.parse(GPS.lastNMEA()) )      // if we successfully parse it, update our data fields
          processGPS();   
      } 
  
    // navigate 
    currentHeading = readCompass();    // get our current heading
    calcDesiredTurn();                // calculate how we would optimatally turn, without regard to obstacles      
    
    // distance in front of us, move as necessary
    move();   
}




//
// Called after new GPS data is received; updates our position and course/distance to waypoint
void processGPS(void)
{
  currentLat = convertDegMinToDecDeg(GPS.latitude);
  currentLong = convertDegMinToDecDeg(GPS.longitude);
             
  if (GPS.lat == 'S')            // make them signed
    currentLat = -currentLat;
  if (GPS.lon = 'W')  
    currentLong = -currentLong; 
             
  // update the course and distance to waypoint based on our new position
  distanceToWaypoint();
  courseToWaypoint();         
  
}


int readCompass(void)
{
  compass.getEvent(&compass_event);    
  float heading = atan2(compass_event.magnetic.y, compass_event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/ 
  // Cedar Park, TX: Magnetic declination: 4° 11' EAST (POSITIVE);  1 degreee = 0.0174532925 radians
  
  #define DEC_ANGLE 0.069
  heading += DEC_ANGLE;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  return ((int)headingDegrees); 
}  // readCompass()



void calcDesiredTurn(void)
{
    // calculate where we need to turn to head to destination
    headingError = targetHeading - currentHeading;
    
    // adjust for compass wrap
    if (headingError < -180)      
      headingError += 360;
    if (headingError > 180)
      headingError -= 360;
  
    // calculate which way to turn to intercept the targetHeading
    if (abs(headingError) <= HEADING_TOLERANCE)      // if within tolerance, don't turn
      turnDirection = straight;  
    else if (headingError < 0)
      turnDirection = left;
    else if (headingError > 0)
      turnDirection = right;
    else
      turnDirection = straight;
 
}  // calcDesiredTurn()




void move(void)
{
  if (sonarDistance >= SAFE_DISTANCE)       // no close objects in front of car
  {
    if (turnDirection == straight)
      speed = FAST_SPEED;
    else
      speed = TURN_SPEED;
    driveMotor->setSpeed(speed);
    driveMotor->run(FORWARD);       
    turnMotor->run(turnDirection);
    return;
  }
}   // moveAndAvoid()
    




void nextWaypoint(void)
{
  waypointNumber++;
  targetLat = waypointList[waypointNumber].getLat();
  targetLong = waypointList[waypointNumber].getLong();
  
  if ((targetLat == 0 && targetLong == 0) || waypointNumber >= NUMBER_WAYPOINTS)    // last waypoint reached? 
  {
    driveMotor->run(RELEASE);    // make sure we stop
    turnMotor->run(RELEASE); 
    loopForever();
  }
    
   processGPS();
   distanceToTarget = originalDistanceToTarget = distanceToWaypoint();
   courseToWaypoint();
   
}  // nextWaypoint()




// returns distance in meters between two positions, both specified 
// as signed decimal-degrees latitude and longitude. Uses great-circle 
// distance computation for hypothetical sphere of radius 6372795 meters.
// Because Earth is no exact sphere, rounding errors may be up to 0.5%.
// copied from TinyGPS library
int distanceToWaypoint() 
{
  
  float delta = radians(currentLong - targetLong);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  float lat1 = radians(currentLat);
  float lat2 = radians(targetLat);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong); 
  delta = sq(delta); 
  delta += sq(clat2 * sdlong); 
  delta = sqrt(delta); 
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong); 
  delta = atan2(delta, denom); 
  distanceToTarget =  delta * 6372795; 
   
  // check to see if we have reached the current waypoint
  if (distanceToTarget <= WAYPOINT_DIST_TOLERANE)
    nextWaypoint();
    
  return distanceToTarget;
}  // distanceToWaypoint()




// returns course in degrees (North=0, West=270) from position 1 to position 2,
// both specified as signed decimal-degrees latitude and longitude.
// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
// copied from TinyGPS library
int courseToWaypoint() 
{
  float dlon = radians(targetLong-currentLong);
  float cLat = radians(currentLat);
  float tLat = radians(targetLat);
  float a1 = sin(dlon) * cos(tLat);
  float a2 = sin(cLat) * cos(tLat) * cos(dlon);
  a2 = cos(cLat) * sin(tLat) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  targetHeading = degrees(a2);
  return targetHeading;
}   // courseToWaypoint()





// converts lat/long from Adafruit degree-minute format to decimal-degrees; requires <math.h> library
double convertDegMinToDecDeg (float degMin) 
{
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}



//
// Uses 4 line LCD display to show the following information:
// LINE 1: Target Heading; Current Heading;
// LINE 2: Heading Error; Distance to Waypoint; 
// LINE 3: Sonar Distance; Speed;
// LINE 4: Memory Availalble; Waypoint X of Y;  
void updateDisplay(void)
{

  static unsigned long lastUpdate = millis();       // for controlling frequency of LCD updates
  unsigned long currentTime;

  // check time since last update
  currentTime = millis();
  if (lastUpdate > currentTime)   // check for time wrap around
    lastUpdate = currentTime;      

  if (currentTime >= lastUpdate + 500 )   // limit refresh rate
  {
    lastUpdate = currentTime;

    // line 1
    lcd.clear();
    lcd.print(F("tH= "));
    lcd.print(targetHeading, DEC);
    lcd.write(DEGREE_SYMBOL);
    lcd.print(F(" cH= "));
    lcd.print(currentHeading, DEC);
    lcd.write(DEGREE_SYMBOL);
  
    // line 2
    lcd.setCursor(0, 1);
    lcd.print(F("Err "));
    if (headingError < 0)
      lcd.write(LEFT_ARROW);
    lcd.print(abs(headingError), DEC);
    if (headingError > 0)
      lcd.write(RIGHT_ARROW);
    lcd.print(F(" Dist "));
    lcd.print(distanceToTarget, DEC);
    lcd.print(F("m "));
    #ifdef USE_GRAPHING
      lcd.write(map(distanceToTarget, 0, originalDistanceToTarget, 0, 7));    // show tiny bar graph of distance remaining
    #endif
    
    // line 3
    lcd.setCursor(0, 2);
    lcd.print(F("Snr "));
    lcd.print(sonarDistance, DEC);
    #ifdef USE_GRAPHING
      lcd.write(map(sonarDistance, 0, MAX_DISTANCE_IN, 0, 7));
    #endif
    lcd.print(F(" Spd "));
    lcd.print(speed, DEC);
    #ifdef USE_GRAPHING
      lcd.write(map(speed, 0, 255, 0, 7));
    #endif
  
    // line 4
    lcd.setCursor(0, 3);
    lcd.print(F("Mem "));
    lcd.print(freeRam(), DEC);
    lcd.print(F(" WPT "));
    lcd.print(waypointNumber + 1, DEC);
    lcd.print(F(" OF "));
    lcd.print(NUMBER_WAYPOINTS - 1, DEC);


     #ifdef DEBUG
      //Serial.print("GPS Fix:");
      //Serial.println((int)GPS.fix);
      Serial.print(F("LAT = "));
      Serial.print(currentLat);
      Serial.print(F(" LON = "));
      Serial.println(currentLong);
      //Serial.print("Waypint LAT ="); 
      //Serial.print(waypointList[waypointNumber].getLat());
      //Serial.print(F(" Long = "));
      //Serial.print(waypointList[waypointNumber].getLong());
      Serial.print(F(" Dist "));
      Serial.print(distanceToWaypoint());
      Serial.print(F("Original Dist "));
      Serial.println(originalDistanceToTarget);
      Serial.print(F("Compass Heading "));
      Serial.println(currentHeading);
      Serial.print(F("GPS Heading "));
      Serial.println(GPS.angle);
      
      //Serial.println(GPS.lastNMEA());
     
      //Serial.print(F("Sonar = "));
      //Serial.print(sonarDistance, DEC);
      //Serial.print(F(" Spd = "));
      //Serial.println(speed, DEC);
      //Serial.print(F("  Target = "));
      //Serial.print(targetHeading, DEC);
      //Serial.print(F("  Current = "));
      //Serial.print(currentHeading, DEC);
      //Serial.print(F("  Error = "));
      //Serial.println(headingError, DEC);
      //Serial.print(F("Free Memory: "));
      //Serial.println(freeRam(), DEC);
     #endif

  } //  if (currentTime >= lastUpdate + 500 )

}  // updateDisplay()  




// end of program routine, loops forever
void loopForever(void)
{
  while (1)
    ;
}





