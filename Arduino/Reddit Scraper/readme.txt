Extension of the Reddit Scraper with some modifications to the Python code to account for serial input and output. 
Created with Arduino Uno.

Project allows the user to scrape a reddit sub from an LCD connected to an Arduino which is connected to a computer. 
The Select version allows the user to select from a set of subreddits to scrape from. Subreddits can be added by modifying the Arduino code.
The Type version allows the user to 'type' (with buttons) in a subreddit to scrape from.

Follow https://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay for LCD wiring
Two button setup - both are separate from each other.
Button has one side connected to power. Other side has 10k resistor connected to ground and a wire before that connecting to a digital pin.
One button is connected to digital pin 8. The other is connected to digital pin 7. 

Select version:
	Button8 - Advance the option forward when selecting a subreddit
	Button7 - Confirm selection
	
Type version: 
	Button8 - Change the current letter to the next one in alphabetical order.
	Button7 - Confirm the typed letter on the screen. Moves the cursor to the next position to type another letter with Button8.
	Button8 and Button7 - Confirm the entire word/subreddit to be scraped. The last letter which appears is not included in the word.
	
To run: Connect Arduino to computer, upload Arduino code, and run Python code.

Reddit titles which are too long for the screen are limited (16 chars).