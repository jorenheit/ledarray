#include "ledarray.h"

/*
  Stap 1:   Aansluiten
  
      Specificeer hieronder welke pins op de Arduino je hebt verbonden
      met de data-, latch- en clock-pin van het LED-bordje.
      Hieronder is de pin-layout van het bord weergegeven: 

      GND  DATA

      x    LATCH

      5V   CLOCK

      De GND en 5V moet je per se verbinden met de 5V en GND
      pinnen op de Arduino. De anderen kun je in principe met 
      alle digitale pins verbinden (2-13). In dit voorbeeld zijn
      ze met pins 2, 3 en 4 verbonden.
*/

#define DATA 4
#define LATCH 3
#define CLOCK 2

LEDArray leds(LATCH, CLOCK, DATA);

/* 
  Stap 2: Programmeren!

      Er is nu een "leds" object voor je aangemaakt. Dit object stelt
      het bordje voor. Je kunt nu commando's naar het bordje sturen
      om LEDs aan/uit te zetten, of in/uit te laten faden.

      Om alle LEDs tegelijk aan/uit te zetten gebruik je de on() en
      off() functies:

      leds.on() -> zet alle LEDs aan
      leds.off() -> zet alle LEDs uit
      
      Om individuele LEDs aan/uit te zetten, geef je deze functies argumenten.
      Je kunt de LEDs bij naam noemen, of bij nummer. De eerste rode LED is
      LED 0, de laatste groene LED is LED 7. Het alternatief is dat je de 
      namen gebruikt: RED1, RED2, YELLOW1, YELLOW2, BLUE1, BLUE2, GREEN1 en
      GREEN 2.

      leds.on(0, 7)          -> zet de eerste en laatste led AAN
      leds.on(RED1, GREEN2)  -> doet hetzelfde als hierboven
      leds.off(0, 7)         -> zet de eerste en laatste led UIT
      leds.off(RED1, GREEN2) -> doet hetzelfde als hierboven

      In- en uitfaden werkt op dezelfde manier. Als je geen LEDs
      als argumenten meegeeft, reageren alle LEDs tegelijk. Wel
      moet je aangeven hoe lang het faden ongeveer moet duren (in sec).
      Deze tijd is niet heel accuraat, maar geeft je wel de mogelijkheid
      om ongeveer te bepalen hoe snel het gaat.

      leds.fadeIn(2.0)   -> alle LEDs faden IN over een periode van 2 seconden
      leds.fadeOut(2.0)  -> alle LEDs faden UIT over een periode van 2 seconden
      leds.fadeIn(0.5, YELLOW1, YELLOW2) -> alleen de gele LEDs over een halve seconde

      Tot slot is er nog de blink-functie. Hiermee laat je de LEDs
      knipperen. Hij moet weten hoe vaak de LEDs moeten knipperen en 
      hoeveel milliseconden de knippers duren.

      leds.blink(5, 100)   -> alle LEDs knipperen met 100ms tussenpozen
      leds.blink(10, 1000, GREEN1)

*/ 

void setup() {

}

void loop() {
  // Voorbeeld-code van een willekeurig patroon

  leds.fadeIn(2.0);
  leds.blink(5, 100);
  leds.fadeOut(2.0);

  // Een for-loop kun je gebruiken om alle leds automatisch
  // bij langs te gaan.
  for (int i = 0; i != 8; ++i) {
    leds.on(i);
    delay(100);
  }

  // Of in omgekeerde volgorde
  for (int i = 0; i != 8; ++i) {
    leds.off(7 - i);
    delay(100);
  }
}


