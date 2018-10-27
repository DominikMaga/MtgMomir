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

int page = 1;              // ustawienie okna głównego
int fontPos1 = 8;         //kolejne pozycje pixeli na wyświetlaczu
int fontPos2 = 16;
int fontPos3 = 24;
int l = 0, m = 1, n = 2; //zmienne potrzebne do ustalenia wyświetlania przedmiotów w menu

String menuItem[6] = {"Momir", "Standard", "cba", "bcbv", "bcc", "dbcv"};        // lista przedmiotow w menu

bool menuHighlight[3] = { 1, 0, 0 };
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

void setup() {
  Serial.begin(9600);
  encoder = new ClickEncoder( A1, A0, A2);
  encoder->setAccelerationEnabled(false);

  display.begin();
  display.clearDisplay();

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
  if (down && page == 1 ) {
    // Początek przesówania menu w dół
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
        contrast--;
        break;
      case 1:
        volume--;
        break;
      case 2:
//        selectedLanguage--;
        selectedLanguage=resetValuesLess(selectedLanguage, 0, 2);
        break;
      case 3:
//        selectedDifficulty--;
        selectedDifficulty=resetValuesLess(selectedDifficulty, 0, 1);
        break;
    }
  }
  if (up && page == 1)                     // Obsługa przesówania menu w górę
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
        contrast++;
        break;
      case 1:
        volume++;
        break;
      case 2:
//        selectedLanguage++;
        selectedLanguage=resetValuesMore(selectedLanguage, 0, 2);
        break;
      case 3:
//        selectedDifficulty++;
        selectedDifficulty=resetValuesMore(selectedDifficulty, 0, 1);
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
    display.display();
    Serial.println(page);
  } else if (page == 2) {
    switch (getMenuItem()) {
      case 0:
        displayIntMenuPage(menuItem[0], contrast);
        break;
      case 1:
        displayIntMenuPage(menuItem[1], volume);
        break;
      case 2:
        displayStringMenuPage(menuItem[2], language[selectedLanguage]);
        break;
      case 3:
        displayStringMenuPage(menuItem[3], difficulty[selectedDifficulty]);
        break;
    }
  }
}

void resetDefaults()            // przywrócenie wartosci domyslnych (raczej nie potrzebne)
{
  contrast = 60;
  volume = 50;
  selectedLanguage = 0;
  selectedDifficulty = 0;
  //    setContrast();
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
  display.setTextSize(2);
  display.setCursor(5, 25);
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
    delay(150);
  } else   if (value / 2 < last) {
    last = value / 2;
    up = true;
    delay(150);
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
