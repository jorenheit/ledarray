#ifndef LEDARRAY_H
#define LEDARRAY_H

enum LEDNames {
    RED1,
    RED2,
    YELLOW1,
    YELLOW2,
    BLUE1,
    BLUE2,
    GREEN1,
    GREEN2,
    N_LEDS
};

class LEDArray 
{
  int m_latchPin, m_clockPin, m_dataPin;
  uint8_t m_state;

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

  bool isOn(int index)
  {
    if (index < 0 || index >= N_LEDS) return false;
    return m_state & bitmaskFromIndex(index);
  }

  bool isOff(int index)
  {
    return !isOn(index);
  }
    
  template <typename ... Args>
  void on(Args ... args) 
  {
    onOff(true, args ...);
  }

  template <typename ... Args>
  void off(Args ... args) 
  {
    onOff(false, args ...);
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

  void setBitPattern(uint8_t newState)
  {
    m_state = newState;
    update();
  }

private:
  inline uint8_t bitmaskFromIndex(uint8_t index)
  {
    return (1 << N_LEDS - index - 1);
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
      if (on) m_state |= bitmaskFromIndex(leds[i]);
      else    m_state &= ~bitmaskFromIndex(leds[i]);
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
