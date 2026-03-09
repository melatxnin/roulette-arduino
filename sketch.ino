#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

constexpr int ledCount = 37; // number of pixels
constexpr int dinPin = 6;

// buttons
constexpr int startButtonPin = 10;
constexpr int redButtonPin = 9;
constexpr int blackButtonPin = 8;
constexpr int greenButtonPin = 7;

constexpr int spinDelay = 30;

// colour choices
constexpr int noColour = -1;
constexpr int redColour = 0;
constexpr int blackColour = 1;
constexpr int greenColour = 2;

void spinBall();
void displayRoulette(int posBall);
void displayResult(int posBall);

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