#include <Adafruit_NeoPixel.h>
#include <InputDebounce.h>

#define SERIAL_ENABLED false
#define BUTTON_PIN 3
#define BUTTON_DEBOUNCE_DELAY   20

Adafruit_NeoPixel pixels9(1, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels10(1, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels11(1, 11, NEO_GRB + NEO_KHZ800);

static const int pinLED = LED_BUILTIN;

volatile int button_mode = 0;
static InputDebounce buttonTestA;


void buttonTest_pressedCallback(uint8_t pinIn){
  digitalWrite(pinLED, HIGH); // turn the LED on
#if SERIAL_ENABLED
  Serial.print("HIGH (pin: ");
  Serial.print(pinIn);
  Serial.println(")");
#endif
}

void buttonTest_releasedCallback(uint8_t pinIn){
  digitalWrite(pinLED, LOW); // turn the LED off
#if SERIAL_ENABLED
  Serial.print("LOW (pin: ");
  Serial.print(pinIn);
  Serial.println(")");
#endif
}

void buttonTest_pressedDurationCallback(uint8_t pinIn, unsigned long duration){
#if SERIAL_ENABLED
  Serial.print("HIGH (pin: ");
  Serial.print(pinIn);
  Serial.print(") still pressed, duration ");
  Serial.print(duration);
  Serial.println("ms");
#endif
}

void buttonTest_releasedDurationCallback(uint8_t pinIn, unsigned long duration){
#if SERIAL_ENABLED
  Serial.print("LOW (pin: ");
  Serial.print(pinIn);
  Serial.print("), duration ");
  Serial.print(duration);
  Serial.println("ms");
#endif
  button_mode++;
  if(button_mode > 3){
    button_mode = 0;
  }
}

uint32_t button_mode_to_color(Adafruit_NeoPixel *p){
  if(button_mode==0)
    return p->Color(150, 0, 0);
  if(button_mode==1)
    return p->Color(0, 0, 150);
  if(button_mode==2)
    return p->Color(150, 150, 0);
  return p->Color(0, 150, 0);
}

void setup() {
#if SERIAL_ENABLED
  Serial.begin(115200);
  while (!Serial);
  Serial.println("starting");
#endif

  buttonTestA.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  buttonTestA.setup(BUTTON_PIN, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);

  pixels9.begin();
  pixels10.begin();
  pixels11.begin();
}

void loop() {
  unsigned long now = millis();
  buttonTestA.process(now);

#if SERIAL_ENABLED
  Serial.print("button mode: ");
  Serial.print(button_mode);
  Serial.println("");
#endif

  pixels9.clear(); 
  pixels10.clear(); 
  pixels11.clear(); 

  pixels9.setPixelColor(0, button_mode_to_color(&pixels9));
  pixels10.setPixelColor(0, button_mode_to_color(&pixels10));
  pixels11.setPixelColor(0, button_mode_to_color(&pixels11));
  

  pixels9.show();
  pixels10.show();
  pixels11.show();

  delay(1);
}
