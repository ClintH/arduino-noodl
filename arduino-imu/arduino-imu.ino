// arduino-imu, based on example from Mathias Lewin @ Topp.se
#include <CurieBLE.h>
#include "CurieIMU.h"
unsigned long loopTime = 0;
unsigned long interruptsTime = 0;

// As data comes in from serial port, this
// string will act as a buffer (see serialEvent, bottom)
String inputString = "";

// This boolean flips to 'true' when a whole line is read
// from serial (see serialEvent, bottom)
boolean stringComplete = false;  

// Initial set up
void setup() {
  // Initialize serial port
  Serial.begin(9600);
  
  // Reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // Initializing IMU device
  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(4);
  CurieIMU.attachInterrupt(eventCallback);

  CurieIMU.setDetectionThreshold(CURIE_IMU_MOTION, 20);
  CurieIMU.setDetectionDuration(CURIE_IMU_MOTION, 10);
  CurieIMU.interrupts(CURIE_IMU_MOTION);
}

void loop() {  
  if (1) { // Setting this 1 to 0 can be a handy way to disable this block if necessary
    demoSendImuValues();

    delay(100); // don't flood the poor serial port
  }

  // If a complete line has been read from serial port
  if (stringComplete) {
    // eg: write the number to a servo
    //     myservo.write(inputString.toInt() );  

    // Reset buffer and flag
    inputString = "";
    stringComplete = false;
  }
}

void demoSendImuValues() {
  float ax, ay, az;
  loopTime = millis();
  if(abs(loopTime - interruptsTime) < 1000 ) {
    CurieIMU.readAccelerometerScaled(ax, ay, az);
    // Note that the keys here (ax, ay, az) are what you need to plug
    // into your Noodl project
    sendIntValue(ax*1000, "ax");  
    sendIntValue(ay*1000, "ay");  
    sendIntValue(az*1000, "az");  
  }
}

// Demonstrates reading the from the analog pin
// and writing the value to serial as 'lightlevel'
void demoSendAnalogValue() {
  int apa;

  // Read from Analog 0
  apa = analogRead(A0);

 sendIntValue(apa, "lightlevel");
}

void sendIntValue(int val, const char* key) {
  char reply[32];
  char jsonreply[128];

   // Convert number to a string, saving it to the 'reply' character array
  itoa(val, reply, 10);

  // Package up the data in MQTTT format, with an identifier,
  // and saving this to 'jsonreply' character array
  mqttify(jsonreply, key, reply);

  // Send the MQTTTified data to the serial connection
  Serial.println(jsonreply);
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

// This is a helper function to convert a single variable to a MQTTT format
void mqttify(char* destination, const char* payloadName, const char* payloadData) {
  strcpy(destination, "{\"" );
  strcat(destination, payloadName );
  strcat(destination, "\":" );
  strcat(destination, payloadData );
  strcat(destination, "}");
}

static void eventCallback(void){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_MOTION)) {
    Serial.println("motion detected");
    interruptsTime = millis();
  }
}

