Continuation of the 16x2 LCD Reddit Scraper but with a larger LCD screen and a navigable menu.

Uses a Nokia 5110 LCD and a three button setup with no pulldown resistors (uses pullup resistor in the Arduino).
Down button - Pin 12
Up button - Pin 2
Select button - Pin 8

When typing:
	Up button advances current letter
	Down button confirms current letter
	Select confirms the entire collections of letters (i.e. the typed subreddit/word)

The program has various pages:
Menu - Leads to "Choose Sub", "Scrape", "Options"
Choose Sub - Displays the current subreddit; also allows the user to type in a new subreddit or select one
Scrape - Scrapes the subreddit
Options - Change the number of scraped results, contrast of the LCD, and turn on/off the backlight.

The screen can hold roughly 14x4 characters. Special characters will cause errors when scraping (display will not operate properly).
Python code was modified slightly to allow the user to change the number of scraped results.

Followed https://www.youtube.com/watch?v=VtZvf5T98FI as a basis for the menu code

Run Python code when the Arduino is connected