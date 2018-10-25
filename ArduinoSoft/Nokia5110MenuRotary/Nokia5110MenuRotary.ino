
//////////////////////////////////////////////
//       Arduino Rotary Encoder Menu        //
//                 v1.0                     //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

int menuItemsNum = 6;  //number of items in menu

int menuitem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;
int fontPos1 = 8;
int fontPos2 = 16;
int fontPos3 = 24;
int i = 1, j = 2, k = 3; //variables for changing menuItem

//listed menu items
char *menuItem[6] = {"Momir", "Standard", "a", "b", "c", "d"};
//strcpy(menuItem[1], "Momir");
//strcpy(menuItem[2], "Standard");
//strcpy(menuItem[3], "??????");
//strcpy(menuItem[4], "?????!");
//strcpy(menuItem[5], "!??!??");
//strcpy(menuItem[6], "?!?!?!");

boolean backlight = true;
int contrast = 60;
int volume = 50;

String language[3] = { "EN", "ES", "EL" };
int selectedLanguage = 0;

String difficulty[2] = { "EASY", "HARD" };
int selectedDifficulty = 0;

boolean up = false;
boolean down = false;
boolean middle = false;

ClickEncoder *encoder;
int16_t last, value;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//Adafruit_PCD8544 display = Adafruit_SSD1306( 5, 4, 3); //Download the latest Adafruit Library in order to use this constructor

void setup() {

  pinMode(7, OUTPUT);
  turnBacklightOn();

  encoder = new ClickEncoder( A1, A0, A2);
  encoder->setAccelerationEnabled(false);

  display.begin();
  display.clearDisplay();
  //setContrast();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  last = encoder->getValue();
}

void loop() {

  drawMenu();

  readRotaryEncoder();

  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Clicked:
        middle = true;
        break;
    }
  }

  if (up && page == 1 ) {

    up = false;
    if (menuitem == 2 && frame == 2)
    {
      frame--;
    }

    if (menuitem == 4 && frame == 4)
    {
      frame--;
    }
    if (menuitem == 3 && frame == 3)
    {
      frame--;
    }
    lastMenuItem = menuitem;
    menuitem--;
    if (menuitem == 0)
    {
      menuitem = 1;
    }
  } else if (up && page == 2 && menuitem == 1 ) {
    up = false;
    contrast--;
    //    setContrast();
  }
  else if (up && page == 2 && menuitem == 2 ) {
    up = false;
    volume--;
  }
  else if (up && page == 2 && menuitem == 3 ) {
    up = false;
    selectedLanguage--;
    if (selectedLanguage == -1)
    {
      selectedLanguage = 2;
    }
  }
  else if (up && page == 2 && menuitem == 4 ) {
    up = false;
    selectedDifficulty--;
    if (selectedDifficulty == -1)
    {
      selectedDifficulty = 1;
    }
  }

  if (down && page == 1) //We have turned the Rotary Encoder Clockwise
  {

    down = false;
    if (menuitem == 3 && lastMenuItem == 2)
    {
      frame ++;
    } else  if (menuitem == 4 && lastMenuItem == 3)
    {
      frame ++;
    }
    else  if (menuitem == 5 && lastMenuItem == 4 && frame != 4)
    {
      frame ++;
    }
    lastMenuItem = menuitem;
    menuitem++;
    if (menuitem == 7)
    {
      menuitem--;
    }

  } else if (down && page == 2 && menuitem == 1) {
    down = false;
    contrast++;
    //    setContrast();
  }
  else if (down && page == 2 && menuitem == 2) {
    down = false;
    volume++;
  }
  else if (down && page == 2 && menuitem == 3 ) {
    down = false;
    selectedLanguage++;
    if (selectedLanguage == 3)
    {
      selectedLanguage = 0;
    }
  }
  else if (down && page == 2 && menuitem == 4 ) {
    down = false;
    selectedDifficulty++;
    if (selectedDifficulty == 2)
    {
      selectedDifficulty = 0;
    }
  }

  if (middle) //Middle Button is Pressed
  {
    middle = false;

    if (page == 1 && menuitem == 5) // Backlight Control
    {
      if (backlight)
      {
        backlight = false;
        menuItem5 = "Light: OFF";
        turnBacklightOff();
      }
      else
      {
        backlight = true;
        menuItem5 = "Light: ON";
        turnBacklightOn();
      }
    }

    if (page == 1 && menuitem == 6) // Reset
    {
      resetDefaults();
    }


    else if (page == 1 && menuitem <= 4) {
      page = 2;
    }
    else if (page == 2)
    {
      page = 1;
    }
  }
}

int resetMenuValuesMore(int i) {
  i += 1;
  if (i > sizeof(menuItem)) {
    i = 1;
  }
  return i;
}

int resetMenuValuesLess(int i) {
  i -= 1;
  if (i < sizeof(menuItem)) {
    i = sizeof(menuItem);
  }
  return i;
}
void drawMenu_2() {
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(32, 0);
  display.print("MtG: Momir");

  bool menuHighlight1 = true;
  bool menuHighlight2 = false;
  bool menuHighlight3 = false;

  bool menuHighlight[3] = { 1, 0, 0 };

  if (up) {
    if (menuHighlight[0] && !menuHighlight[1]) {
      menuHighlight[0] = false;
      menuHighlight[1] = true;
    }
    else if (menuHighlight[1] && !menuHighlight[2]) {
      menuHighlight[1] = false;
      menuHighlight[2] = true;
      i = resetMenuValuesMore(i);
      j = resetMenuValuesMore(j);
      k = resetMenuValuesMore(k);
    }
    menuitem+=1;
  }

  if (down) {
    if (!menuHighlight[1] && menuHighlight[2]) {
      menuHighlight[2] = false;
      menuHighlight[1] = true;
    }

    else if (!menuHighlight[0] && menuHighlight[1]) {
      menuHighlight[1] = false;
      menuHighlight[0] = true;
      i = resetMenuValuesLess(i);
      j = resetMenuValuesLess(j);
      k = resetMenuValuesLess(k);
    }
    menuitem-=1;
  }

  //display ( item , rozmiar czcionki , podświetlenie )
  displayMenuItem(menuItem[i], fontPos1, menuHighlight[0]);
  displayMenuItem(menuItem[j], fontPos2, menuHighlight[1]);
  displayMenuItem(menuItem[k], fontPos3, menuHighlight[2]);
  display.display();
}

void drawMenu()
{

  if (page == 2 && menuitem == 1)
  {
    displayIntMenuPage(menuItem[0], contrast);
  }

  else if (page == 2 && menuitem == 2)
  {
    displayIntMenuPage(menuItem[1], volume);
  }
  else if (page == 2 && menuitem == 3)
  {
    displayStringMenuPage(menuItem[2], language[selectedLanguage]);
  }
  else if (page == 2 && menuitem == 4)
  {
    displayStringMenuPage(menuItem[3], difficulty[selectedDifficulty]);
  }
  else if (page == 2 && menuitem == 4)
  {
    displayStringMenuPage(menuItem[5], difficulty[selectedDifficulty]);
  }

}

void resetDefaults()
{
  contrast = 60;
  volume = 50;
  selectedLanguage = 0;
  selectedDifficulty = 0;
  //    setContrast();
  backlight = true;
  //    menuItem5 = "Light: ON";
  turnBacklightOn();
}

/*  void setContrast()
  {
    display.setContrast(contrast);
    display.display();
  }
*/
void turnBacklightOn()
{
  digitalWrite(7, LOW);
}

void turnBacklightOff()
{
  digitalWrite(7, HIGH);
}

void timerIsr() {
  encoder->service();
}

void displayIntMenuPage(String menuItem, int value)
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(32, 0);
  display.print(menuItem);
  display.drawFastHLine(0, 10, 83, BLACK);
  display.setCursor(5, 8);
  display.print("Mana cost");
  display.setTextSize(2);
  display.setCursor(40, 8);
  display.print(value);
  display.setTextSize(2);
  display.display();
}

void displayStringMenuPage(String menuItem, String value)
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 0);
  display.print(menuItem);
  display.drawFastHLine(0, 10, 83, BLACK);
  display.setCursor(5, 15);
  display.print("Value");
  display.setTextSize(2);
  display.setCursor(5, 25);
  display.print(value);
  display.setTextSize(2);
  display.display();
}

void displayMenuItem(String item, int position, boolean selected)
{
  if (selected)
  {
    display.setTextColor(BLACK, WHITE);
  } else
  {
    display.setTextColor(WHITE, BLACK);
  }
  display.setCursor(0, position);
  display.print("> " + item);
}

void readRotaryEncoder()
{
  value += encoder->getValue();

  if (value / 2 > last) {
    last = value / 2;
    down = true;
    delay(150);
  } else   if (value / 2 < last) {
    last = value / 2;
    up = true;
    delay(150);
  }
}



