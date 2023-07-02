// WLED
#include <FastLED.h>
int fadeAmount = 5; // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
int brightness = 0;
int speedDelay = 25;
int glow_red = 0;
int glow_green = 0;
int glow_blue = 0;

#define LED_PIN 15
#define NUM_LEDS 12
#define BRIGHTNESS 64
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
