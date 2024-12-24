/*
    MIT License

    Copyright (c) 2021 Adrien Grellard

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
/**
 * @file      adri_timer.cpp
 *
 * @brief     main file
 * @see       https://github.com/AdriLighting
 * 
 * @author    Adrien Grellard   
 * @date      2021
 *
 */
#include "adri_timer.h"

namespace {
  void millis2time(const uint32_t & s, char * time) {
    uint32_t seconds    = (s /   (1000)           ) % 60    ;
    uint32_t minutes    = (s /   (1000UL*60UL)    ) % 60    ;
    uint32_t hours      = (s /   (1000UL*3600UL)  ) % 24    ;
    uint32_t days       = (s /   (1000UL*3600UL*24UL)  )    ;
    sprintf(time,"%lu-%02lu:%02lu:%02lu", (unsigned long)days, (unsigned long)hours , (unsigned long)minutes, (unsigned long)seconds);
  }
  String on_time() {
    char t[100];
    uint32_t offset = 0; //1000 * 60 * 60 * 24 * 3 ;
    uint32_t ms=millis()+offset;
    millis2time(ms, t);
    return String(t);
  } 
}

// adri_timers * _adri_timerPtr = nullptr;

adri_timer::adri_timer(timerMod mod, const uint32_t & max, boolean activate){
  _mod          = mod;
  _activate     = activate;
  _durationMax  = max;
  _duration     = millis();
  /*if (_adri_timerPtr!=nullptr) _adri_timerPtr->newTimer(this, _pos) ;*/
}
adri_timer::adri_timer(timerMod mod, const uint32_t & max, uint8_t t, boolean activate){
  _mod          = mod;
  _activate     = activate;
  _durationMax  = max;
  _turnMax      = t;
  _duration     = millis();
  /*if (_adri_timerPtr!=nullptr) _adri_timerPtr->newTimer(this, _pos) ;*/
}
adri_timer::adri_timer(const uint32_t & max, boolean activate){
  _activate   = activate;
  _durationMax  = max;
  _duration   = millis();
  /*if (_adri_timerPtr!=nullptr) _adri_timerPtr->newTimer(this, _pos) ;*/
}
adri_timer::adri_timer(const uint32_t & max, uint8_t t, boolean activate){
  _activate     = activate;
  _durationMax  = max;
  _turnMax      = t;
  _duration     = millis();
  /*if (_adri_timerPtr!=nullptr) _adri_timerPtr->newTimer(this, _pos) ;*/
}



boolean adri_timer::Loop(const uint32_t & v){
  switch (_mod) {
    case REPEAT:  return loop(v);                     break;
    case ONESHOT: return loop_stop(v);                break;
    case TURN:    return loop_turnStop(v, _turnMax);  break;
    default:      return loop(v);                     break;
  }
  return false;
}
boolean adri_timer::Loop(){
  switch (_mod) {
    case REPEAT:  return loop();          break;
    case ONESHOT: return loop_stop();     break;
    case TURN:    return loop_turnStop(); break;
    default:      return loop();          break;
  }
  return false;
}

void adri_timer::debug(){
  if (!_debug) return;
  String time = on_time();
  
  #if defined (__AVR__)
    char buffer[100];
    sprintf(buffer, "%s [pos: %d]\n", time.c_str(), _pos);
    Serial.print(buffer);
  #elif defined (__XTENSA__)
    Serial.printf_P(PSTR("%s [pos: %d]\n"), time.c_str(), _pos);
  #else

  #endif

}
void adri_timer::loop(boolean & statu){
  if (!_activate) {statu = false; return;}

  uint32_t  tNow = millis();
  if ((tNow - _duration)>= _durationMax) {
    do {
      _duration += _durationMax;
      if (_duration < _durationMax) break;      
    } while (_duration < tNow - _durationMax);  
    statu = true; 
    debug();
    return;
  }
  statu = false;
}
boolean adri_timer::loop(){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ((tNow - _duration) >= _durationMax) {
    do {
      _duration += _durationMax;
      if (_duration < _durationMax) break;      
    } while (_duration < tNow - _durationMax);  
    debug();    
    return true;
  }
  return false; 
}
boolean adri_timer::loop(const uint32_t & v){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ( (tNow - _duration) >= v) {
    do {
      _duration += v;
      if (_duration < v) break;     
    } while (_duration < tNow - v); 
    debug();
    return true;
  }
  return false; 
}

boolean adri_timer::loop_turnStop(){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ((tNow - _duration) >= _durationMax) {
    do {
      _duration += _durationMax;
      if (_duration < _durationMax) break;      
    } while (_duration < tNow - _durationMax);  
    _turn++;
    if (_turn >= _turnMax) {
      _turn = 0;
      _activate = false;
    }   
    debug();
    return true;
  }
  return false; 
}
boolean adri_timer::loop_turnStop(uint8_t turnMax){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ((tNow - _duration) >= _durationMax) {
    do {
      _duration += _durationMax;
      if (_duration < _durationMax) break;      
    } while (_duration < tNow - _durationMax);  
    _turn++;
    if (_turn >= turnMax) {
      _turn = 0;
      _activate = false;
    }   
    debug();
    return true;
  }
  return false; 
}
boolean adri_timer::loop_turnStop(const uint32_t & val, uint8_t turnMax){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ((tNow - _duration) >= val) {
    do {
      _duration += val;
      if (_duration < val) break;      
    } while (_duration < tNow - val);  
    _turn++;
    if (_turn >= turnMax) {
      _turn = 0;
      _activate = false;
    }   
    debug();
    return true;
  }
  return false; 
}

boolean adri_timer::loop_stop(){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ((tNow - _duration) >= _durationMax) {
    do {
      _duration += _durationMax;
      if (_duration < _durationMax) break;      
    } while (_duration < tNow - _durationMax);      
    _activate = false;
    debug();
    return true;
  }
  return false; 
}
boolean adri_timer::loop_stop(const uint32_t & val){
  if (!_activate) return false;

  uint32_t tNow = millis();
  if ((tNow - _duration) >= val) {
    do {
      _duration += val;
      if (_duration < val) break;     
    } while (_duration < tNow - val);     
    _activate = false;
    debug();
    return true;
  }
  return false; 
}

boolean adri_timer::loop_wait(){
  if (!_activate) return false;

  if ((millis() - _duration) > _durationMax) {
    return true;
  }
  return false; 
}



boolean adri_timer::isActivate()                  {return _activate;}
void adri_timer::get_remainingTime(uint32_t & v)  {v = (millis() - _duration);}
void adri_timer::get_mod(timerMod & v)            {v = _mod;}
void adri_timer::get_turnnMax(uint8_t & v)        {v = _turnMax;}

void adri_timer::set_pos(uint8_t val)                   {_pos         = val;}
void adri_timer::set_turnnMax(uint8_t val)              {_turnMax     = val;}
void adri_timer::set_durationMax(const uint32_t & val)  {_durationMax = val;}
void adri_timer::set_duration(const uint32_t & val)     {_duration    = val;}
void adri_timer::activate(boolean val){
  if (val) {
    reset();
  } else {
    _lastTime =  (millis() - _duration);
    _duration = 0;
  }
  _activate = val;
}
void adri_timer::activate(){
  reset();
  _activate = true;

}
void adri_timer::pause(){
  _lastTime =  (millis() - _duration);
  _activate = false;
}
void adri_timer::reset() {
  _duration = millis();
}
void adri_timer::resume() {
  activate();
  _duration -= _lastTime;
}





/*
adri_timer * _timersArray[20];
adri_timers::adri_timers(){
  _adri_timerPtr = this;
  for(int i=0; i< _countMax; i++){
    _timersArray[i] = nullptr;  
  }
}
void adri_timers::newTimer(adri_timer * ptr, uint8_t & pos) {
  for(int i=0; i< _countMax; i++){
    if (_timersArray[i] == nullptr){
      _timersArray[i] = ptr; 
      pos = i;
      Serial.println(pos);
      break;
    }  
  }
}
void adri_timers::loop() {
  for(int i=0; i< _countMax; i++){
    if (_timersArray[i] != nullptr) {
      _timersArray[i]->Loop();
    }
  }
}
adri_timers * adri_timerPtrGet(){return _adri_timerPtr;};
*/