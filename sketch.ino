/*
  Controls:
  Red button   (D9)  -> Bet on RED
  Black button (D8)  -> Bet on BLACK
  Green button (D7)  -> Bet on GREEN
  White button (D6) -> Spin the roulette wheel
*/

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

// pins
constexpr int startButtonPin = 6;
constexpr int redButtonPin = 9;
constexpr int blackButtonPin = 8;
constexpr int greenButtonPin = 7;
constexpr int dinPin = 10;

// settings
constexpr int normalSpinDelay = 30;
constexpr int finalSpinDelay = 100;
constexpr int revCount = 5;
constexpr int ledCount = 37;
constexpr int noBall = -1;

// colour choices
constexpr int noColour = -1;
constexpr int redColour = 0;
constexpr int blackColour = 1;
constexpr int greenColour = 2;

// function prototypes
void spinBall();
void displayRoulette(int posBall);
void displayResult(int posBall);

// game state variables
int randomRing = 0;
int selectedColour = noColour;
int resultColour = noColour;
bool canStart = false;

Adafruit_NeoPixel ring(ledCount, dinPin, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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
  lcd.print("Select a color");

  displayRoulette(noBall);
}

void loop()
{
  if (digitalRead(redButtonPin) == LOW)
  {
    selectedColour = redColour;
    lcd.clear();
    lcd.print("Colour: RED");
    delay(200);
  }
  else if (digitalRead(blackButtonPin) == LOW)
  {
    selectedColour = blackColour;
    lcd.clear();
    lcd.print("Colour: BLACK");
    delay(200);
  }
  else if (digitalRead(greenButtonPin) == LOW)
  {
    selectedColour = greenColour;
    lcd.clear();
    lcd.print("Colour: GREEN");
    delay(200);
  }

  if (digitalRead(startButtonPin) == LOW && canStart == true)
  {
    if (selectedColour == noColour)
    {
      lcd.clear();
      lcd.print("Please choose");
      lcd.setCursor(0, 1);
      lcd.print("a colour");
      delay(1000);
      lcd.clear();
      lcd.print("Select a color");
      return;
    }

    randomRing = random(ledCount);
    spinBall();
  }
}

void spinBall()
{
  canStart = false;
  int posBall = 0;

  lcd.clear();
  lcd.print("Wait...");

  for (int i = 0; i < ledCount * revCount; i++)
  {
    displayRoulette(posBall);
    posBall = (posBall + 1) % ledCount;
    delay(normalSpinDelay);
  }

  while (posBall != randomRing)
  {
    posBall = (posBall + 1) % ledCount;
    displayRoulette(posBall);
    delay(finalSpinDelay);
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
  lcd.clear();

  if (posBall == 0)
  {
    lcd.print("GREEN - ");
    resultColour = greenColour;
  }
  else if (posBall % 2 == 0)
  {
    lcd.print("BLACK - ");
    resultColour = blackColour;
  }
  else
  {
    lcd.print("RED - ");
    resultColour = redColour;
  }

  if (resultColour == selectedColour)
    lcd.print("You win");
  else
    lcd.print("You lose");

  selectedColour = noColour;
  delay(2000);
  lcd.clear();
  lcd.print("Select a color");
}