#include "Arduino.h"
#include <FastLED.h>
#include "effect.h"

Effect::Effect(int mode, CRGB* fastleds, int start_led, int end_led, int frame_count) {

  _fastleds = fastleds;
  _mode = mode;
  _start_led = start_led;
  _end_led = end_led;
  _frame_count = frame_count;
  _pos = _start_led;
  _hue = 0;
  _count = 0;

}

void Effect::Update(int state) {

  _state = state;

  if (_mode == 0) { // Fire
    _count++;
    if (_count > _frame_count) {
      for (int i = _start_led; i <= _end_led; i++) {
        int rVal = random8(10, 255);
        int rHue = random8(0, 50);
        _fastleds[i] = CHSV( rHue, 255, rVal * _state);
      }
      _count = 0;
    }

  }
  if (_mode == 1) { // TV
    _count++;
    if (_count > _frame_count) {
      for (int i = _start_led; i <= _end_led; i++) {
        int rVal = random8(10, 255);
        int rHue = random8(100, 164);
        _fastleds[i] = CHSV( rHue, 255, rVal * _state);
      }
      _count = 0;
    }

  }

    if (_mode == 2) { // XMASTREE
    _count++;
    if (_count > _frame_count) {
      for (int i = _start_led; i <= _end_led; i++) {
        int rVal = random8(10, 255);
        int rHue = random8();
        _fastleds[i] = CHSV( rHue, 255, rVal * _state);
      }
      _count = 0;
    }

  }

  if (_mode == 3) { // STROBE
    if (_pos > _end_led) {
      _pos = _start_led;
    }
    for (int i = _start_led; i <= _end_led; i++) {
      _fastleds[i].fadeToBlackBy(10);
    }
    _fastleds[_pos] = CHSV( _hue, 255, 192 * _state);
    _count++;
    if (_count > _frame_count) {
      _pos++;
      _count = 0;
    }

  }

}
