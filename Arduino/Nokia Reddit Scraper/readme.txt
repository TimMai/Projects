Followed https://www.youtube.com/watch?v=VtZvf5T98FI as a basis for the menu code

Continuation of the 16x2 LCD Reddit Scraper but with a larger LCD screen and a navigable menu.

Uses a Nokia 5110 LCD and a three button setup with no pulldown resistors (uses pullup resistor in the Arduino).
Down button - Pin 12
Up button - Pin 2
Select button - Pin 8

When typing:
	Up button advances current letter
	Down button confirms current letter
	Select confirms the entire collections of letters (i.e. the typed subreddit/word)

When scraping and within two seconds after a result is displayed:
	Up button quits the scraping process
	Down button pauses the scraping process. Press down again to resume scraping


The program has various pages:
Menu - Leads to "Choose Sub", "Scrape", "Options"
Choose Sub - Displays the current subreddit; also allows the user to type in a new subreddit or select one
Scrape - Scrapes the subreddit
Options - Leads to "More Options"; also allows user to change contrast of the LCD and turn on/off the backlight.
More Options - Change the number of displayed results and enable/disable EZ Read

EZ Read syllabifies words that are split into two lines because the word is too long. It then determines if the word should be
moved to the next line entirely or if a hyphen can be correctly placed on the initial line to properly split a word into two.
EZ Read should basically emulate how a book would be written. EZ Read also removes spaces from the beginning of lines 2, 3, and 4.

With EZ Read disabled, more text can fit onto the screen but there will be no hyphenation; text will appear as is from Reddit.


The screen can hold roughly 14x5 characters, but only the first four lines are used for displaying the Reddit article title.
The last line is used to display the score of the article.

Special characters will cause errors when scraping (display will not operate properly).

Run Python code when the Arduino is connected by USB to computer