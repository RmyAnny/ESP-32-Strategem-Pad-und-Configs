#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Stratagem Datenbank
struct Stratagem {
  String code;
  String name;
};

Stratagem stratagems[] = {
  {"U", "Reinforce"},
  {"D", "Resupply"},
  {"L", "SOS Beacon"},
  {"R", "SEAF Artillery"},
  {"UD", "Machine Gun"},
  {"UL", "Anti-Materiel Rifle"},
  {"UR", "Stalwart"},
  {"DL", "Guard Dog Rover"},
  {"UDU", "Eagle Strafing Run"},
  {"UDD", "Eagle Airstrike"},
  {"UDL", "Eagle Cluster Bomb"},
  {"UDR", "Eagle Napalm Strike"},
  {"URD", "Orbital Precision Strike"},
  {"URL", "Orbital Gas Strike"},
  {"URR", "Orbital EMS Strike"},
  {"UDLR", "Orbital 120MM Barrage"},
  {"UDUD", "Orbital 380MM Barrage"},
  {"DUUR", "Eagle 500kg Bomb"},
  {"RDLU", "Orbital Laser"},
  {"RDUL", "Orbital Railcannon"}
};

const int stratagemCount = 20;
String currentInput = "";
int selectedStratagem = -1;

// Button Farben
#define BTN_COLOR TFT_MAGENTA  // Magenta wie du wolltest

// Button Bereiche für 320x240 Display - ANGEPASST
struct Button {
  int x, y, w, h;
  char dir;
};

Button buttons[] = {
  {60, 60, 50, 50, 'U'},    // Up (neu zentriert)
  {60, 160, 50, 50, 'D'},   // Down (neu zentriert)
  {20, 110, 50, 50, 'L'},   // Left (20px vom Rand)
  {100, 110, 50, 50, 'R'}   // Right (angepasst)
};

Button resetBtn = {10, 220, 90, 30, 'X'};      // Breiter für "RESET"
Button activateBtn = {110, 220, 110, 30, 'A'}; // 10px Abstand für "ACTIVATE"

void drawButton(int x, int y, int w, int h, String label, uint16_t color) {
  tft.fillRoundRect(x, y, w, h, 8, color);
  tft.drawRoundRect(x, y, w, h, 8, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  int16_t tx = x + (w - tft.textWidth(label.c_str())) / 2;
  int16_t ty = y + (h - 24) / 2;
  tft.setCursor(tx, ty);
  tft.print(label);
}

void drawUI() {
  tft.fillScreen(TFT_BLACK);
  
  // Titel oben mittig
  tft.setTextColor(TFT_MAROON);
  tft.setTextSize(2);
  tft.setCursor(10, 8);
  tft.println("STRATAGEM PAD");
  
  // D-Pad zentriert links - ANGEPASST
  drawButton(60, 60, 50, 50, "^", BTN_COLOR);    // Up
  drawButton(60, 160, 50, 50, "v", BTN_COLOR);   // Down
  drawButton(20, 110, 50, 50, "<", BTN_COLOR);   // Left (20px vom Rand)
  drawButton(100, 110, 50, 50, ">", BTN_COLOR);  // Right
  
  // Input Box rechts - Height auf 60 geändert, verschoben
  tft.drawRect(180, 80, 135, 60, TFT_MAROON);
  tft.setTextColor(TFT_MAROON);
  tft.setTextSize(2);
  tft.setCursor(195, 85);
  tft.println("INPUT:");
  
  // Reset Button - RESET ausgeschrieben, angepasste Größe
  drawButton(10, 220, 90, 30, "RESET", TFT_MAROON);
  
  updateDisplay();
}

String codeToArrows(String code) {
  String result = "";
  for(unsigned int i = 0; i < code.length(); i++) {
    if(code[i] == 'U') result += "^ ";
    else if(code[i] == 'D') result += "v ";
    else if(code[i] == 'L') result += "< ";
    else if(code[i] == 'R') result += "> ";
  }
  return result;
}

void updateDisplay() {
  // Clear Input Area (angepasst für neue Box)
  tft.fillRect(185, 105, 125, 30, TFT_BLACK);
  
  // Zeige aktuelle Eingabe
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  
  String arrows = codeToArrows(currentInput);
  
  // Einzeilig (max ~7 Zeichen)
  tft.setCursor(190, 110);
  if(arrows.length() > 14) {
    tft.println(arrows.substring(0, 14));
  } else {
    tft.println(arrows);
  }
  
  // Match suchen
  selectedStratagem = -1;
  for(int i = 0; i < stratagemCount; i++) {
    if(stratagems[i].code == currentInput) {
      selectedStratagem = i;
      break;
    }
  }
  
  // Clear Activate Button Bereich - ANGEPASST
  tft.fillRect(110, 220, 110, 30, TFT_BLACK);
  
  // Activate Button nur bei Match - ACTIVATE + MAROON
  if(selectedStratagem >= 0) {
    drawButton(110, 220, 110, 30, "ACT", TFT_MAROON);
    
    // Stratagem Name unter Input Box
    tft.fillRect(180, 145, 135, 20, TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.setTextSize(1);
    tft.setCursor(182, 150);
    
    // Kürze langen Text
    String name = stratagems[selectedStratagem].name;
    if(name.length() > 20) {
      name = name.substring(0, 20);
    }
    tft.println(name);
  } else {
    // Clear Name Bereich
    tft.fillRect(180, 145, 135, 20, TFT_BLACK);
  }
}

void handleInput(char dir) {
  if(dir == 'X') {
    // Reset
    currentInput = "";
    selectedStratagem = -1;
  } else if(dir == 'A') {
    // Activate
    if(selectedStratagem >= 0) {
      // Success Animation
      tft.fillScreen(TFT_MAROON);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(3);
      tft.setCursor(50, 100);
      tft.println("ACTIVATED!");
      tft.setTextSize(2);
      tft.setCursor(30, 130);
      tft.println(stratagems[selectedStratagem].name);
      delay(1500);
      currentInput = "";
      selectedStratagem = -1;
      drawUI();
      return;
    }
  } else {
    // Direction input
    currentInput += dir;
    if(currentInput.length() > 8) {
      currentInput = currentInput.substring(1); // Max 8 Eingaben
    }
  }
  
  updateDisplay();
}

// Button-Test via Serial
void checkSerialInput() {
  if(Serial.available()) {
    char c = Serial.read();
    if(c == 'w' || c == 'W') handleInput('U');
    else if(c == 's' || c == 'S') handleInput('D');
    else if(c == 'a' || c == 'A') handleInput('L');
    else if(c == 'd' || c == 'D') handleInput('R');
    else if(c == 'r' || c == 'R') handleInput('X');
    else if(c == ' ') handleInput('A');
  }
}

// Touch-Simulator über INT Pin
void checkTouch() {
  static int lastInt = HIGH;
  static unsigned long lastTouchTime = 0;
  static int touchCounter = 0;
  
  int intState = digitalRead(36);
  
  if(intState == LOW && lastInt == HIGH) {
    unsigned long now = millis();
    
    if(now - lastTouchTime < 300) {
      touchCounter++;
    } else {
      touchCounter = 0;
    }
    
    lastTouchTime = now;
    
    // Cycle: U -> R -> D -> L -> RESET -> ACTIVATE
    switch(touchCounter % 6) {
      case 0: handleInput('U'); break;
      case 1: handleInput('R'); break;
      case 2: handleInput('D'); break;
      case 3: handleInput('L'); break;
      case 4: handleInput('X'); break;
      case 5: handleInput('A'); break;
    }
  }
  
  lastInt = intState;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Helldivers Stratagem Pad ===");
  Serial.println("WASD = Richtungen, R = Reset, Space = Activate");
  Serial.println("Touch = Schnell tippen zum Wechseln");
  
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  pinMode(36, INPUT);
  
  tft.init();
  tft.setRotation(1);
  tft.writecommand(0x36);
  tft.writedata(0x20);
  
  drawUI();
  
  Serial.println("\nStratagems:");
  for(int i = 0; i < stratagemCount; i++) {
    Serial.print(stratagems[i].code);
    Serial.print(" = ");
    Serial.println(stratagems[i].name);
  }
}

void loop() {
  checkSerialInput();
  checkTouch();
  delay(10);
}
