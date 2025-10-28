#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
struct Stratagem {
  String code;
  String name;
};
Stratagem stratagems[] = { //hier nur demo attacken ja der rest ist falsch
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
struct Button { // bereich buttons
  int x, y, w, h;
  char dir;
};
Button buttons[] = {
  {40, 60, 60, 60, 'U'},   // Up
  {40, 160, 60, 60, 'D'},  // Down
  {10, 110, 60, 60, 'L'},  // Left
  {70, 110, 60, 60, 'R'}   // Right
};
Button resetBtn = {150, 180, 150, 40, 'X'};
Button activateBtn = {150, 130, 150, 40, 'A'};
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
  tft.setTextColor(TFT_YELLOW); //Text von Input feld
  tft.setTextSize(2);
  tft.setCursor(60, 5);
  tft.println("HELLDIVERS"); //ab hier eingabe Felder dings 
  drawButton(40, 60, 60, 60, "^", TFT_DARKGREEN);   // Up
  drawButton(40, 160, 60, 60, "v", TFT_DARKGREEN);  // Down
  drawButton(10, 110, 60, 60, "<", TFT_DARKGREEN);  // Left
  drawButton(70, 110, 60, 60, ">", TFT_DARKGREEN);  // Right 
  drawButton(150, 180, 150, 40, "RESET", TFT_MAROON); //reset button
  tft.drawRect(145, 30, 170, 90, TFT_YELLOW);  // Input Anzeigefeld
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(150, 35);
  tft.println("INPUT:");
  
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
  tft.fillRect(150, 50, 160, 65, TFT_BLACK);   // Clear Input Area
  tft.setTextColor(TFT_WHITE); // ab da wird aktuelle Eingabe angezeigt
  tft.setTextSize(2);
  tft.setCursor(155, 55);
  String arrows = codeToArrows(currentInput);
  if(arrows.length() > 12) { //if bed. wenn Text zu lang dass andere Zeile 
    tft.println(arrows.substring(0, 12));
    tft.setCursor(155, 75);
    tft.println(arrows.substring(12));
  } else {
    tft.println(arrows);
  }
  selectedStratagem = -1; //sobald Iput mit DB übereinstimmt siehe ab hier
  for(int i = 0; i < stratagemCount; i++) {
    if(stratagems[i].code == currentInput) {
      selectedStratagem = i;
      break;
    }
  }
  if(selectedStratagem >= 0) {  // Activate Button
    drawButton(150, 130, 150, 40, "ACTIVATE", TFT_DARKGREEN);
    // Stratagem Name anzeigen (in grüm???):
    tft.fillRect(10, 225, 300, 15, TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.setTextSize(1);
    tft.setCursor(10, 225);
    tft.println(stratagems[selectedStratagem].name);
  } else {
    tft.fillRect(150, 130, 150, 40, TFT_BLACK);
    tft.fillRect(10, 225, 300, 15, TFT_BLACK);
  }
}
void handleInput(char dir) {
  if(dir == 'X') {
    currentInput = "";     // Reset
    selectedStratagem = -1;
  } else if(dir == 'A') {
    if(selectedStratagem >= 0) {     // Activate
      tft.fillScreen(TFT_GREEN); //eigentlich Animation wenn es klappt aber ist beim testen noch nicht da gewesen 
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(3);
      tft.setCursor(30, 100);
      tft.println("ACTIVATED!");
      delay(1000);
      currentInput = "";
      selectedStratagem = -1;
      drawUI();
      return;
    }
  } else {
    currentInput += dir; // hier ist Direction input
    if(currentInput.length() > 8) {
      currentInput = currentInput.substring(1); // Max. 8 Eingaben mehr geht noch nicht 
    }
  } 
  updateDisplay();
}
void checkSerialInput() { // Button-Test via Serial, KI Vorsachlag ka was da passiert
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
void checkTouch() {
  static int lastInt = HIGH;
  static unsigned long lastTouchTime = 0;
  static int touchCounter = 0;
  int intState = digitalRead(36);
  if(intState == LOW && lastInt == HIGH) {     // Touch erkannt dann:
    unsigned long now = millis(); 
    if(now - lastTouchTime < 300) {    // Schnelles Tippen = nächster Button - jaaaaaa ausbaufähig aber vorab egal
      touchCounter++;
    } else {
      touchCounter = 0;
    }
    lastTouchTime = now;
    switch(touchCounter % 6) {     // Cycle durch Buttons: U -> R -> D -> L -> RESET -> ACTIVATE, Demo version
      case 0: handleInput('U'); break;
      case 1: handleInput('R'); break;
      case 2: handleInput('D'); break;
      case 3: handleInput('L'); break;
      case 4: handleInput('X'); break;
      case 5: handleInput('A'); break;
    }
    tft.fillCircle(300, 10, 5, TFT_YELLOW); // dat hier soll n visuelles Feedback sein 
    delay(100);
    tft.fillCircle(300, 10, 5, TFT_BLACK);
  }
  lastInt = intState;
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Helldivers Stratagem Pad ===");
  Serial.println("Nutze WASD für Richtungen, R=Reset, Space=Activate");
  Serial.println("ODER: Tippe auf Display (schnell mehrmals fuer verschiedene Buttons)");
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  pinMode(36, INPUT); // Touch INT
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
  checkSerialInput();  // Steuerung Tastatur, ich hab keuine Ahnung 
  checkTouch();        // Touch (cycle through buttons) von diesem Visualisierungs Mist siehe Z.175 abwärts
  delay(10);
}
