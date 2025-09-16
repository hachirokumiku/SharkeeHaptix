#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <MicroOSC.h>
#include <MicroOSCMessage.h>
#include <Adafruit_DRV2605.h>

// Sharkee's Haptic System
// Wemos D1 Mini + Adafruit DRV2605L
// OwO UwU xD

// ENTER YUOR CREDS HERE
const char* const wirelessNetwork = "Sharkee";
const char* const networkPasskey = "SharkeeOwO1234";

// LEAVE AT 9000 UNLESS YOU NEED OTHERWISE
const unsigned int hapticPort = 9000;
WiFiUDP vibeChannel;

// sET up the microOsc here
MicroOSC<4> incomingMessages;

// init haptic enging >:3c
Adafruit_DRV2605 hapticEngine;

//Blinky thingy
const int activityLED = D4;
//set up code, serial port, start the blinky.
void setup() {
  Serial.begin(115200);
  pinMode(activityLED, OUTPUT);
  digitalWrite(activityLED, HIGH);
//Set up the wiffy
  Serial.print("Attempting Handshake OwO... ");
  WiFi.begin(wirelessNetwork, networkPasskey);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(" Success UwU! ");
  }

  Serial.println("\nHandshake Complete! UwU!");
  Serial.print(" Haptic IP: ");
  Serial.println(WiFi.localIP());

  vibeChannel.begin(hapticPort);
  Serial.print("I'm all ears... ");
  Serial.println(hapticPort);

  // Initialize haptic engine or whatever.
  if (!hapticEngine.begin()) {
    Serial.println("Nothings happening! Check wiring...");
    while (1) {
      digitalWrite(activityLED, LOW);
      delay(250);
      digitalWrite(activityLED, HIGH);
      delay(250);
    }
  }

  // Set up the haptic chip...

  hapticEngine.setMode(DRV2605_MODE_INTTRIG); 
  hapticEngine.selectLibrary(1); 
  Serial.println("DRV2605L ready to rumble.");
  digitalWrite(activityLED, LOW);
  delay(1000);
  digitalWrite(activityLED, HIGH);
  // This is where we link the function to the incoming packets
  // and do stuff with them
  // so VrChat can send Buzzes.
  
  incomingMessages.add("/avatar/parameters/Haptic", [](MicroOSCMessage &msg) {
    if (msg.isFloat(0)) {
      float hapticIntensity = msg.getFloat(0);
      performHapticAction(hapticIntensity);
    }
  });
}

void loop() {
  // Check for new packets and nom on them.
  int packetSize = vibeChannel.parsePacket();
  if (packetSize > 0) {
    while (packetSize--) {
      incomingMessages.parse(vibeChannel.read());
    }
  }
}

void performHapticAction(float intensity) {
  uint8_t selectedEffect = 0;

  if (intensity > 0.0 && intensity <= 0.25) {
    selectedEffect = 25; // A little "Buzz!"
    Serial.println("Buzz!");
  } else if (intensity > 0.25 && intensity <= 0.5) {
    selectedEffect = 10; // A solid "Thump!"
    Serial.println("Thump!!");
  } else if (intensity > 0.5 && intensity <= 0.75) {
    selectedEffect = 32; // The "Thicc Buzz!"
    Serial.println("Thicc Buzz!!!");
  } else if (intensity > 0.75) {
    selectedEffect = 9; // The "OmG!!!!"
    Serial.println("OmG!!!!");
  }
  
  if (selectedEffect > 0) {
    hapticEngine.setWaveform(0, selectedEffect); 
    hapticEngine.setWaveform(1, 0);     
    hapticEngine.go();
  }
}
