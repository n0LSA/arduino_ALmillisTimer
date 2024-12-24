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
 * @file      adri_timer.h
 *
 * @brief     main file
 * @see       https://github.com/AdriLighting
 * 
 * @author    Adrien Grellard   
 * @date      2021
 *
 */
#ifndef ADRI_TIMER_H
  #define ADRI_TIMER_H 
  #include <Arduino.h>

  /** 
  * @enum    timerMod
  * @brief   mod pour indiquer le type de timer
  *          
  * @see     https://github.com/AdriLighting
  * 
  * @author  Adrien Grellard   
  * @date    2021
  */
  enum timerMod {
    REPEAT,   /**< timer se repetent a lifinit */
    ONESHOT,  /**< timer s'executant une seule fois  */
    TURN      /**< timer s'executant le nombre de fois voulu */
  };

  /**
   * @class   adri_timer
   * 
   * @brief   class principale
   * @see     https://github.com/AdriLighting/
   * 
   * @author  Adrien Grellard 
   * @date    2021
   */  
  class adri_timer
  {
    boolean   _debug        = false;  /**<  */
    uint8_t   _pos          = 0;      /**<  */

    timerMod    _mod        = REPEAT; /**<  */

    boolean   _activate     = true;   /**<  */

    uint32_t  _duration     = 0;      /**<  */
    uint32_t  _lastTime     = 0;      /**<  */

    uint32_t  _durationMax  = 1000;   /**<  */

    uint8_t   _turn         = 0;      /**<  */
    uint8_t   _turnMax      = 3;      /**<  */

  public:

    adri_timer() {}
    adri_timer(timerMod m, const uint32_t & max, boolean activate);
    adri_timer(timerMod m, const uint32_t & max, uint8_t t, boolean activate);
    adri_timer(const uint32_t & max, boolean activate);
    adri_timer(const uint32_t & max, uint8_t t, boolean activate);

    ~adri_timer(){}

    void  pause();
    void  reset();
    void  resume();
    void  debug();

    boolean Loop();
    boolean Loop(const uint32_t & v);
    void    loop(boolean & statu);
    boolean loop();
    boolean loop(const uint32_t & v);
    boolean loop_turnStop(uint8_t m);
    boolean loop_turnStop(const uint32_t & val, uint8_t m);
    boolean loop_turnStop();
    boolean loop_stop(const uint32_t & val);
    boolean loop_stop();
    boolean loop_wait();

    void  get_remainingTime(uint32_t & v);
    void  get_mod(timerMod & v);
    void  get_turnnMax(uint8_t & val);
    
    void  set_duration(const uint32_t & val);
    void  set_durationMax(const uint32_t & val);
    void  set_turnnMax(uint8_t val);
    void  set_pos(uint8_t val);
    void  activate();
    void  activate(boolean val);
    boolean isActivate();

  };

/*
class adri_timers
  {
    uint8_t _count    = 0;
    uint8_t _countMax   = 20;
  public:
    adri_timers();
    ~adri_timers();
    void newTimer(adri_timer * ptr, uint8_t & pos);
    void loop();
  };
adri_timers * adri_timerPtrGet();
*/    
#endif