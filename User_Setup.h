#define USER_SETUP_ID 2432
#define ST7789_DRIVER
#define TFT_WIDTH  320  
#define TFT_HEIGHT 480 // eigentlch 240 auf 320  UND vertauschen dann geht es wie hier

#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  12
#define TFT_BL   27 // nicht wie sonst default die 32 

#define TFT_RGB_ORDER TFT_BGR  //wg Farben oder so
#define TFT_INVERSION_OFF
#define CGRAM_OFFSET
#define TFT_COL_OFFSET 0 //ka why aber damit geht es xD
#define TFT_ROW_OFFSET 0

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT
#define SPI_FREQUENCY  27000000  // Reduziere von 40 auf 27