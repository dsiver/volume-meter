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

#define DEBUG
#define serialBaudRate 9600
#define minAmplitude 0
#define maxAmplitude 1023
#define rectangleWidthInterval EsploraTFT.width()/3
#define yellowRectangleStart EsploraTFT.width()-(rectangleWidthInterval*2)
#define redRectangleStart EsploraTFT.width()-rectangleWidthInterval
#define rectangleHeight 10

int oldRectangleWidth = 0;

void setup() {
  EsploraTFT.begin();
  Serial.begin(serialBaudRate);
  EsploraTFT.background(0, 0, 0);
}

void loop() {
  int loudness = Esplora.readMicrophone();
  loudness = constrain(loudness, minAmplitude, maxAmplitude);
  drawGraph(loudness);
}

// EsploraTFT.rect(xStart, yStart, width, height);
void drawGraph(int loudness) {
  int newRectangleWidth = map(loudness, minAmplitude, maxAmplitude, 0, EsploraTFT.width());
  newRectangleWidth = constrain(newRectangleWidth, 0, EsploraTFT.width());
  if (newRectangleWidth != oldRectangleWidth) {
    EsploraTFT.fill(0, 0, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, oldRectangleWidth, rectangleHeight);
  }
  drawRectangle(newRectangleWidth);
  oldRectangleWidth = newRectangleWidth;
#ifdef DEBUG
  outputToSerialMonitor(loudness, newRectangleWidth);
#endif
}

void drawRectangle(int width) {
  if (width < yellowRectangleStart) {
    EsploraTFT.fill(0, 255, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, width, rectangleHeight);
  }
  else if (width < redRectangleStart && width > yellowRectangleStart) {
    EsploraTFT.fill(0, 255, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, yellowRectangleStart, rectangleHeight);
    EsploraTFT.fill(0, 255, 255);
    EsploraTFT.rect(yellowRectangleStart, EsploraTFT.height() / 2, width, rectangleHeight);
  }
  else if (width < EsploraTFT.width() && width > redRectangleStart) {
    EsploraTFT.fill(0, 255, 0);
    EsploraTFT.rect(0, EsploraTFT.height() / 2, yellowRectangleStart, rectangleHeight);
    EsploraTFT.fill(0, 255, 255);
    EsploraTFT.rect(yellowRectangleStart, EsploraTFT.height() / 2, redRectangleStart, rectangleHeight);
    EsploraTFT.fill(0, 0, 255);
    EsploraTFT.rect(redRectangleStart, EsploraTFT.height() / 2, width, rectangleHeight);
  }
  delay(100);
  EsploraTFT.fill(0, 0, 0);
  EsploraTFT.rect(0, EsploraTFT.height() / 2, EsploraTFT.width(), rectangleHeight);
}

void outputToSerialMonitor(int loudness, int mappedValue) {
  Serial.print("loudness: " + String(loudness));
  Serial.print(" newRectangleWidth: " + String(mappedValue));
  Serial.print(" oldRectangleWidth: " + String(oldRectangleWidth));
  Serial.println("");
}
