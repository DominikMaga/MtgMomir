
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
//int frame = 1;
int page = 1; // dodać więcej page 2,3,4,5 itd dla każdego nowego typu okna
int lastMenuItem = 1;
int fontPos1 = 8;
int fontPos2 = 16;
int fontPos3 = 24;
int l = 1, m = 2, n = 3; //variables for changing menuItem


//listed menu items
char *menuItem[6] = {"Momir", "Standard", "a", "b", "c", "d"};
//strcpy(menuItem[1], "Momir");
//strcpy(menuItem[2], "Standard");
//strcpy(menuItem[3], "??????");
//strcpy(menuItem[4], "?????!");
//strcpy(menuItem[5], "!??!??");
//strcpy(menuItem[6], "?!?!?!");

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

  pinMode(7, OUTPUT);
  turnBacklightOn();

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

  if (up && page == 1 ) {
    // Początek przesówania menu w dół?
    up = false;




    if (menuHighlight[0] && !menuHighlight[1]) {
      menuHighlight[0] = false;
      menuHighlight[1] = true;
    }
    else if (menuHighlight[1] && !menuHighlight[2]) {
      menuHighlight[1] = false;
      menuHighlight[2] = true;
      l = resetValuesMore(l, , 0, sizeof(menuItem));
      m = resetValuesMore(m, 0, sizeof(menuItem));
      n = resetValuesMore(n, 0, sizeof(menuItem));
    }


  } else if (up && page == 2) {            //kontorla odejmowania wartości przez encoder
    up = false;
    switch (getMenuItem()) {
      case 1:
        contast--;
        break;
      case 2:
        volume--;
        break;
      case 3:
        selectedLanguage--;
        resetValuesLess(selectedLagnguage, 0, 2);
        break;
      case 4:
        selectedDifficulty--;
        resetValuesLess(selectedDifficulty, 0, 1);
        break;

    }
  }


  if (down && page == 1)                     // Obsługa przesówania menu w górę
  {

    down = false;

    if (!menuHighlight[1] && menuHighlight[2]) {
      menuHighlight[2] = false;
      menuHighlight[1] = true;
    }

    else if (!menuHighlight[0] && menuHighlight[1]) {
      menuHighlight[1] = false;
      menuHighlight[0] = true;
      l = resetValuesLess(l, 1, sizeof(menuItem));
      m = resetValuesLess(m, 1, sizeof(menuItem));
      n = resetValuesLess(n, 1, sizeof(menuItem));
    }



    // obsługa odejmowania wartości w innych elementach w menu -- trzeba przerobić na bardziej dynamiczne

  } else if (up && page == 2) {            //kontorla odejmowania wartości przez encoder
    up = false;
    switch (getMenuItem()) {
      case 1:
        contast++;
        break;
      case 2:
        volume++;
        break;
      case 3:
        selectedLanguage++;
        resetValuesMore(selectedLagnguage, 0, 2);
        break;
      case 4:
        selectedDifficulty++;
        resetValuesMore(selectedDifficulty, 0, 1);
        break;

    }

  }


  if (middle) //Middle Button is Pressed               /wcisniecie przycisku enkodera
  {
    middle = false;
    if (page == 1) {
      switch (getMenuItem()) {
        case 5:
          if (backlight) {
            backlight = false;
            setMenuItem("Light: ON");
            //+ ew. jakas metoda
          } else {
            backlight = true;
            setMenuItem("Light: OFF");
          }
          break;
        case 6:
          resetDefaults();
          break;
        default:
          page = 2
                 break;
      }
    } else if (page == 2) {
      page = 1;
    }
  }
}


//kontrola elementów z poziomu menu

}

//////////////////////////////////////////////////Moje


void drawMenu_2() {
  display.setTextSize(1);                     ustawienie wyglądu głównego menu
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(32, 0);
  display.print("MtG: Momir");



  //if(page=1){
  //display ( item , rozmiar czcionki , podświetlenie )
  displayMenuItem(menuItem[i], fontPos1, menuHighlight[0]);
  displayMenuItem(menuItem[j], fontPos2, menuHighlight[1]);
  displayMenuItem(menuItem[k], fontPos3, menuHighlight[2]);
  display.display();
  //} else if(page ==2){
  switch (getMenuItem()) {
    case 1:
      displayIntMenuPage(menuItem[0], contrast);
      break;
    case 2:
      displayIntMenuPage(menuItem[1], volume);
      break;
    case 3:
      displayStringMenuPage(menuItem[2], language[selectedLanguage]);
      break;
    case 4:
      displayStringMenuPage(menuItem[2], difficulty[selectedDifficulty]);
      break;
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

void displayIntMenuPage(String menuItem, int value)  ///menu dla kontroli intow
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

void displayStringMenuPage(String menuItem, String value)     // menu do kontroli stringow
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

void displayMenuItem(String item, int position, boolean selected)   //potrzebne do rysowania elemnetów menu
{
  if (selected)
  {
    display.setTextColor(BLACK, WHITE);
  } else
  {
    display.setTextColor(WHITE, BLACK);
  }
  display.setCursor(0, position);
  display.print("•" + item);
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
void setMenuItem(string a) {

  if (menuHighlight[0] {
  menuItem[l] = a;
  }
  if (menuHighlight[1] {
  menuItem[m] = a;
  }
  if (menuHighlight[2] {
  menuItem[n] = a;
  }

}
int getMenuItem() {
  if (menuHighlight[0]) {
    return l;
  }
  if (menuHighlight[1]) {
    retrun m;
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



