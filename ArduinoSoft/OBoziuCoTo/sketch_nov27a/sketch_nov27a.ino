#include <SPI.h>
#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 4;
String directory="/1/";
int n_files;
String nm;
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  n_files = countNumFiles(SD.open(directory));
  Serial.print("Number of MP3 files: "); Serial.println(n_files);
}

void loop() {
  int i, rand_photo;
  File folder = SD.open(directory);
  File random_file;
  rand_photo = random(0, n_files)+1;
  random_file = selectFileN(rand_photo, folder);
//  Serial.print("Random: ");Serial.print(rand_photo);
  if(random_file)
  nm = random_file.name();
  Serial.print("Random file name: "); Serial.println(nm);
  folder.rewindDirectory();  
  folder.close();
  random_file.close();

  delay(100);
}

int countNumFiles(File dir) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) { // no more files
      entry.rewindDirectory();
      break;
    }
    if (!entry.isDirectory()) {
      n_files++;
    }
    
    entry.close();
  }

  return n_files;
}

File selectFileN(int number, File dir)
{
  int counter = 0;
  File return_entry;
  while(true)
  {
    File entry = dir.openNextFile();
    if (! entry)
    {
      Serial.println("Last file reached");
      dir.rewindDirectory();
      break;
    }
    if(entry.name() != NULL)
      counter++;
    if(counter==number)
    {
      return_entry = entry;
      dir.rewindDirectory();
      break;
    }
    entry.close();
  }
  counter = 0;
  return return_entry;
}
