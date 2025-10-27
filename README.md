# ESP-32-Strategem-Pad-und-Configs
Config for ESP 32 (dev mode) with chip CH340C
✅ User_Setup.h:

Width: 320, Height: 480 (obwohl physisch 320x240!)
Backlight Pin: 27 (nicht 32!)
MOSI: 13, SCLK: 14 (nicht 23/18!)
RST: 12

✅ Im Code:

tft.setRotation(1) für Querformat
tft.writecommand(0x36); tft.writedata(0x20); für richtige Orientierung
Koordinaten wie bei 320x240 verwenden
