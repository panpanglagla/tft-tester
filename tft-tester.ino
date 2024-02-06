#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
#include "images.h"
#include <math.h>

#define TFT_CS D10
#define TFT_DC D9
#define TFT_BL D2

Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

float progress = 0;

const int16_t radius1Back = 100;
const int16_t radius2Back = 80;
const int16_t radius1Front = 93;
const int16_t radius2Front = 87;

int16_t cx;
int16_t cy;

const int numCharacters = 6;
int character = 0;

float step = PI / 180;
float start = 7 * PI / 8;
float end = 1 * PI / 8;

uint16_t SLACK_AUBERGINE = 0x48A9;
uint16_t SLACK_GREEN = 0x2DAF;
uint16_t SLACK_BLUE=0x363E; // 54 197 240
uint16_t SLACK_RED=0xE0EB; // 224 30 90
uint16_t SLACK_YELLOW=0xED85;
uint16_t SLACK_HORCHATA=0xF77D;
uint16_t SLACK_WHITE=0xFFFF;
uint16_t SLACK_BLACK=0x18E3;

uint16_t SLACK_START_R=54;
uint16_t SLACK_START_G=197;
uint16_t SLACK_START_B=240;

uint16_t SLACK_END_R=224;
uint16_t SLACK_END_G=30;
uint16_t SLACK_END_B=90;

uint16_t gradient(int percent)
{
    uint8_t r, g, b;
    r = map(percent, 0, 100, SLACK_START_R, SLACK_END_R);
    g = map(percent, 0, 100, SLACK_START_G, SLACK_END_G);
    b = map(percent, 0, 100, SLACK_START_B, SLACK_END_B);
    return tft.color565(r, g, b);
}

void setup() {
  Serial.begin(9600);
  Serial.println("GC9A01A Test!");

  tft.begin();

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  cx = tft.width() / 2;
  cy = tft.height() / 2;

  tft.fillScreen(SLACK_AUBERGINE);

  delay(2000);

  drawArc(start, end, step, SLACK_BLACK, radius1Back + 2, radius2Back - 2);
  drawArcGradient(start, end, step, radius1Back, radius2Back);
  
  delay(500);
}

void drawRect(int16_t a1x, int16_t a1y, int16_t a2x, int16_t a2y, int16_t b1x, int16_t b1y, int16_t b2x, int16_t b2y, uint16_t color) {
  tft.fillTriangle(a1x, a1y, a2x, a2y, b1x, b1y, color);
  tft.fillTriangle(a2x, a2y, b1x, b1y, b2x, b2y, color);
}

void drawArc(float start, float end, float step, uint16_t color, uint16_t radius1, uint16_t radius2) {
  int a1x;
  int a1y;
  int a2x;
  int a2y;
  int b1x;
  int b1y;
  int b2x;
  int b2y;
  float angle;

  int startx = cx + cos(start) * 0.5 * (radius1 + radius2);
  int starty = cy + sin(start) * 0.5 * (radius1 + radius2);
  int endx = cx + cos(end) * 0.5 * (radius1 + radius2);
  int endy = cy + sin(end) * 0.5 * (radius1 + radius2);

  tft.fillCircle(startx, starty, 0.5 * (radius1 - radius2), color);
  tft.fillCircle(endx, endy, 0.5 * (radius1 - radius2), color);

  tft.fillCircle(startx, starty, 0.5 * (radius1 - radius2), color);
  tft.fillCircle(endx, endy, 0.5 * (radius1 - radius2), color);

  for (float angle = start; angle > end - step; angle -= step) {
    a1x = cx + cos(angle) * radius1;
    a1y = cy + sin(angle) * radius1;
    a2x = cx + cos(angle) * radius2;
    a2y = cy + sin(angle) * radius2;
    b1x = cx + cos(angle - step) * radius1;
    b1y = cy + sin(angle - step) * radius1;
    b2x = cx + cos(angle - step) * radius2;
    b2y = cy + sin(angle - step) * radius2;
    drawRect(a1x, a1y, a2x, a2y, b1x, b1y, b2x, b2y, color);
  }
  int end1x = cx + cos(end) * radius1;
  int end1y = cy + sin(end) * radius1;
  int end2x = cx + cos(end) * radius2;
  int end2y = cy + sin(end) * radius2;
  drawRect(b1x, b1y, b2x, b2y, end1x, end1y, end2x, end2y, color);
  
}

void drawArcGradient(float start, float end, float step, uint16_t radius1, uint16_t radius2) {
  int a1x;
  int a1y;
  int a2x;
  int a2y;
  int b1x;
  int b1y;
  int b2x;
  int b2y;
  float angle;

  /*
  int a0x = cx + cos(start) * 0.5 * (radius1 + radius2);
  int a0y = cy + sin(start) * 0.5 * (radius1 + radius2);
  tft.fillCircle(a0x, a0y,  0.5 * (radius1 - radius2) , color);
  */

  float ratio;
  int startx = cx + cos(start) * 0.5 * (radius1 + radius2);
  int starty = cy + sin(start) * 0.5 * (radius1 + radius2);
  int endx = cx + cos(end) * 0.5 * (radius1 + radius2);
  int endy = cy + sin(end) * 0.5 * (radius1 + radius2);

  tft.fillCircle(startx, starty, 0.5 * (radius1 - radius2), gradient(0));
  tft.fillCircle(endx, endy, 0.5 * (radius1 - radius2), gradient(100));

  for (angle = start; angle > end - step; angle -= step) {
    ratio = (start - angle) / (start - end);
     
    a1x = cx + cos(angle) * radius1;
    a1y = cy + sin(angle) * radius1;
    a2x = cx + cos(angle) * radius2;
    a2y = cy + sin(angle) * radius2;
    b1x = cx + cos(angle - step) * radius1;
    b1y = cy + sin(angle - step) * radius1;
    b2x = cx + cos(angle - step) * radius2;
    b2y = cy + sin(angle - step) * radius2;
    drawRect(a1x, a1y, a2x, a2y, b1x, b1y, b2x, b2y, gradient(round(100 * ratio)));
  }
  ratio = 1;
  int end1x = cx + cos(end) * radius1;
  int end1y = cy + sin(end) * radius1;
  int end2x = cx + cos(end) * radius2;
  int end2y = cy + sin(end) * radius2;
  drawRect(b1x, b1y, b2x, b2y, end1x, end1y, end2x, end2y, gradient(round(100 * ratio)));
  
}

void loop(void) {

  drawArc(start, start - progress * (start - end), step, progress >= 1.00 ? SLACK_GREEN : SLACK_YELLOW, radius1Front, radius2Front);

  if (progress >= 1.00) {
    delay(1000);
    if (character > numCharacters - 1) {
      character = 0;
    } else {
      character++;
    }
    drawCharacter();
    drawArc(start, end, step, SLACK_BLACK, radius1Back + 2, radius2Back - 2);
    drawArcGradient(start, end, step, radius1Back, radius2Back);
    progress = 0.00;
  } else {
    progress += 0.01;
  }
}

void drawCharacter() {
  if (character == 0) {
    tft.drawRGBBitmap(0, 0, GOLLUM, tft.width(), tft.height());
  }
  if (character == 1) {
    tft.drawRGBBitmap(0, 0, FRODO, tft.width(), tft.height());
  }
  if (character == 2) {
    tft.drawRGBBitmap(0, 0, GANDALF, tft.width(), tft.height());
  }
  if (character == 3) {
    tft.drawRGBBitmap(0, 0, SAM, tft.width(), tft.height());
  }
  if (character == 4) {
    tft.drawRGBBitmap(0, 0, GALADRIEL, tft.width(), tft.height());
  }
  if (character == 5) {
    tft.drawRGBBitmap(0, 0, ARAGORN, tft.width(), tft.height());
  }
  if (character == 6) {
    tft.drawRGBBitmap(0, 0, PIPPIN, tft.width(), tft.height());
  }
  delay(1000);
}
