/**
    Alertar cuando abre puerta (poner las luces en roja y sonido)
    Habilitar/deshabilitar alarma
    Controlar luces/tele usando wifi
    Ver estado de la puerta

    Commands
    <num_led>,<state>
    state 0 = off, 1 = on

    20,1 Alarm enable
    20,0 Alarm disable

    25,1 Prender Tele
    25,0 Apagar Tele

    Door status
    30,0 Door closed
    30,1 Door opened
*/
#include <SoftwareSerial.h>
#include <FastLED.h>
#include "effect.h"

#define FRAMES_PER_SECOND  120

#define pinPuerta   2
#define pinAlarma   9
#define pinLEDS     7

CRGB leds[13];

enum modes {
  FIRE,
  TV,
  XMASTREE,
  STROBE
};

Effect effect1(TV, leds, 9, 9, 60);

String inputString = "";
String valueString = "";
String commandString = "";
bool stringComplete = false;
bool ALARM_ENABLED = false;
int estadoTele = 0;
int estadoPuerta = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pinPuerta, INPUT_PULLUP);
  pinMode(pinAlarma, OUTPUT);
  digitalWrite(pinAlarma, LOW);

  inputString.reserve(10);
  valueString.reserve(10);
  commandString.reserve(10);

  FastLED.addLeds<WS2811, pinLEDS, RGB>(leds, 13);
  FastLED.setBrightness(200);

  Serial.println(F("Ready..."));
  for (int i = 0; i < 13; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void loop() {

  serialEvent();
  parseCommand();
  revisarPuerta();
  updateAllEffects();
  if (ALARM_ENABLED) {
    leds[5] = CRGB::Blue;
  } else {
    leds[5] = CRGB::Black;
  }
  if (ALARM_ENABLED && estadoPuerta == 1) {
    tone(pinAlarma, 1000);
    leds[12] = CRGB::Red;
    leds[3] = CRGB::Red;
    leds[8] = CRGB::Red;
    leds[10] = CRGB::Red;
  }
  FastLED.show();
}

void updateAllEffects() {
  effect1.Update(estadoTele);
}

void revisarPuerta() {
  int puerta = digitalRead(pinPuerta);
  if (puerta != estadoPuerta) {
    estadoPuerta = puerta;
    String msg = "";
    msg = "30,";
    msg += estadoPuerta;
    Serial.println(msg);
  }
}

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

void parseCommand() {
  // print the string when a newline arrives:
  if (stringComplete) {
    inputString.trim();
    Serial.println(inputString);
    int separatorIndex = inputString.indexOf(',');
    commandString = inputString.substring(0, separatorIndex);
    valueString = inputString.substring(separatorIndex + 1);
    //    handle
    int value = valueString.toInt();
    int led = commandString.toInt();
    if (led < 20) {
      leds[led] = value * CRGB::Yellow;
    }
    if (led == 20 && value == 1) {
      ALARM_ENABLED = true;
    } else if (led == 20 && value == 0) {
      ALARM_ENABLED = false;
      noTone(pinAlarma);
      leds[12] = CRGB::Black;
      leds[3] = CRGB::Black;
      leds[8] = CRGB::Black;
      leds[10] = CRGB::Black;
    }
    if (led == 25 && value == 1) {
      estadoTele = 1;
    } else if (led == 25 && value == 0) {
      estadoTele = 0;
    }
    // clear the string:
    inputString = "";
    valueString = "";
    commandString = "";
    stringComplete = false;
  }
}
