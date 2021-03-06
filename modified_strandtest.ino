// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 72 // Number of LEDs in strip (there are actually 71)

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4  // green boi
#define CLOCKPIN   5  // yellow boi
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}


// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 1, tail = 0, head_delta = 1, tail_delta = 1; // Index of first 'on' and 'off' pixels
uint32_t color = 0x110000;      // 'On' color (starts green) 

void loop() {

  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(100);                        // Pause 20 milliseconds (~50 FPS)

  head += head_delta;
  tail += tail_delta;
  if(head >= NUMPIXELS || head < 0) {         // Increment head index.  Off end of strip?
    head_delta = -head_delta;
    if((color >>= 8) == 0)          //  Next color (G->R->B) ... past blue now?
      color = 0x110000;             //   Yes, reset to green
  }
  if(tail >= NUMPIXELS || tail < 0)
      tail_delta = -tail_delta; // Increment, reset tail index
}
