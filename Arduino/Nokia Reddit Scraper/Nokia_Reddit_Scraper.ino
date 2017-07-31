#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
//Followed https://www.youtube.com/watch?v=VtZvf5T98FI as a basis for the menu code
/*
  LCD -> Arduino Uno pin layout
  8 -> GND
  7 -> PIN 7
  6 -> 3.3 V
  5 -> PIN 13
  4 -> PIN 11
  3 -> PIN 5
  2 -> PIN 4
  1 -> PIN 3
  UP -> PIN 2
  DOWN -> PIN 12
  SELECT -> PIN 8
*/
static const unsigned char pic2[] PROGMEM = {
  B11111111, B11111111, B11111111, B11111110, B11111111, B11111111, B11111111, B11110000,
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11110000,
  B11111111, B11111111, B11111111, B11110111, B10110101, B11011111, B11111111, B11110000,
  B11111111, B11111111, B11110111, B11011110, B11011111, B01110111, B11111111, B11100000,
  B11111111, B11111111, B11111111, B01111110, B01111011, B11101111, B11111111, B10110000,
  B11111111, B11111111, B11011001, B11000001, B10000101, B10110111, B10111111, B11100000,
  B11111111, B10111101, B11110111, B01011010, B01011010, B01101100, B10111111, B11100000,
  B11111111, B11111011, B11101000, B00010010, B10001000, B10010111, B11011111, B01100000,
  B11111101, B11111101, B11111001, B10100010, B00101001, B10010101, B01101110, B11000000,
  B11111111, B01100110, B00000100, B10001000, B10000010, B01001010, B10010101, B10100000,
  B11111111, B01010101, B01100000, B00000001, B00001000, B01010001, B10111001, B01000000,
  B11111101, B11101010, B00000110, B00101000, B00100010, B00100110, B00101010, B10100000,
  B11011110, B10010110, B00010000, B00000000, B10000001, B00010011, B10010101, B00100000,
  B10111010, B01100001, B10000000, B00000000, B00000000, B10001000, B01101010, B01000000,
  B10111011, B10011010, B01000000, B00001111, B11100000, B10010100, B00100011, B00000000,
  B01100110, B01100000, B00000000, B00011111, B11110000, B00000010, B10010100, B10000000,
  B10101010, B01001000, B01000000, B00111111, B11111100, B01000011, B10001000, B10000000,
  B11011101, B10110010, B00000000, B11111111, B11111100, B00000000, B00110011, B00000000,
  B00010110, B00000101, B00000001, B11111111, B11111111, B00010110, B01000100, B00000000,
  B01101001, B01100000, B00100011, B11111111, B11111100, B00000000, B00100010, B01000000,
  B10100100, B00100000, B00000001, B11111011, B11111110, B01000001, B00000000, B00000000,
  B00011010, B10101001, B01000100, B11111011, B01111100, B00000100, B01011010, B00000000,
  B00101001, B10001010, B00100000, B11111001, B01111100, B00010000, B00000000, B00000000,
  B00100100, B00100000, B00000000, B01101100, B01111100, B00000010, B00101000, B00000000,
  B10101100, B00101000, B00000000, B00101110, B00111000, B00001000, B01010010, B00000000,
  B10000001, B01000010, B00100001, B11110000, B01111111, B00010000, B00000000, B00000000,
  B00010001, B00011010, B01101111, B11111011, B11111111, B11100001, B00001000, B00000000,
  B01101100, B10000000, B00111111, B11111110, B10111111, B11111101, B01010001, B00000000,
  B00100010, B10001001, B01111111, B11100000, B00100110, B11111111, B00000100, B00000000,
  B00011010, B00100001, B11111111, B11100101, B01011111, B11111111, B01000000, B10000000,
  B10000000, B10000011, B11111111, B01111010, B00011011, B10111111, B11010010, B00000000,
  B00101101, B00110111, B11111111, B01101111, B11101110, B11111111, B11001100, B00000000,
  B10010100, B10001111, B11111111, B11101011, B10111101, B11111111, B11111100, B00000000,
  B01101001, B01101111, B11111111, B11111010, B01100110, B01111111, B11110010, B10000000,
  B11010101, B01011111, B11111111, B11011110, B01010110, B10111111, B11111010, B00100000,
  B00101010, B10111111, B11111111, B10111011, B11011011, B11111111, B11111101, B11000000,
  B10011001, B11111111, B11111111, B11101111, B10011011, B11111111, B11111110, B01000000,
  B11111111, B11111111, B11111111, B11111110, B11110111, B11111111, B11111101, B10100000,
  B00100100, B01111111, B11111111, B11111111, B11100111, B11111111, B11111110, B01000000,
  B11111111, B01111111, B11111111, B11111011, B11111111, B11111111, B11111111, B10100000,
  B11111111, B11111111, B11111111, B11111110, B11111111, B11111111, B11111111, B11000000,
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11110000,
  B11111111, B11111111, B11101111, B11111101, B11111101, B11111111, B11101111, B11110000,
  B01010110, B10000001, B00001010, B01000000, B00000000, B11101011, B00001110, B01000000,
  B00011000, B01001000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B01000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00001000, B00000000, B00000000, B00000000, B01010000, B00000000, B00000000, B00000000,
  B01000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
};

static const unsigned char pic[] PROGMEM = {
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00001111, B11100000, B00000000, B00000000, B00000000,
  B00000000, B00011000, B00110000, B00000000, B00000000, B00000000,
  B00000000, B00110000, B00010000, B00000000, B00000000, B00000000,
  B00000000, B00100000, B00001000, B00000000, B00000000, B00000000,
  B00000000, B01100000, B00001000, B00000000, B00000000, B00000000,
  B00000000, B11110001, B11001100, B00000000, B00000000, B00000000,
  B00000001, B11011011, B11101000, B00000000, B00000000, B00000000,
  B00000001, B11010011, B10101000, B00000000, B00000000, B00000000,
  B00000001, B11111001, B01101000, B00000000, B00000000, B00000000,
  B00000001, B10011101, B11001000, B00000000, B00000000, B00000000,
  B00000001, B11111111, B11001000, B00000011, B00000000, B00000000,
  B00000001, B10110100, B00001000, B00000010, B10000000, B00000000,
  B00000001, B00000000, B00001000, B00000110, B10000000, B00000000,
  B00000001, B00000000, B00001000, B00000100, B01000000, B00000000,
  B00000001, B00000000, B00001000, B00001100, B10000000, B00000000,
  B00000001, B00000000, B11101100, B00001000, B10000000, B00000000,
  B00000001, B00000000, B01000101, B11111001, B10000000, B00000000,
  B00000001, B00000000, B11100111, B00110001, B10000000, B00000000,
  B00000001, B00000000, B01100100, B10110011, B11000000, B00000000,
  B00000000, B10000000, B10100100, B11111101, B01100000, B00000000,
  B00000000, B10000000, B01100001, B00000110, B11000000, B00000000,
  B00000000, B10000000, B01110000, B00000001, B00100000, B00000000,
  B00000000, B11000000, B11011000, B00000001, B11100000, B00000000,
  B00000000, B01000000, B10111000, B00000001, B11000000, B00000000,
  B00000000, B01100000, B11100000, B00000001, B00000000, B00000000,
  B00000000, B00100000, B10000000, B10000011, B00000000, B00000000,
  B00000000, B00110000, B00000000, B00000010, B00000000, B00000000,
  B00000000, B00001100, B00000000, B00001110, B00000000, B00000000,
  B00000000, B00000111, B10000000, B00111000, B00000000, B00000000,
  B00000000, B00000011, B11111110, B11110000, B00000000, B00000000,
  B00000000, B00000011, B10011010, B10000000, B00000000, B00000000,
  B00000000, B00010011, B10011111, B10000000, B00000000, B00000000,
  B00000000, B00111111, B00011010, B10000000, B00000000, B00000000,
  B00000000, B00010101, B00001100, B10000000, B00000000, B00000000,
  B00000000, B00011011, B00000111, B10000000, B00000000, B00000000,
  B00000000, B00000110, B00000011, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
};

//Options
boolean backlight = true;
int contrast = 42;
int num_results = 10;   //Number of results that will be displayed
bool readability = true;

int menu_item = 1;      //Current item on the menu
int page = 1;           //Current page
String sub = "popular"; //Current selected sub
String preset_subs[6];  //Array of preset subs
int letter = 0;         //Keep track of current letter when typing
char chars[26];         //Characters A-Z

int test = 0; //Testing with the GFX library on the contrast page

/*
  Pages: main, sub, type, scrape, options, change_contrast, change_num, choose_preset
  main - choose sub, scrape, or go to options
  sub - type in a subreddit or choose from a preset sub
  type - type in a subreddit
  options - change the number of scraped results, change the contrast, turn on/off the backlight
  change_contrast - change contrast
  change_num - change number of scraped results
  choose_preset - choose a preset sub
*/
/*
  main - 1
  subreddit menu - 2
  type sub - 3
  choose preset sub - 4
  scrape - 5
  options - 6
  num of results - 7
  change contrast - 8
*/

//Keep track of whether the button has been pressed
volatile boolean up = false;
volatile boolean down = false;
volatile boolean select = false;
int down_button_state = 0;
int up_button_state = 0;
int select_button_state = 0;
int lastdown_button_state = 0;
int lastselect_button_state = 0;
int lastup_button_state = 0;


Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);

void menu();  //Render the display
void check_down_button();
void check_up_button();
void check_select_button();
void scrape();  //Scrape the subreddit


void setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, OUTPUT);

  digitalWrite(7, LOW); //Turn Backlight ON

  Serial.begin(9600);
  display.begin();
  display.setContrast(contrast); //Set default contrast to 42
  display.display();
  display.clearDisplay();

  display.setTextSize(2);

  display.setTextColor(BLACK, WHITE);
  display.setCursor(5, 0);
  display.print("Reddit");
  display.setCursor(0, 20);
  display.print("Scraper");
  display.display();
  delay(2000);
  display.clearDisplay();
  instantiate_vars();
}

void loop() {
  menu();

  down_button_state = digitalRead(12);
  select_button_state = digitalRead(8);
  up_button_state = digitalRead(2);

  check_down_button();
  check_up_button();
  check_select_button();

  //Switch-case statements to determine which page is currently being viewed
  switch (page) {
    //Main
    case 1:
      if (up) {
        up = false;
        menu_item--;
        if (menu_item == 0)
          menu_item = 3;
      }
      else if (down) {
        down = false;
        menu_item++;
        if (menu_item == 4)
          menu_item = 1;
      }

      else if (select) {
        select = false;
        switch (menu_item) {
          case 1:
            page = 2; //Subreddit menu
            break;
          case 2:
            page = 5; //Scrape
            break;
          case 3:
            page = 6; //Options
            break;
        }
        menu_item = 1;
      }
      break;

    //Subreddit menu
    case 2:
      if (up) {
        up = false;
        menu_item--;
        if (menu_item == 0)
          menu_item = 3;
      }

      else if (down) {
        down = false;
        menu_item++;
        if (menu_item == 4)
          menu_item = 1;
      }

      else if (select) {
        select = false;
        switch (menu_item) {
          case 1:
            page = 3; //Type sub
            sub = "";
            letter = 0;
            break;
            
          case 2:
            page = 4; //Choose preset sub
            break;
            
          case 3:
            page = 1; //Main menu
            break;
        }
        menu_item = 1;
      }
      break;

    //Type sub
    case 3:
      if (up) {
        up = false;
        letter++;
        if (letter == 26)
          letter = 0;
      }

      else if (down) {
        down = false;
        sub += chars[letter];
        letter = 0;
      }

      else if (select) {
        select = false;
        page = 2; //Subreddit menu
      }
      break;

    //Choose preset sub
    case 4:
      if (up) {
        up = false;
        menu_item--;
        if (menu_item == 0)
          menu_item = 5;
      }

      else if (down) {
        down = false;
        menu_item++;
        if (menu_item == 6)
          menu_item = 1;
      }

      else if (select) {
        select = false;
        switch (menu_item) {
          case 1:
            sub = preset_subs[0];
            break;
          case 2:
            sub = preset_subs[1];
            break;
          case 3:
            sub = preset_subs[2];
            break;
          case 4:
            sub = preset_subs[3];
            break;
          case 5:
            sub = preset_subs[4];
            break;
          case 6:
            sub = preset_subs[5];
            break;
        }
        page = 2; //Subreddit menu
        menu_item = 1;
      }
      break;

    //Case 5 (scrape) is skipped because it is called in the menu() function. It is not here because buttons are not supposed to function here

    //Options
    case 6:
      if (up) {
        up = false;
        menu_item--;
        if (menu_item == 0)
          menu_item = 4;
      }

      else if (down) {
        down = false;
        menu_item++;
        if (menu_item == 5)
          menu_item = 1;
      }

      else if (select) {
        select = false;
        switch (menu_item) {
          case 1:
            page = 7; //More options
            menu_item = 1;
            break;
            
          case 2:
            page = 9; //Change contrast
            break;
            
          case 3:
            if (backlight) {
              backlight = false;
              digitalWrite(7, HIGH);
            }
            else {
              backlight = true;
              digitalWrite(7, LOW);
            }
            menu_item = 3;
            break;
            
          case 4:
            page = 1; //Main menu
            menu_item = 1;
            break;
        }
      }
      break;

    //More options
    case 7:
      if (up) {
        up = false;
        menu_item--;
        if (menu_item == 0)
          menu_item = 3;
      }

      else if (down) {
        down = false;
        menu_item++;
        if (menu_item == 4)
          menu_item = 1;
      }

      else if (select) {
        select = false;

        switch(menu_item) {
          case 1:
            page = 8;
            menu_item = 1;
            break;
            
          case 2:
            if (readability)
              readability = false;
            else
              readability = true;
            break;
            
          case 3:
            page = 6;   // Leads back to opitons
            menu_item = 1;
            break;
        }
      }
      break;

    //Num of results
    case 8:
      if (up) {
        up = false;
        if (num_results < 25)
          num_results++;
      }

      else if (down) {
        down = false;
        if (num_results > 1)
          num_results--;
      }

      else if (select) {
        select = false;
        menu_item = 1;
        page = 7; //More options
      }
      break;

    //Change contrast
    case 9:
      if (up) {
        up = false;
        contrast++;
        display.setContrast(contrast);
      }

      else if (down) {
        down = false;
        contrast--;
        display.setContrast(contrast);
      }

      else if (select) {
        select = false;
        page = 6; //Options
        menu_item = 1;
      }
      break;
  }
}

void menu() {
  //Switch-case statements to determine which page is currently being viewed
  switch (page) {
    //Main
    case 1:
      display.setTextSize(1);
      display.clearDisplay();
      display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 0);
      display.print("Reddit Scraper");
      display.drawFastHLine(0, 10, 83, BLACK);

      if (menu_item == 1)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 15);
      display.print("Choose Sub");

      if (menu_item == 2)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 25);
      display.print("Scrape");

      if (menu_item == 3)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 35);
      display.print("Options");
      display.display();
      break;

    //Subreddit menu
    case 2:
      display.setTextSize(1);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextColor(BLACK, WHITE);
      display.print("Current sub:");
      display.setCursor(0, 8);
      display.print(sub);

      if (menu_item == 1)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 18);
      display.print("Type sub");

      if (menu_item == 2)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 28);
      display.print("Choose preset");

      if (menu_item == 3)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 38);
      display.print("Exit");
      display.display();
      break;

    //Type sub
    case 3:
      display.setTextSize(2);
      display.clearDisplay();
      display.setCursor(0, 5);
      display.setTextColor(BLACK, WHITE);
      display.print(chars[letter]);
      display.setCursor(0, 25);
      display.setTextSize(1);
      display.print(sub);
      display.display();
      break;

    //Choose preset sub
    case 4:
      display.setTextSize(1);
      display.clearDisplay();

      if (menu_item == 1)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 0);
      display.print(preset_subs[0]);

      if (menu_item == 2)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 10);
      display.print(preset_subs[1]);

      if (menu_item == 3)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 20);
      display.print(preset_subs[2]);

      if (menu_item == 4)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 30);
      display.print(preset_subs[3]);

      if (menu_item == 5)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 40);
      display.print(preset_subs[4]);
      display.display();
      break;

    //Scrape
    case 5:
      scrape();
      break;

    //Options
    case 6:
      display.setTextSize(1);
      display.clearDisplay();

      if (menu_item == 1)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);

      display.print("More options");
      if (menu_item == 2)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 10);
      display.print("Contrast: ");
      display.print(contrast);

      if (menu_item == 3)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 20);
      display.print("Backlight: ");
      if (backlight)
        display.print("ON");
      else
        display.print("OFF");

      if (menu_item == 4)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 30);
      display.print("Back");
      display.display();
      break;

    //Number of results
    case 8:
      display.setTextSize(1);
      display.clearDisplay();
      display.setTextColor(BLACK, WHITE);
      display.print("Number of");
      display.setCursor(0, 8);
      display.print("results");
      display.setCursor(0, 25);
      display.setTextSize(2);
      display.print(num_results);
      display.display();
      break;

    //Change contrast
    case 9:
      display.setTextSize(1);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextColor(BLACK, WHITE);
      display.print("Change");
      display.setCursor(0, 10);
      display.print("contrast");
      //display.drawFastHLine(0, 20, 61, BLACK);
      //display.drawFastVLine(60,0,20,BLACK);
      display.setTextSize(2);
      display.setCursor(20, 30);
      display.print(contrast);

      if (contrast != 50) {
        display.drawCircle(60, 15, 10, BLACK); //Circle
        display.drawPixel(57.5, 10, BLACK); //Left eye
        display.drawPixel(62.5, 10, BLACK); //Right eye
        display.drawFastVLine(60, 25, 40, BLACK); //Body
        if (test <= 40) {
          display.drawLine(60, 30, 50, 40, BLACK); //Left arm down
          display.drawLine(60, 30, 70, 40, BLACK); //Right arm down
          test++;
        }

        else if (test <= 80) {
          display.drawLine(60, 30, 50, 27, BLACK); //Left arm up
          display.drawLine(60, 30, 70, 27, BLACK); //Right arm up
          test++;
        }

        if (test == 80)
          test = 0;
        display.drawRect(57.5, 17.5, 5, 5, BLACK); //Mouth facing left
        display.display();
      }
      else {
        display.drawBitmap(40, 10, pic, 48, 40, BLACK);
        display.display();
      }
      break;

    //More options - lead to num of results and increase scraper readability
    case 7:
      display.setTextSize(1);
      display.clearDisplay();
      display.setCursor(0, 0);
      if (menu_item == 1)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.print("Num of results: ");
      display.print(num_results);

      if (menu_item == 2)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 18);
      display.print("EZ Read: ");
      if (readability)
        display.print("ON");
      else
        display.print("OFF");

      if (menu_item == 3)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 28);
      display.print("Back");
      display.display();
      break;
  }
}

void check_down_button() {
  if (down_button_state != lastdown_button_state) {
    if (down_button_state == 0)
      down = true;
    delay(50);
  }
  lastdown_button_state = down_button_state;
}

void check_up_button() {
  if (up_button_state != lastup_button_state) {
    if (up_button_state == 0)
      up = true;
    delay(50);
  }
  lastup_button_state = up_button_state;
}

void check_select_button() {
  if (select_button_state != lastselect_button_state) {
    if (select_button_state == 0)
      select = true;
    delay(50);
  }
  lastselect_button_state = select_button_state;
}

void scrape() {
  display.setTextSize(1);
  display.clearDisplay();
  display.print("Scraping:");
  display.setCursor(0, 10);
  display.print(sub);
  display.display();
  Serial.print(String(num_results) + "\n");
  Serial.print(sub + "\n");
  if (readability)
    Serial.print("true\n");
  else
    Serial.print("false\n");
  delay(1000);
  display.clearDisplay();

  char input;
  String line1;
  String line2;
  String line3;
  String line4;
  String score;
  while (1) {
    if (Serial.available()) {
      if (line1.length() < 14) {
        input = Serial.read();
        line1 += input;
        display.setCursor(0, 0);
        display.print(line1);
        Serial.flush();
        display.display();
      }

      else if (line2.length() < 14) {
        input = Serial.read();
        line2 += input;
        display.setCursor(0, 10);
        display.print(line2);
        Serial.flush();
        display.display();
      }

      else if (line3.length() < 14) {
        input = Serial.read();
        line3 += input;
        display.setCursor(0, 20);
        display.print(line3);
        Serial.flush();
        display.display();
      }

      else if (line4.length() < 14) {
        input = Serial.read();
        line4 += input;
        display.setCursor(0, 30);
        display.print(line4);
        Serial.flush();
        display.display();
      }

      else {
        input = Serial.read();
        score += input;
        display.setCursor(0, 40);
        display.print(score);
        Serial.flush();
        display.display();
      }
    }

    if (score.length() == 15) {
      delay(2000);
      display.clearDisplay();
      line1 = "";
      line2 = "";
      line3 = "";
      line4 = "";
      score = "";
      display.display();
    }

    if (line1 == "exit25") {
      page = 1;
      return;
    }

    else if (line1 == "exit26") {
      display.setTextSize(1);
      display.clearDisplay();
      display.setTextColor(BLACK, WHITE);
      display.print("Unable to find");
      display.setCursor(0, 10);
      display.print(sub);
      display.display();
      delay(2000);
      page = 1;
    }
  }
}

void instantiate_vars() {
  chars[0] = 'a';
  chars[1] = 'b';
  chars[2] = 'c';
  chars[3] = 'd';
  chars[4] = 'e';
  chars[5] = 'f';
  chars[6] = 'g';
  chars[7] = 'h';
  chars[8] = 'i';
  chars[9] = 'j';
  chars[10] = 'k';
  chars[11] = 'l';
  chars[12] = 'm';
  chars[13] = 'n';
  chars[14] = 'o';
  chars[15] = 'p';
  chars[16] = 'q';
  chars[17] = 'r';
  chars[18] = 's';
  chars[19] = 't';
  chars[20] = 'u';
  chars[21] = 'v';
  chars[22] = 'w';
  chars[23] = 'x';
  chars[24] = 'y';
  chars[25] = 'z';
  preset_subs[0] = "popular";
  preset_subs[1] = "jokes";
  preset_subs[2] = "askreddit";
  preset_subs[3] = "overwatch";
  preset_subs[4] = "pics";
  preset_subs[5] = "random";
}



