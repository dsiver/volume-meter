/*
 * Volume Meter
 * Board: Arduino Esplora
 *
 * David Siver
 * CS 4985
 * Fall 2015
 */

#include <Esplora.h>
#include <SPI.h>
#include <TFT.h>

#define DEBUG 0
#define serialBaudRate 9600
#define minAmplitude 0
#define maxAmplitude 1023
#define rectangleWidthInterval EsploraTFT.width()/6
#define yellowRectangleStart 0+rectangleWidthInterval
#define redRectangleStart 0+(rectangleWidthInterval*3)
#define rectangleHeight 10

int oldRectangleWidth = 0;

void setup() {
  EsploraTFT.begin();
  Serial.begin(serialBaudRate);
  EsploraTFT.background(0, 0, 0);
#ifdef DEBUG
  //  char text[4];
  //  String(EsploraTFT.width()).toCharArray(text, 4);
  //  EsploraTFT.stroke(255, 255, 255);
  //  EsploraTFT.text(text, 0, 0);
  Serial.print("yellowRectangleStart: " + String(yellowRectangleStart));
  Serial.print(" redRectangleStart: " + String(redRectangleStart));
  Serial.println("");
#endif
}

void loop() {
  int loudness = Esplora.readMicrophone();
  loudness = constrain(loudness, minAmplitude, maxAmplitude);
  drawGraph(loudness);
}

void drawGraph(int loudness) {
  int newRectangleWidth = map(loudness, minAmplitude, maxAmplitude, 0, EsploraTFT.width());
  newRectangleWidth = constrain(newRectangleWidth, 0, EsploraTFT.width());
  if (newRectangleWidth != oldRectangleWidth) {
    EsploraTFT.fill(0, 0, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, oldRectangleWidth, rectangleHeight);
  }
  drawRectangle(newRectangleWidth);
  oldRectangleWidth = newRectangleWidth;
}

void drawRectangle(int width) {
  EsploraTFT.fill(0, 0, 0);
  EsploraTFT.rect(0, EsploraTFT.height() / 2, EsploraTFT.width(), rectangleHeight);
  if (width < yellowRectangleStart) {
    EsploraTFT.fill(0, 255, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, width, rectangleHeight);
  }
  else if (width > yellowRectangleStart && width < redRectangleStart) {
    EsploraTFT.fill(0, 255, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, yellowRectangleStart, rectangleHeight);
    EsploraTFT.fill(0, 255, 255);
    width -= yellowRectangleStart;
    EsploraTFT.rect(yellowRectangleStart, EsploraTFT.height() / 2, width, rectangleHeight);
#ifdef DEBUG
    Serial.println("Y, width: " + String(width));
#endif
  }
  else if (width > redRectangleStart) {
    EsploraTFT.fill(0, 255, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, yellowRectangleStart, rectangleHeight);
    EsploraTFT.fill(0, 255, 255);
    EsploraTFT.rect(yellowRectangleStart, EsploraTFT.height() / 2, redRectangleStart, rectangleHeight);
    EsploraTFT.fill(0, 0, 255);
    EsploraTFT.rect(redRectangleStart, EsploraTFT.height() / 2, width, rectangleHeight);
#ifdef DEBUG
    Serial.println("R, width: " + String(width));
#endif
  }
  delay(10);
}
