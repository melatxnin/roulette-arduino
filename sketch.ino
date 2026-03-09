#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

Adafruit_NeoPixel ring(ledCount, dinPin, NEO_GRB + NEO_KHZ800);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

constexpr int ledCount = 37; // number of pixels
constexpr int dinPin = 6;

// buttons
constexpr int startButtonPin = 10;
constexpr int redButtonPin = 9;
constexpr int blackButtonPin = 8;
constexpr int greenButtonPin = 7;

constexpr int spinDelay = 30;

void spinBall();
void displayRoulette(int posBall);
void displayResult(int posBall);

int randomRing = 0;
bool canStart = false;

void setup()
{
  randomSeed(analogRead(A0));

  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(redButtonPin, INPUT_PULLUP);
  pinMode(blackButtonPin, INPUT_PULLUP);
  pinMode(greenButtonPin, INPUT_PULLUP);

  canStart = true;

  ring.begin();
  ring.clear();
  ring.show();

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Press to start");
}

void loop()
{
  if (digitalRead(startButtonPin) == LOW && canStart == true)
  {
    randomRing = random(ledCount); // new random result
    spinBall();
  }
}

void spinBall()
{
  canStart = false;
  int posBall = 0;

  lcd.clear();
  lcd.print("Wait...");

  for (int i = 0; i < ledCount * 5; i++)
  {
    displayRoulette(posBall);
    posBall = (posBall + 1) % ledCount;
    delay(spinDelay);
  }

  while (posBall != randomRing)
  {
    posBall = (posBall + 1) % ledCount;
    displayRoulette(posBall);
    delay(spinDelay);
  }

  displayResult(posBall);

  displayRoulette(posBall);
  delay(1000);
  canStart = true;
}

void displayRoulette(int posBall)
{
  for (int i = 0; i < ledCount; i++)
  {
    if (i == posBall)
    {
      ring.setPixelColor(i, ring.Color(255, 255, 255));
    }
    else if (i == 0)
    {
      ring.setPixelColor(i, ring.Color(0, 255, 0));
    }
    else if (i % 2 == 0)
    {
      ring.setPixelColor(i, ring.Color(0, 0, 0));
    }
    else
    {
      ring.setPixelColor(i, ring.Color(255, 0, 0));
    }
  }

  ring.show();
}

void displayResult(int posBall)
{
  if (posBall == 0)
  {
    lcd.clear();
    lcd.print("Result: 0");
  }
  else if (posBall % 2 == 0)
  {
    lcd.clear();
    lcd.print("Result: BLACK");
  }
  else
  {
    lcd.clear();
    lcd.print("Result: RED");
  }

  lcd.setCursor(0, 2);
  lcd.print("Press to restart");
}