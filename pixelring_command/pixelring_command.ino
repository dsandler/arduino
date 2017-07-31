#include "FastLED.h"
#include "SerialCommand.h"
#include "printf.h"

#define PIN 3
#define NLEDS 24
#define DELAY 20
#define HUESPEED 5
#define HUESTEP (255/NLEDS)

CRGB leds[NLEDS];
SerialCommand sCmd;

uint8_t gValue, gSpeed;

uint32_t gColor;

void do_value() {
  char *arg = sCmd.next();
  if (arg) {
    gValue = atoi(arg);
  }
  Serial.print("value=");
  Serial.println(gValue);
}

void do_help(char const *input) {
  Serial.println("commands:\n"
                 "  color <hex>\n"
                 "  rainbow\n"
                 "  speed <num>\n"
                 "  value <num>");
}

void do_speed() {
  char *arg = sCmd.next();
  if (arg) {
    gSpeed = atoi(arg);
  }
  printf("speed=%d\n", gSpeed);
}

void do_rainbow() {
  Serial.println("color=rainbow");
  gColor = 0;
}

void do_color() {
  char *arg = sCmd.next();
  if (arg) {
    if (*arg == '#') arg++;
    else if (*arg == '0' && arg[1] == 'x') arg += 2;
    gColor = (uint32_t) strtoul(arg, NULL, 16);
  }
  printf("color=0x%08lx\n", gColor);
}

void setup() {
  gValue = 255;
  gSpeed = HUESPEED;
  
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NLEDS);

  Serial.begin(9600);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("speed", do_speed);
  sCmd.addCommand("value", do_value);
  sCmd.addCommand("color", do_color);
  sCmd.addCommand("rainbow", do_rainbow);
  sCmd.setDefaultHandler(do_help);
  sCmd.setPrompt("> ");
}
void loop() {
  sCmd.readSerial();
  
  static uint8_t hue = 0;
  hue = (hue + gSpeed); // no need for %255

  for (int i=0; i<NLEDS; i++) {
    if (gColor != 0) {
      leds[i] = gColor;
    } else {
      leds[i] = CHSV((hue+i*HUESTEP), 255, gValue);
    }
  }
  FastLED.show();
  delay(DELAY);
}

