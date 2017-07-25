#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
//Followed https://www.youtube.com/watch?v=VtZvf5T98FI as a basis for the menu code
/*
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

boolean backlight = true;
int contrast = 42;

int num_results = 10;   //Number of results that will be displayed
int menuitem = 1;       //Current item on the menu
String page = "main";   //Current page
String sub = "popular"; //Current selected sub
String preset_subs[6];  //Array of preset subs
int letter = 0;         //Keep track of current letter when typing
char chars[26];         //Characters A-Z

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

void loop() {
  menu();

  down_button_state = digitalRead(12);
  select_button_state = digitalRead(8);
  up_button_state = digitalRead(2);

  check_down_button();
  check_up_button();
  check_select_button();

  //If-else statements to determine which page is currently being viewed

  if (page == "main") {
    if (up) {
      up = false;
      menuitem--;
      if (menuitem == 0)
        menuitem = 3;
    }
    else if (down) {
      down = false;
      menuitem++;
      if (menuitem == 4)
        menuitem = 1;
    }

    else if (select) {
      select = false;
      if (menuitem == 1) {
        page = "sub";
        menuitem = 1;
      }

      else if (menuitem == 2) {
        page = "scrape";
        menuitem = 1;
      }

      else if (menuitem == 3) {
        page = "options";
        menuitem = 1;
      }
    }
  }

  else if (page == "sub") {
    if (up) {
      up = false;
      menuitem--;
      if (menuitem == 0)
        menuitem = 3;
    }

    else if (down) {
      down = false;
      menuitem++;
      if (menuitem == 4)
        menuitem = 1;
    }

    else if (select) {
      select = false;
      if (menuitem == 1) {
        page = "type";
        menuitem = 1;
        sub = "";
        letter = 0;
      }

      else if (menuitem == 2) {
        page = "choose_preset";
        menuitem = 1;
      }
      else if (menuitem == 3) {
        page = "main";
        menuitem = 1;
      }
    }
  }

  else if (page == "type") {
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
      page = "sub";
    }
  }

  else if (page == "options") {
    if (up) {
      up = false;
      menuitem--;
      if (menuitem == 0)
        menuitem = 4;
    }

    else if (down) {
      down = false;
      menuitem++;
      if (menuitem == 5)
        menuitem = 1;
    }

    else if (select) {
      select = false;
      if (menuitem == 1) {
        page = "change_num";
        menuitem = 1;
      }

      else if (menuitem == 2) {
        page = "change_contrast";
        menuitem = 1;
      }

      else if (menuitem == 3) {
        if (backlight) {
          backlight = false;
          digitalWrite(7, HIGH);
        }

        else {
          backlight = true;
          digitalWrite(7, LOW);
        }
      }
      else if (menuitem == 4) {
        page = "main";
        menuitem = 1;
      }
    }
  }

  else if (page == "change_contrast") {
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
      page = "options";
      menuitem = 1;
    }
  }

  else if (page == "change_num") {
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
      menuitem = 1;
      page = "options";
    }
  }

  else if (page == "choose_preset") {
    if (up) {
      up = false;
      menuitem--;
      if (menuitem == 0)
        menuitem = 5;
    }

    else if (down) {
      down = false;
      menuitem++;
      if (menuitem == 6)
        menuitem = 1;
    }

    else if (select) {
      select = false;
      switch (menuitem) {
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
      page = "sub";
      menuitem = 1;
    }
  }
}

void menu() {
  //If-else statements to determine which page is currently being viewed
  if (page == "main") {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 0);
    display.print("Reddit Scraper");
    display.drawFastHLine(0, 10, 83, BLACK);
    display.setCursor(0, 15);

    if (menuitem == 1)
      display.setTextColor(WHITE, BLACK);

    else
      display.setTextColor(BLACK, WHITE);

    display.print("Choose Sub");

    display.setCursor(0, 25);

    if (menuitem == 2)
      display.setTextColor(WHITE, BLACK);

    else
      display.setTextColor(BLACK, WHITE);

    display.print("Scrape");

    if (menuitem == 3)
      display.setTextColor(WHITE, BLACK);

    else
      display.setTextColor(BLACK, WHITE);

    display.setCursor(0, 35);
    display.print("Options");
    display.display();
  }

  //Select sub
  else if (page == "sub") {
    display.setTextSize(1);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(BLACK, WHITE);
    display.print("Current sub:");
    display.setCursor(0, 8);
    display.print(sub);

    if (menuitem == 1)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);

    display.setCursor(0, 18);
    display.print("Type sub");

    if (menuitem == 2)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 28);
    display.print("Choose preset");

    if (menuitem == 3)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);

    display.setCursor(0, 38);
    display.print("Exit");
    display.display();
  }

  //Type
  else if (page == "type") {
    display.setTextSize(2);
    display.clearDisplay();
    display.setCursor(0, 5);
    display.setTextColor(BLACK, WHITE);
    display.print(chars[letter]);
    display.setCursor(0, 25);
    display.setTextSize(1);
    display.print(sub);
    display.display();
  }

  else if (page == "scrape")
    scrape();

  else if (page == "options") {
    display.setTextSize(1);
    display.clearDisplay();

    if (menuitem == 1)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);

    display.print("Num of results: ");
    display.print(num_results);
    if (menuitem == 2)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 18);
    display.print("Contrast: ");
    display.print(contrast);

    if (menuitem == 3)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 28);
    display.print("Backlight: ");
    if (backlight)
      display.print("ON");
    else
      display.print("OFF");

    if (menuitem == 4)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 38);
    display.print("Back");
    display.display();
  }

  else if (page == "change_contrast") {
    display.setTextSize(2);
    display.clearDisplay();
    display.setCursor(5, 15);
    display.setTextColor(BLACK, WHITE);
    display.print(contrast);
    display.display();
  }

  else if (page == "change_num") {
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
  }

  else if (page == "choose_preset") {
    display.setTextSize(1);
    display.clearDisplay();

    if (menuitem == 1)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 0);
    display.print(preset_subs[0]);

    if (menuitem == 2)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 10);
    display.print(preset_subs[1]);

    if (menuitem == 3)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 20);
    display.print(preset_subs[2]);

    if (menuitem == 4)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 30);
    display.print(preset_subs[3]);

    if (menuitem == 5)
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 40);
    display.print(preset_subs[4]);
    display.display();
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
        display.setCursor(0,30);
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
      page = "main";
      return;
    }
  }
}

