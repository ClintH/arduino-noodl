

String inputString = "";         
boolean stringComplete = false;  

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void mqttify(char* destination, const char* payloadName, const char* payloadData) {
  strcpy(destination, "{\"" );
  strcat(destination, payloadName );
  strcat(destination, "\":" );
  strcat(destination, payloadData );
  strcat(destination, "}");
}

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  myservo.attach(9);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int apa;
  char reply[32];
  char jsonreply[128];

  if (1) {
    
    apa = analogRead(A0);
    itoa(apa, reply, 10);
    mqttify(jsonreply, "lightlevel", reply);
    Serial.println(jsonreply);

    apa = analogRead(A1);
    itoa(apa, reply, 10);
    mqttify(jsonreply, "humidity", reply);
    Serial.println(jsonreply);
    delay(100); // don't flood the poor serial port
  }

  if (stringComplete) {
    myservo.write(inputString.toInt() );  
    inputString = "";
    stringComplete = false;
  }

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


