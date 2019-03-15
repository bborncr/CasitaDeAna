/*
 * Create independent groups of effects using the FastLED library
 * 
 */
 
#ifndef EFFECT_H
#define EFFECT_H

#include "Arduino.h"
#include <FastLED.h>

class Effect {

  public:

    Effect(int mode, CRGB* fastleds, int start_led, int end_led, int frame_count=20);

    void Update(int state);

  private:

    CRGB* _fastleds;
    int _mode, _start_led, _end_led, _frame_count;
    int _count;
    int _pos;
    int _hue;
    int _state;
};

#endif
