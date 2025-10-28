#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  
  tft.init();
  tft.setRotation(1);
  
  tft.writecommand(0x36);
  tft.writedata(0x20);
  
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  
  tft.setCursor(10, 10);
  tft.println("Oben links");
  
  tft.setCursor(10, 110);
  tft.println("Mitte links");
  
  tft.setCursor(10, 220);
  tft.println("Unten links");
  
  tft.setCursor(200, 10);
  tft.println("Oben rechts");
  
  tft.setCursor(200, 220);
  tft.println("Unten rechts");
}

void loop() {}
