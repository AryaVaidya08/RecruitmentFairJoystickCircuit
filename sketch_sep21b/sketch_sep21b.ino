#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define XPOS_PIN A1
#define YPOS_PIN A0
#define BUTTON_CLICK_PIN 2

#define NEOPIXEL_PIN 6

long int xPosition = 0;
long int yPosition = 0;
long int buttonState = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600); 

  //Joystick Setup
  pinMode(XPOS_PIN, INPUT);
  pinMode(XPOS_PIN, INPUT);
  pinMode(BUTTON_CLICK_PIN, INPUT_PULLUP);

  //Neopixel Setup
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

void readJoystickInput() {
  xPosition = analogRead(XPOS_PIN);
  yPosition = analogRead(YPOS_PIN);
  buttonState = !digitalRead(BUTTON_CLICK_PIN);
}

void printOutput() {
  Serial.print("X: "); Serial.print(xPosition);
  Serial.print(" | Y: "); Serial.print(yPosition);
  Serial.print(" | Button: "); Serial.println(buttonState);
}

void loop() {
  readJoystickInput();
  printOutput();

  int xLevel = 0;
  int yLevel = 0;
  int pinToLight = 0;

  if (xPosition <= 146) {
    xLevel = 1;
  } else if (xPosition <= 292) {
    xLevel = 2;
  } else if (xPosition <= 439) {
    xLevel = 3;
  } else if (xPosition <= 586) {
    xLevel = 4;
  } else if (xPosition <= 733) {
    xLevel = 5;
  } else if (xPosition <= 879) {
    xLevel = 6;
  }  else if (xPosition <= 1024) {
    xLevel = 7;
  }

  if (yPosition <= 146) {
    yLevel = 1;
  } else if (yPosition <= 292) {
    yLevel = 2;
  } else if (yPosition <= 439) {
    yLevel = 3;
  } else if (yPosition <= 586) {
    yLevel = 4;
  } else if (yPosition <= 733) {
    yLevel = 5;
  } else if (yPosition <= 879) {
    yLevel = 6;
  }  else if (yPosition <= 1024) {
    yLevel = 7;
  }

  if (xLevel == 1 && yLevel == 4) {
    pinToLight = 8;
  } else if (xLevel == 1 && yLevel == 3) {
    pinToLight = 9;
  } else if (xLevel == 1 && yLevel == 2) {
    pinToLight = 10;
  } else if (xLevel == 4 && yLevel == 1) {
    pinToLight = 11;
  } else if (xLevel == 5 && yLevel == 1) {
    pinToLight = 0;
  } else if (xLevel == 6 && yLevel == 1) {
    pinToLight = 1;
  } else if (xLevel == 7 && yLevel == 4) {
    pinToLight = 2;
  } else if (xLevel == 7 && yLevel == 5) {
    pinToLight = 3;
  } else if (xLevel == 7 && yLevel == 6) {
    pinToLight = 4;
  } else if (xLevel == 4 && yLevel == 7) {
    pinToLight = 5;
  } else if (xLevel == 3 && yLevel == 7) {
    pinToLight = 6;
  } else if (xLevel == 2 && yLevel == 7) {
    pinToLight = 7;
  }
  


  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.setPixelColor(pinToLight, strip.Color(255, 255, 255));
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
