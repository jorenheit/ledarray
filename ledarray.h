#ifndef LEDARRAY_H
#define LEDARRAY_H

#define N_LEDS 8
using State = uint8_t;
static_assert(sizeof(State) * 8 >= N_LEDS, "State type is not large enough to encode all the LEDs.");

#define RED1    0
#define RED2    1
#define YELLOW1 2
#define YELLOW2 3
#define BLUE1   4
#define BLUE2   5
#define GREEN1  6
#define GREEN2  7

class LEDArray 
{
  int m_latchPin, m_clockPin, m_dataPin;
  State m_state;

public:
  LEDArray(int latch, int clock, int data):
    m_latchPin(latch),
    m_clockPin(clock),
    m_dataPin(data),
    m_state(0)
    {
      pinMode(m_latchPin, OUTPUT);
      pinMode(m_clockPin, OUTPUT);      
      pinMode(m_dataPin, OUTPUT);
    }

  template <typename ... Args>
  void on(Args ... args) 
  {
    onOff(true, args ...);
    update();
  }

  template <typename ... Args>
  void off(Args ... args) 
  {
    onOff(false, args ...);
    update();
  }

  template <typename ... Args>
  void fadeIn(float seconds, Args ... args)
  {
    fadeInOut(true, seconds, args...);
  }

  template <typename ... Args>
  void fadeOut(float seconds, Args ... args)
  {
    fadeInOut(false, seconds, args...);
  }

  template <typename ... Args>
  void blink(int count, int mill, Args ... args)
  {
    for (int i = 0; i != count; ++i) {
      on(args ...);
      delay(mill);
      off(args ...);
      delay(mill);
    }
  }

  void setBitPattern(State newState)
  {
    m_state = newState;
    update();
  }

private:
  bool isOn(int index) {
    if (index < 0 || index >= N_LEDS) return false;
    return m_state & (1 << N_LEDS - index - 1);
  }

  bool isOff(int index) {
    return !isOn(index);
  }

  void update()
  {
    digitalWrite(m_latchPin, LOW);
    shiftOut(m_dataPin, m_clockPin, LSBFIRST, m_state);
    digitalWrite(m_latchPin, HIGH);
  }

  template <typename ... Args>
  void onOff(bool on, Args ... args)
  {
    if constexpr (sizeof ... (args) == 0) {
      m_state = on ? -1 : 0;
      update();
      return;
    }

    int const leds[] = {args ...};
    for (int i = 0; i != sizeof ... (args); ++i) {
      int const ledIndex = leds[i];
      if (ledIndex < 0 || ledIndex >= N_LEDS) continue; // ignore out of bounds indices
      if (on) m_state |= (1 << (N_LEDS - leds[i]) - 1);
      else    m_state &= ~(1 << (N_LEDS - leds[i]) - 1);
    }
    update();
  }

  template <typename ... Args>
  void fadeInOut(bool in, float seconds, Args ... args)
  {
    int const stepsPerSecond = 5;
    int const steps = max(10, stepsPerSecond * seconds);
    float brightness = in ? 0.0 : 1.0;
    for (int i = 0; i != steps; ++i) {
      onBrightness(brightness, (seconds * 1000) / steps, args ...);
      brightness += (1.0 / steps) * (in ? 1 : -1);
    }
  }

  template <typename ... Args>
  void onBrightness(float brightness, unsigned long duration, Args ... args)
  {
    int const delayRange = 25;
    brightness = brightness > 0 ? min(1, brightness) : 0;
    unsigned long const startTime = millis();

    if (brightness > 0) {
      while (millis() - startTime < duration)
      {
        on(args ...);
        delay(1);
        delay(brightness * delayRange);
        off(args ...);
        delay((1 - brightness) * delayRange);
      }
    }
    else
    {
      off(args ...);
      while (millis() - startTime < duration)
      {}
    }
  }
};




#endif // LEDARRAY_H