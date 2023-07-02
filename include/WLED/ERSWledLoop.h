  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(glow_red, glow_green, glow_blue);
    leds[i].fadeLightBy(brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255)
  {
    fadeAmount = -fadeAmount;
  }
  delay(speedDelay);
