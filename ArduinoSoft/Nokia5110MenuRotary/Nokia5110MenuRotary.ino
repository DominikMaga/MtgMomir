/*
            ##<><><><><><><><><><><><><><><><><><><><><>##
             +       Arduino Rotary Encoder Menu         +
             +                 v2.0                      +
             +           by Dominik Maga                 +
            ##<><><><><><><><><><><><><><><><><><><><><>##
*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <stdio.h>
#include <stdlib.h>

int page = 1;              // ustawienie okna głównego
int fontPos1 = 8;         //kolejne pozycje pixeli na wyświetlaczu
int fontPos2 = 16;
int fontPos3 = 24;
int l = 0, m = 1, n = 2; //zmienne potrzebne do ustalenia wyświetlania przedmiotów w menu

String menuItem[6] = {"Mana cost", "Vol", "Format", "ON/OFF", "reset", "???"};        // lista przedmiotow w menu

bool menuHighlight[3] = { 1, 0, 0 };
boolean backlight = true; //czy to potrzebne?
int manaCost = 0;
int volume = 50;

String standard[4] = { "Standard", "Pauper", "Full Momir", "Pauper Momir" };
int selectedStandard = 0;

String difficulty[2] = { "EASY", "HARD" };
int selectedDifficulty = 0;

boolean up = false;
boolean down = false;
boolean middle = false;

ClickEncoder *encoder;
int16_t last, value;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(9600);

  //random kradziony
  pinMode(13, OUTPUT); // signal ready to go and then wait a bit
  digitalWrite(13, HIGH);
  delay(2000);

  encoder = new ClickEncoder( A1, A0, A2);
  encoder->setAccelerationEnabled(false);

  display.begin();
  display.clearDisplay();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  last = encoder->getValue();
}

void loop() {

  //kradziony random
  unsigned long seed=seedOut(31); // create 31 bit seed and show value, random results
  randomSeed(seed);

  /*Serial.print(" random (1-99) = ");
  for (int i=0;i<20;++i)
  {
    Serial.print(random(1,100));
    Serial.print(" ");
  }
  Serial.print('n');
  */

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

  if (middle) //Middle Button is Pressed               /wcisniecie przycisku enkodera
  {
    middle = false;
    if (page == 1) {
      switch (getMenuItem()) {
        case 4:
          if (backlight) {
            Serial.println();
            backlight = false;
            setMenuItem("Light: ON");
            //+ ew. jakas metoda
          } else {
            backlight = true;
            setMenuItem("Light: OFF");
          }
          break;
        case 5:
          resetDefaults();
          break;
        default:
          page = 2;
          break;
      }
    } else if (page == 2) {
      page = 1;
    }
  }

  if (down && page == 1 ) {    // Początek przesuwania menu w dół
    down = false;
    if (menuHighlight[0] && !menuHighlight[1]) {
      menuHighlight[0] = false;
      menuHighlight[1] = true;
    }
    else if (menuHighlight[1] && !menuHighlight[2]) {
      menuHighlight[1] = false;
      menuHighlight[2] = true;
    }
    else if(menuHighlight[2] && !menuHighlight[1] && !menuHighlight[0]){                           //tego brakowało tu był błąd
      l = resetValuesMore(l, 0, (sizeof(menuItem)/6)-1);
      m = resetValuesMore(m, 0, (sizeof(menuItem)/6)-1);
      n = resetValuesMore(n, 0, (sizeof(menuItem)/6)-1);
    }
  } else if (up && page == 2) {            //kontorla odejmowania wartości przez encoder
    up = false;
    switch (getMenuItem()) {
      case 0:
        manaCost--;
        break;
      case 1:
        volume--;
        break;
      case 2:
//        selectedStandard--;
        selectedStandard=resetValuesLess(selectedStandard, 0, 3);
        break;
      case 3:
//        selectedDifficulty--;
        selectedDifficulty=resetValuesLess(selectedDifficulty, 0, 1);
        break;
    }
  }
  if (up && page == 1)                     // Obsługa przesuwania menu w górę
  {
    up = false;
    if (!menuHighlight[1] && menuHighlight[2]) {
      menuHighlight[2] = false;
      menuHighlight[1] = true;
    }
    else if (!menuHighlight[0] && menuHighlight[1]) {
      menuHighlight[1] = false;
      menuHighlight[0] = true;
    }
    else if (menuHighlight[0] && !menuHighlight[1] && !menuHighlight[2]){
      l = resetValuesLess(l, 0, (sizeof(menuItem)/6)-1);
      m = resetValuesLess(m, 0, (sizeof(menuItem)/6)-1);
      n = resetValuesLess(n, 0, (sizeof(menuItem)/6)-1);
    }
  } else if (down && page == 2) {            //kontorla odejmowania wartości przez encoder w podmenu
    down = false;
    switch (getMenuItem()) {
      case 0:
        manaCost=resetValuesMore(manaCost, 0, 16);
        break;
      case 1: // co z tym zrobić?
        volume++;
        break;
      case 2:
//        selectedStandard++;
        selectedStandard=resetValuesMore(selectedStandard, 0, 3);
        break;
      case 3:
//        selectedDifficulty++;
        selectedDifficulty=resetValuesMore(selectedDifficulty, 0, 1);
        break;
    }
  }


}
void drawMenu() {
  display.setTextSize(1);                   //  ustawienie wyglądu głównego menu
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(32, 0);
  display.print("MtG: Momir");
  //rysowanie glownego menu oraz podmenu
  if (page == 1) {
    //display ( item , rozmiar czcionki , podświetlenie )
    displayMenuItem(menuItem[l], fontPos1, menuHighlight[0]);
    displayMenuItem(menuItem[m], fontPos2, menuHighlight[1]);
    displayMenuItem(menuItem[n], fontPos3, menuHighlight[2]);
    Serial.println(page);
  } else if (page == 2) {
    switch (getMenuItem()) {
      case 0:
        displayIntMenuPage(menuItem[0], manaCost);
        break;
      case 1:
        displayIntMenuPage(menuItem[1], volume);
        break;
      case 2:
        displayStringMenuPage(menuItem[2], standard[selectedStandard]);
        break;
      case 3:
        displayStringMenuPage(menuItem[3], difficulty[selectedDifficulty]);
        break;
    }
  }
  display.display();
}

void resetDefaults()            // przywrócenie wartosci domyslnych (raczej nie potrzebne)
{
  manaCost = 0;
  volume = 50;
  selectedStandard = 0;
  selectedDifficulty = 0;
  //    setmanaCost();
  backlight = true;
  //    menuItem5 = "Light: ON";
  //  turnBacklightOn();
}

void timerIsr() {                     ////idk chyba potrzebne////
  encoder->service();
}

void displayIntMenuPage(String menuItem, int value)  ///menu dla kontroli int'ow
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(32, 0);
  display.print(menuItem);
  display.drawFastHLine(0, 10, 83, BLACK);
  display.setCursor(5, 8);
  display.print("Value");
  display.setTextSize(2);
  display.setCursor(40, 8);
  display.print(value);
  display.setTextSize(2);
  display.display();
}

void displayStringMenuPage(String menuItem, String value)     // menu do kontroli stringow np język
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 0);
  display.print(menuItem);
  display.drawFastHLine(0, 10, 83, BLACK);
  display.setCursor(5, 15);
  display.print("Value");
  display.setTextSize(1);     //DOPRACOWAĆ
  display.setCursor(40, 8);   //
  display.print(value);
  display.setTextSize(2);
  display.display();
}

void displayMenuItem(String item, int position, bool selected)   //potrzebne do rysowania elemnetów menu
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

void readRotaryEncoder()          // ustalenie w ktora strone obraca się encoder
{
  value += encoder->getValue();

  if (value / 2 > last) {
    last = value / 2;
    down = true;
    delay(500);
  } else   if (value / 2 < last) {
    last = value / 2;
    up = true;
    delay(500);
  }
}
void setMenuItem(String a) {        // zmienia nazwe przedmiotu w menu

  if (menuHighlight[0]) {
    menuItem[l] = a;
  }
  if (menuHighlight[1]) {
    menuItem[m] = a;
  }
  if (menuHighlight[2]) {
    menuItem[n] = a;
  }

}
int getMenuItem() {           // zwraca aktualnie wybrany przedmiot w Menu
  if (menuHighlight[0]) {
    return l;
  }
  if (menuHighlight[1]) {
    return m;
  }
  if (menuHighlight[2]) {
    return n;
  }

}
int resetValuesMore(int i, int MinValue, int MaxValue) { //przekręcenie licznika w górę
  i += 1;
  if (i > MaxValue ) {  ///sizeof(menuItem)
    i = MinValue;
  }
  return i;
}

int resetValuesLess(int i, int MinValue, int MaxValue) {  // przekręcenie licznika w dół
  i -= 1;
  if (i < MinValue) { //sizeof(menuItem)
    i = MaxValue;
  }
  return i;
}

unsigned int bitOut(void)
{
  static unsigned long firstTime=1, prev=0;
  unsigned long bit1=0, bit0=0, x=0, port=0, limit=99;
  if (firstTime)
  {
    firstTime=0;
    prev=analogRead(port);
  }
  while (limit--)
  {
    x=analogRead(port);
    bit1=(prev!=x?1:0);
    prev=x;
    x=analogRead(port);
    bit0=(prev!=x?1:0);
    prev=x;
    if (bit1!=bit0)
      break;
  }
  return bit1;
}

unsigned long seedOut(unsigned int noOfBits)
{
  // return value with 'noOfBits' random bits set
  unsigned long seed=0;
  for (int i=0;i<noOfBits;++i)
    seed = (seed<<1) | bitOut();
  return seed;
}

void printDirectory(File dir, int numTabs) { //TODO
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
      n_files++;
    }
    
    entry.close();
  }
  Serial.println(n_files);
}
