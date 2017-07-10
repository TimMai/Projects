#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int count = 0;
int row = 0;
String subs[5];
int sub_size = sizeof(subs)/sizeof(String);
String line0 = "";
String line1 = "";
char chars[26];

void setup() {
  //Begins the serial connection
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(8, INPUT);
  pinMode(7, INPUT);
  subs[0] = "popular";
  subs[1] = "leagueoflegends";
  subs[2] = "overwatch";
  subs[3] = "askreddit";
  subs[4] = "jokes";
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
}

void loop() {
  lcd.print("Reddit Scraper");
  delay(2000);
  select();
}

void select() {
  int option = 0;
  int pos = 0;
  String sub = "";
  lcd.clear();
  lcd.print("Type sub:");
  lcd.setCursor(pos, 1);
  lcd.print(chars[option]);
  while (1) {
    delay(100);
    if (digitalRead(8) == HIGH && digitalRead(7) == LOW) {
      lcd.setCursor(pos, 1);
      option++;
      if (option == 26) {
        option = 0;
      }
      lcd.print(chars[option]);
      delay(100);
    }

    if (digitalRead(7) == HIGH && digitalRead(8) == LOW) {
      sub += chars[option];
      pos += 1;
      lcd.setCursor(pos, 1);
      option = 0;
      lcd.print(chars[option]);
      delay(100);
    }

    if (digitalRead(7) == HIGH && digitalRead(8) == HIGH)
      break;
  }
  lcd.clear();
  lcd.print("Now scraping:");
  lcd.setCursor(0, 1);
  lcd.print(sub);
  Serial.print(sub + "\n");
  delay(1000);
  lcd.clear();
  char input;
  while (1) {
    if (Serial.available() > 0) {
      if (line0.length() < 16) {
        input = Serial.read();
        line0 = line0 + input;
        lcd.setCursor(0, 0);
        lcd.print(line0);
        Serial.flush();
      }
      else {
        input = Serial.read();
        line1 += input;
        lcd.setCursor(0,1);
        lcd.print(line1);
        Serial.flush();
      }
    }
    if (line1.length() == 16) {
      delay(3000);
      lcd.clear();
      line0 = "";
      line1 = "";
    }
  }
}


