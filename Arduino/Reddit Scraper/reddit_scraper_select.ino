#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int count = 0;
int row = 0;
String subs[5];
int sub_size = sizeof(subs)/sizeof(String);
String line0 = "";
String line1 = "";

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
}

void loop() {
  lcd.print("Reddit Scraper");
  delay(2000);
  select();
}

void select() {
  int option = 0;
  lcd.clear();
  lcd.print("Select sub:");
  lcd.setCursor(0, 1);
  lcd.print(subs[0]);
  while (digitalRead(7) == LOW) {
    if (digitalRead(8) == HIGH) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      option++;
      if (option == sub_size) {
        option = 0;
      }
      lcd.print(subs[option]);
      delay(500);
    }
  }
  lcd.clear();
  lcd.print("Now scraping:");
  lcd.setCursor(0, 1);
  lcd.print(subs[option]);
  Serial.print(subs[option] + "\n");
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


