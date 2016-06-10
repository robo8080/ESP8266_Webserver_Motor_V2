/*
 * ESP-WROOM-2 & Adafruit Motor Shield V2 for Arduino
 * Adafruit Motor Shield V2 for Arduino 
 *  http://akizukidenshi.com/catalog/g/gK-07748/
 *  https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino
 */
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "your-ssid";
const char* password = "your-password";

String form = "<form action='led'><input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off<input type='submit' value='Submit'></form>";

// HTTP server will listen at port 80
ESP8266WebServer server(80);

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);  //Left
// You can also make another motor on port M2
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);  //Right

const int led = 13;
//const int led = 15;
 
void handle_led() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();
  
  digitalWrite(led, state);
  server.send(200, "text/plain", String("LED is now ") + ((state)?"on":"off"));
}
void handle_forward() {
  // get the value of request argument "state" and convert it to an int
  int spd = server.arg("speed").toInt();
  
  Motor1->setSpeed(spd);  // Left
  Motor2->setSpeed(spd);  // Right
  Motor1->run(FORWARD);
  Motor2->run(FORWARD);
  server.send(200, "text/plain", String("Forward : ") + "Speed = " + spd);
}
void handle_left() {
  // get the value of request argument "state" and convert it to an int
  int spd = server.arg("speed").toInt();
  
  Motor1->setSpeed(spd);  // Left
  Motor2->setSpeed(spd);  // Right
  Motor1->run(FORWARD);
  Motor2->run(BACKWARD);
  server.send(200, "text/plain", String("Left : ") + "Speed = " + spd);
}
void handle_right() {
  // get the value of request argument "state" and convert it to an int
  int spd = server.arg("speed").toInt();
  
  Motor1->setSpeed(spd);  // Left
  Motor2->setSpeed(spd);  // Right
  Motor1->run(BACKWARD);
  Motor2->run(FORWARD);
  server.send(200, "text/plain", String("Right : ") + "Speed = " + spd);
}
void handle_back() {
  // get the value of request argument "state" and convert it to an int
  int spd = server.arg("speed").toInt();
  
  Motor1->setSpeed(spd);  // Left
  Motor2->setSpeed(spd);  // Right
  Motor1->run(BACKWARD);
  Motor2->run(BACKWARD);
  server.send(200, "text/plain", String("Back : ") + "Speed = " + spd);
}
void handle_stop() {
  // get the value of request argument "state" and convert it to an int
  int spd = server.arg("speed").toInt();
  
//  digitalWrite(led, state);
  Motor1->setSpeed(0);  // Left
  Motor2->setSpeed(0);  // Right
  Motor1->run(RELEASE);
  Motor2->run(RELEASE);
  server.send(200, "text/plain", String("Stop : ") );
}
 
void setup(void) {
  Serial.begin(115200);
  Serial.println("");
  pinMode(led, OUTPUT);
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  Motor1->setSpeed(150);
  Motor2->setSpeed(150);
  Motor1->run(FORWARD);
  Motor2->run(FORWARD);
  // turn on motor
  Motor1->run(RELEASE);
  Motor2->run(RELEASE);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  // Set up the endpoints for HTTP server
  //
  // Endpoints can be written as inline functions:
  server.on("/", [](){
   server.send(200, "text/html", form);
  });
  
  // And as regular external functions:
  server.on("/led", handle_led);
  server.on("/forward", handle_forward);
  server.on("/back", handle_back);
  server.on("/left", handle_left);
  server.on("/right", handle_right);
  server.on("/stop", handle_stop);
  
  // Start the server 
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
  // check for incomming client connections frequently in the main loop:
  server.handleClient();
}


