// Helldivers Stratagem Pad für ESP32-2432S032
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();

// Touch I2C Pins (GT911 oder FT6336)
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25
#define TOUCH_I2C_ADDR 0x5D  // GT911 Standard ggf. 0x38 

static const int SCREEN_W = 320;
static const int SCREEN_H = 240;
int16_t touch_x = 0; // Var f Touch
int16_t touch_y = 0;
bool touch_pressed = false;
struct Strat {
  String code;
  String name;
};
std::vector<Strat> stratDB = {
  {"U", "Reinforce"},
  {"D", "Resupply"},
  {"L", "SOS Beacon"},
  {"R", "Seaf Artillery"},
  {"UD", "Machine Gun"},
  {"UL", "Anti-Material Rifle"},
  {"UR", "Stalwart"},
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
  {"RDUL", "Orbital Railcannon Strike"}
};

lv_obj_t* labelInput;
lv_obj_t* listContainer; 
lv_obj_t* activateBtn;
lv_obj_t* activateLabel; //für UI

String currentSeq = "";

String seqToArrows(const String &s) {
  String out = "";
  for (size_t i = 0; i < s.length(); ++i) {
    char c = s.charAt(i);
    if (c == 'U') out += "↑";
    else if (c == 'D') out += "↓";
    else if (c == 'L') out += "←";
    else if (c == 'R') out += "→";
    if (i + 1 < s.length()) out += " ";
  }
  return out; //Input wird zu Arrows umgew.
}
bool readTouch() {
  Wire.beginTransmission(TOUCH_I2C_ADDR);
  Wire.write(0x81);
  Wire.write(0x4E);
  if (Wire.endTransmission() != 0) return false;
  
  Wire.requestFrom(TOUCH_I2C_ADDR, 1);
  if (!Wire.available()) return false;
  
  uint8_t status = Wire.read();
  if (!(status & 0x80)) return false; //lesen von touch
  
  Wire.beginTransmission(TOUCH_I2C_ADDR);
  Wire.write(0x81);
  Wire.write(0x50);
  Wire.endTransmission();
  
  Wire.requestFrom(TOUCH_I2C_ADDR, 6);
  if (Wire.available() >= 6) {
    uint8_t xl = Wire.read();
    uint8_t xh = Wire.read();
    uint8_t yl = Wire.read();
    uint8_t yh = Wire.read();
    Wire.read(); // skip
    Wire.read(); // skip
    
    touch_x = (xh << 8) | xl;
    touch_y = (yh << 8) | yl;
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(0x81);
    Wire.write(0x4E);
    Wire.write(0x00);
    Wire.endTransmission(); //status wieder löschen
    
    return true;
  }
  return false;
}
static void updateMatches();
static void activate_cb(lv_event_t* e) {
  lv_obj_t* toast = lv_label_create(lv_scr_act());
  lv_label_set_text(toast, "STRATAGEM AKTIVIERT!");
  lv_obj_set_style_text_color(toast, lv_color_hex(0xFFD700), 0);
  lv_obj_align(toast, LV_ALIGN_CENTER, 0, 0);

  lv_timer_t* t = lv_timer_create([](lv_timer_t* timer){
    lv_obj_t* tlabel = (lv_obj_t*) timer->user_data;
    if(lv_obj_is_valid(tlabel)) lv_obj_del(tlabel);
    currentSeq = "";
    lv_label_set_text(labelInput, "");
    updateMatches();
    lv_timer_del(timer);
  }, 2000, toast); //timer irgendwie
}
static void arrow_cb(lv_event_t* e) {
  lv_obj_t* btn = lv_event_get_target(e);
  const char* dir = (const char*) lv_obj_get_user_data(btn);
  if(dir) {
    currentSeq += String(dir);
    lv_label_set_text(labelInput, seqToArrows(currentSeq).c_str());
    updateMatches();
  }
}
static void reset_cb(lv_event_t* e) {
  currentSeq = "";
  lv_label_set_text(labelInput, "");
  updateMatches();
}
std::vector<Strat> getMatches(const String& seq) {
  std::vector<Strat> out;
  for (auto &s : stratDB) {
    if (seq.length() == 0) {
      out.push_back(s);
    } else {
      if (s.code.startsWith(seq)) out.push_back(s);
    }
  }
  return out;
} //Versuch eines Filters
static void updateMatches() {
  lv_obj_clean(listContainer);
  auto matches = getMatches(currentSeq);
  for (size_t i = 0; i < matches.size() && i < 8; ++i) {
    lv_obj_t* row = lv_obj_create(listContainer);
    lv_obj_set_size(row, LV_PCT(98), 28);
    lv_obj_set_style_bg_color(row, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(row, 1, 0);
    lv_obj_set_style_border_color(row, lv_color_hex(0x444444), 0);
    lv_obj_set_style_radius(row, 4, 0);
    lv_obj_set_style_pad_all(row, 4, 0);

    lv_obj_t* lbl = lv_label_create(row);
    lv_label_set_text(lbl, matches[i].name.c_str());
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 4, 0);

    if (matches[i].code == currentSeq) {
      lv_obj_set_style_border_color(row, lv_color_hex(0xFFD700), 0);
      lv_obj_set_style_border_width(row, 2, 0);
      lv_obj_set_style_bg_color(row, lv_color_hex(0x3A3A00), 0);
      lv_obj_clear_flag(activateBtn, LV_OBJ_FLAG_HIDDEN);
      lv_label_set_text(activateLabel, matches[i].name.c_str());
    }
  }
  bool exact = false;
  for (auto &m : matches) if (m.code == currentSeq) exact = true;
  if (!exact) lv_obj_add_flag(activateBtn, LV_OBJ_FLAG_HIDDEN);
}
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
  int32_t w = (area->x2 - area->x1 + 1);
  int32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp); //displayyyy
}
bool my_touch_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data) {
  if (readTouch()) { // Koordinaten für Rotation 1 anpassen
    data->point.x = map(touch_x, 0, 240, 0, 320);
    data->point.y = map(touch_y, 0, 320, 0, 240);
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  return false;
}
void setup_lvgl() {
  lv_init(); //lvgl setup
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH); //init dp
  tft.begin();
  tft.setRotation(1);
  tft.writecommand(0x36);
  tft.writedata(0x20);
  tft.fillScreen(TFT_BLACK);
  static lv_color_t buf[SCREEN_W * 10];
  static lv_disp_draw_buf_t draw_buf;
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_W * 10);
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = SCREEN_W;
  disp_drv.ver_res = SCREEN_H;
  lv_disp_drv_register(&disp_drv);
  Wire.begin(TOUCH_SDA, TOUCH_SCL); //touch init
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(10);
  digitalWrite(TOUCH_RST, HIGH);
  delay(10);
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touch_read;
  lv_indev_drv_register(&indev_drv);
}

// ab hier richtige GUI
void create_gui() {
  lv_obj_t* scr = lv_scr_act();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x0A0A0A), 0);
  lv_obj_t* title = lv_label_create(scr);  // Überschrift
  lv_label_set_text(title, "HELLDIVERS STRATAGEM");
  lv_obj_set_style_text_color(title, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 4);
  labelInput = lv_label_create(scr); // Input fürs Lable 
  lv_label_set_text(labelInput, "");
  lv_obj_set_style_text_font(labelInput, &lv_font_montserrat_20, 0);
  lv_obj_set_style_text_color(labelInput, lv_color_hex(0xFFFFFF), 0);
  lv_obj_align(labelInput, LV_ALIGN_TOP_MID, 0, 26);
  lv_obj_t* dpad = lv_obj_create(scr); // Container (Pad)
  lv_obj_set_size(dpad, 130, 170);
  lv_obj_align(dpad, LV_ALIGN_LEFT_MID, 4, 10);
  lv_obj_set_style_bg_color(dpad, lv_color_hex(0x1A1A1A), 0);
  lv_obj_set_style_border_width(dpad, 2, 0);
  lv_obj_set_style_border_color(dpad, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_radius(dpad, 8, 0);
  const int bsize = 44;

  // erster Button UP (Pfeil nach oben):
  lv_obj_t* btnUp = lv_btn_create(dpad);
  lv_obj_set_size(btnUp, bsize, bsize);
  lv_obj_align(btnUp, LV_ALIGN_TOP_MID, 0, 10);
  lv_obj_set_user_data(btnUp, (void*)"U");
  lv_obj_add_event_cb(btnUp, arrow_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btnUp, lv_color_hex(0x444444), 0);
  lv_obj_set_style_border_color(btnUp, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_border_width(btnUp, 2, 0);
  lv_obj_t* lblUp = lv_label_create(btnUp);
  lv_label_set_text(lblUp, LV_SYMBOL_UP);
  lv_obj_center(lblUp);

  // zweiter Button anch unten down:
  lv_obj_t* btnDown = lv_btn_create(dpad);
  lv_obj_set_size(btnDown, bsize, bsize);
  lv_obj_align(btnDown, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_obj_set_user_data(btnDown, (void*)"D");
  lv_obj_add_event_cb(btnDown, arrow_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btnDown, lv_color_hex(0x444444), 0);
  lv_obj_set_style_border_color(btnDown, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_border_width(btnDown, 2, 0);
  lv_obj_t* lblDown = lv_label_create(btnDown);
  lv_label_set_text(lblDown, LV_SYMBOL_DOWN);
  lv_obj_center(lblDown);

  // Pfeil nach links Button:
  lv_obj_t* btnLeft = lv_btn_create(dpad);
  lv_obj_set_size(btnLeft, bsize, bsize);
  lv_obj_align(btnLeft, LV_ALIGN_LEFT_MID, 10, 0);
  lv_obj_set_user_data(btnLeft, (void*)"L");
  lv_obj_add_event_cb(btnLeft, arrow_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btnLeft, lv_color_hex(0x444444), 0);
  lv_obj_set_style_border_color(btnLeft, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_border_width(btnLeft, 2, 0);
  lv_obj_t* lblLeft = lv_label_create(btnLeft);
  lv_label_set_text(lblLeft, LV_SYMBOL_LEFT);
  lv_obj_center(lblLeft);

  // Right Pfeil Button:
  lv_obj_t* btnRight = lv_btn_create(dpad);
  lv_obj_set_size(btnRight, bsize, bsize);
  lv_obj_align(btnRight, LV_ALIGN_RIGHT_MID, -10, 0);
  lv_obj_set_user_data(btnRight, (void*)"R");
  lv_obj_add_event_cb(btnRight, arrow_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btnRight, lv_color_hex(0x444444), 0);
  lv_obj_set_style_border_color(btnRight, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_border_width(btnRight, 2, 0);
  lv_obj_t* lblRight = lv_label_create(btnRight);
  lv_label_set_text(lblRight, LV_SYMBOL_RIGHT);
  lv_obj_center(lblRight);

  // Reset Button für EIngabe deleten:
  lv_obj_t* resetBtn = lv_btn_create(scr);
  lv_obj_set_size(resetBtn, 90, 32);
  lv_obj_align(resetBtn, LV_ALIGN_BOTTOM_LEFT, 24, -8);
  lv_obj_add_event_cb(resetBtn, reset_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(resetBtn, lv_color_hex(0x660000), 0);
  lv_obj_t* rlbl = lv_label_create(resetBtn);
  lv_label_set_text(rlbl, "RESET");
  lv_obj_center(rlbl);

  
  listContainer = lv_obj_create(scr); //weiterer Container
  lv_obj_set_size(listContainer, 172, 170);
  lv_obj_align(listContainer, LV_ALIGN_RIGHT_MID, -4, 10);
  lv_obj_set_style_bg_color(listContainer, lv_color_hex(0x121212), 0);
  lv_obj_set_style_border_width(listContainer, 2, 0);
  lv_obj_set_style_border_color(listContainer, lv_color_hex(0xFFD700), 0);
  lv_obj_set_style_radius(listContainer, 8, 0);
  lv_obj_set_flex_flow(listContainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(listContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(listContainer, 4, 0);

  activateBtn = lv_btn_create(scr);   // Button fürs "Aktivieren"
  lv_obj_set_size(activateBtn, 140, 36);
  lv_obj_align(activateBtn, LV_ALIGN_BOTTOM_RIGHT, -8, -8);
  lv_obj_add_event_cb(activateBtn, activate_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(activateBtn, lv_color_hex(0x006600), 0);
  lv_obj_add_flag(activateBtn, LV_OBJ_FLAG_HIDDEN);
  activateLabel = lv_label_create(activateBtn);
  lv_label_set_text(activateLabel, "ACTIVATE");
  lv_obj_center(activateLabel);
  updateMatches();
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Strategem Pad made by Anny is starting (^-^)");

  setup_lvgl();
  create_gui();
  
  Serial.println("readyyyyyy");
}

void loop() {
  lv_timer_handler();
  delay(5);
}
