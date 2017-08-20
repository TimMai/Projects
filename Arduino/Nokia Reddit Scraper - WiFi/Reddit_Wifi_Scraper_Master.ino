#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>

//Followed https://www.youtube.com/watch?v=VtZvf5T98FI as a basis for the menu code

/*
  Old layout
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
  Use internal pullup resistors on PIN 2, 12, and 8.
*/

/*
  New layout
  8 - GND
  7 - 2
  6 - 3.3 V
  5 - 3
  4 - 4
  3 - 5
  2 - 6
  1 - 7

  UP - 13
  DOWN - 12
  SELECT - 11
*/

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
bool ez_read = true;

int menu_item = 1;      //Current item on the menu
int page = 1;           //Current page
String sub = "popular"; //Current selected sub
char * preset_subs[6];  //Array of preset subs
int letter = 0;         //Keep track of current letter when typing
char chars[26];         //Characters A-Z

int test = 0; //Testing with the GFX library on the contrast page

//Keep track of keys
int temp_up_button = 13;
int temp_down_button = 12;
int temp_select_button = 11;
int up_button = 13;
int down_button = 12;
int select_button = 11;

//WIFI additions
int id = 0;
bool cont = false;
String wire_request = "";
int global_pulse = 1;


/*
  Pages: main, sub, type, scrape, options, change_contrast, change_num, choose_preset
  main - choose sub, scrape, or go to options
  sub - type in a subreddit or choose from a preset sub
  type - type in a subreddit
  choose_preset - choose a preset sub
  options - leads to more options, change the contrast, turn on/off the backlight
  more_options - leads to change number of scraped results and turning on/off EZ Read
  change_num - change number of scraped results
  change_contrast - change contrast

  name -> number
  main - 1
  subreddit menu - 2
  type sub - 3
  choose preset sub - 4
  scrape - 5
  options - 6
  more options - 7
  num of results - 8
  change contrast - 9
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


Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

void menu();  //Render the menu system
void check_down_button(); //Check status of button12
void check_up_button();   //Check status of button2
void check_select_button(); //Check status of button8
void scrape();  //Scrape the subreddit


void setup() {
  Wire.begin();
  pinMode(up_button, INPUT_PULLUP);   //Up
  pinMode(down_button, INPUT_PULLUP);  //Down
  pinMode(select_button, INPUT_PULLUP);   //Select
  pinMode(2, OUTPUT);         //Connect to backlight functionality

  digitalWrite(2, LOW); //Turn Backlight ON

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

  wire_request = "RequestID";
  Wire.beginTransmission(1);
  Wire.write(wire_request.c_str());
  Wire.endTransmission();

  Wire.requestFrom(1, 3);
  Serial.println("Requesting ID...");
  while (!cont) {
    receiveEvent();
  }
  Serial.println("ID: " + String(id));
  cont = false;

  delay(2000);
  display.clearDisplay();
  instantiate_vars();
}

void loop() {
  menu();
  check_buttons();

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
        if (sub == "")  //Do not allow empty entries
          sub = "random";
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
            page = 7; //More options
            menu_item = 1;
            break;

          case 2:
            page = 9; //Change contrast
            break;

          case 3:
            if (backlight) {
              backlight = false;
              digitalWrite(2, HIGH);
            }
            else {
              backlight = true;
              digitalWrite(2, LOW);
            }
            menu_item = 3;
            break;

          case 4:
            page = 10;
            menu_item = 1;
            break;

          case 5:
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

        switch (menu_item) {
          case 1:
            page = 8;
            menu_item = 1;
            break;

          case 2:
            if (ez_read)
              ez_read = false;
            else
              ez_read = true;
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

    //Key bindings
    case 10:
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
            while (1) {
              check_buttons();

              if (up) {
                up = false;
                temp_up_button = up_button;
                break;
              }

              else if (down) {
                down = false;
                temp_up_button = down_button;
                break;
              }

              else if (select) {
                select = false;
                temp_up_button = select_button;
                break;
              }
            }
            break;

          case 2:
            while (1) {
              check_buttons();
              if (up) {
                up = false;
                temp_down_button = up_button;
                break;
              }

              else if (down) {
                down = false;
                temp_down_button = down_button;
                break;
              }

              else if (select) {
                select = false;
                temp_down_button = select_button;
                break;
              }
            }
            break;

          case 3:
            while (1) {
              check_buttons();
              if (up) {
                up = false;
                temp_select_button = up_button;
                break;
              }

              else if (down) {
                down = false;
                temp_select_button = down_button;
                break;
              }

              else if (select) {
                select = false;
                temp_select_button = select_button;
                break;
              }
            }
            break;

          case 4:
            up_button = 2;
            temp_up_button = 2;
            down_button = 12;
            temp_down_button = 12;
            select_button = 8;
            temp_select_button = 8;
            break;

          case 5:
            if (temp_up_button == temp_down_button || temp_up_button == temp_select_button || temp_down_button == temp_select_button) {
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(BLACK, WHITE);
              display.setCursor(0, 10);
              display.print("Error");
              display.setCursor(0, 20);
              display.print("Overlapping");
              display.setCursor(0, 30);
              display.print("key bindings");
              display.display();
              delay(2000);
              menu_item = 1;
              break;
            }

            else {
              page = 6;
              menu_item = 1;
              up_button = temp_up_button;
              down_button = temp_down_button;
              select_button = temp_select_button;
              break;
            }
        }
      }
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
      display.setCursor(0, 0);
      display.setTextColor(BLACK, WHITE);
      display.print(chars[letter]);
      display.setCursor(0, 20);
      display.setTextSize(1);
      display.print(sub);
      display.setCursor(0, 30);
      display.print("'blank' not");
      display.setCursor(0, 40);
      display.print("allowed");
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
      display.print("Key bindings");

      if (menu_item == 5)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 40);
      display.print("Back");
      display.display();
      break;

    //More options - lead to num of results and increase scraper ez_read
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
      if (ez_read)
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
          test = 0; //Reset the counter

        display.drawRect(57.5, 17.5, 5, 5, BLACK); //Mouth facing left
        display.display();
      }
      else {
        display.drawBitmap(40, 10, pic, 48, 40, BLACK);
        display.display();
      }
      break;

    //Key bindings
    case 10:
      display.setTextSize(1);
      display.clearDisplay();
      display.setCursor(0, 0);

      if (menu_item == 1)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.print("Up: " + String(temp_up_button));
      display.setCursor(0, 10);

      if (menu_item == 2)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.print("Down: " + String(temp_down_button));
      display.setCursor(0, 20);

      if (menu_item == 3)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.print("Select: " + String(temp_select_button));
      display.setCursor(0, 30);

      if (menu_item == 4)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.print("Reset bindings");

      display.setCursor(0, 40);
      if (menu_item == 5)
        display.setTextColor(WHITE, BLACK);
      else
        display.setTextColor(BLACK, WHITE);
      display.print("Back");
      display.display();
      break;

  }
}

void check_buttons() {
  down_button_state = digitalRead(down_button);
  select_button_state = digitalRead(select_button);
  up_button_state = digitalRead(up_button);

  check_down_button();
  check_up_button();
  check_select_button();
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
  display.print("Sending");
  display.setCursor(0, 10);
  display.print("params...");
  display.display();

  //Occurs when beginning scrape BEGIN
  Serial.println("Send params...");
  wire_request = "SendParams";
  Wire.beginTransmission(1);
  Wire.write(wire_request.c_str());
  Wire.endTransmission();
  delay(100);

  //Send param1 - sub
  wire_request = sub;
  Wire.beginTransmission(1);
  Wire.write(wire_request.c_str());
  Wire.endTransmission();
  delay(100);

  //Send param2 - num_results
  wire_request = String(num_results);
  Wire.beginTransmission(1);
  Wire.write(wire_request.c_str());
  Wire.endTransmission();
  delay(100);

  //Send param3 - id
  wire_request = String(id);
  Wire.beginTransmission(1);
  Wire.write(wire_request.c_str());
  Wire.endTransmission();
  delay(100);

  //Send param4 - ez_read
  if (ez_read)
    wire_request = "true";
  else
    wire_request = "false";
  Wire.beginTransmission(1);
  Wire.write(wire_request.c_str());
  Wire.endTransmission();
  delay(10000);

  //Wait for the OK that the params have been sent
  Serial.println("Is it OK?");
  wire_request = "SendOK";  //Don't need to send this message because it automatically does it on the slave size
  Wire.requestFrom(1, 2);
  while (!cont) {
    receiveEvent();
  }
  cont = false;
  display.clearDisplay();

  /*
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Retrieving");
    display.setCursor(0, 10);
    display.print("results...");
    display.display();
    //Occurs when scraping BEGIN
    Serial.println("Get the results");
    wire_request = "GetResults";
    Wire.beginTransmission(1);
    Wire.write(wire_request.c_str());
    Wire.endTransmission();
    delay(15000);
    display.clearDisplay();
  */

  //First see if the results and params have matching IDs
  while (!cont) {
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Retrieving");
    display.setCursor(0, 10);
    display.print("results...");
    display.display();
    //Occurs when scraping BEGIN
    Serial.println("Get the results");
    wire_request = "GetResults";
    Wire.beginTransmission(1);
    Wire.write(wire_request.c_str());
    Wire.endTransmission();
    delay(15000);
    display.clearDisplay();

    Wire.requestFrom(1, 2);
    receiveEvent();
    delay(4000);
    //Also allow the user to end the scrape prematurely
  }
  cont = false;
  Serial.println("Past phase 0");

  for (int pulse = 1; pulse < num_results * 3; pulse++) {
    //Serial.println("Waiting for pulse " + String(pulse));
    while (!cont) {
      if (pulse % 3 != 0)
        Wire.requestFrom(1, 28);
      else
        Wire.requestFrom(1, 15);
      receiveEvent();
      delay(2500);
    }
    Serial.println("Next phase");
    cont = false;
  }
  wire_request = "";
  id++;

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

void receiveEvent() {
  String input;

  while (Wire.available()) {
    char c = Wire.read();
    input += c;
  }

  if (input == "") {
    cont = false;
    return;
  }

  if (wire_request == "RequestID") {
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == ' ') {
        input = input.substring(0, i);
      }
    }
    id = input.toInt();
    cont = true;
    wire_request = "";
    return;
  }

  else if (wire_request == "SendOK" && input == "OK") {
    Serial.println("Got the OK");
    wire_request = "";
    cont = true;
    return;
  }

  else if (wire_request == "GetResults" && input == "OK") {
    Serial.println("IDs match.");
    cont = true;
    return;
  }

  else if (wire_request == "GetResults" && input == "NO") {
    Serial.println("IDs do not match.");
    cont = false;
    return;
  }

  else if (wire_request == "GetResults") {
    //Displaying on screen
    Serial.println(input);

    if (global_pulse % 3 == 1) {
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.print(input.substring(0, 14));
      display.setCursor(0, 10);
      display.print(input.substring(14, 28));
      display.display();
      global_pulse++;
    }

    else if (global_pulse % 3 == 2) {
      display.setCursor(0, 20);
      display.print(input.substring(0, 14));
      display.setCursor(0, 30);
      display.print(input.substring(14, 28));
      display.display();
      global_pulse++;
    }

    else {
      display.setCursor(0, 40);
      display.print(input);
      display.display();
      delay(1250);
      display.clearDisplay();
      global_pulse++;
    }

    page = 1;
    cont = true;
    return;
  }
}
