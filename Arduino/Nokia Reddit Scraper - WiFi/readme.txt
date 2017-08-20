WiFi Nokia Reddit Scraper - by Tim Mai (shiperl@ufl.edu)


GENERAL INFORMATION
-----------------------------------
This project allows the user to scrape a subreddit to look at the top articles and their scores. This can be done wirelessly without 
any connection to a computer (only needs a 1 V, 5 A adapter - a standard cell phone adapter). A computer can be used to be a power souorce
and for debugging through the serial monitor. 

Main components:
	Arduino UNO 		(referred as slave for I2C)
	Arduino UNO WiFi	(referred as master for I2C)
	Nokia 5110 LCD
	Three buttons

Note: Slave uses WiFi link firmware (https://github.com/arduino-org/arduino-library-wifilink) instead of the default ESP-Link
	
The master sends the scraping parameters to the slave, and the slave sends these parameters to a ThingSpeak server (params server).
A Python script checks periodically when a new parameter has been posted to the server. When a new entry has been found, the script is ran
and the results are posted to another ThingSpeak server (results server). The slave then retrieves these results from the results server
and sends the data to the master. The master then displays the data on the screen.

To run the project, start up the the Python script and the slave. Then start up the master after ~15 seconds or more.

The startup screen should appear and disappear once initial data has been retrieved from the slave.
If that does not happen, several issues may be at play but it can be likely solved by:
	reset the slave and then after ~15 seconds reset the master
Either that or a WiFi connection was not established.

For information about the menu system and basis of the code itself (without the WiFi component), see the Nokia Reddit Scraper project.


LAYOUTS
----------------------------------	
LCD Pin Layout - LCD to Arduino UNO:
  8 - GND
  7 - 2
  6 - 3.3 V
  5 - 3
  4 - 4
  3 - 5
  2 - 6
  1 - 7
  
I2C Layout - Slave to Master
  A5 - A5
  A4 - A4
  GND - GND
  
See https://www.arduino.cc/en/Reference/Wire for more info

Down button - Master Pin 13
Up button - Master Pin 12
Select button - Master Pin 11


MORE DETAILS ABOUT PROJECT
-----------------------------------
Parameters that are sent include:
  sub
  num of results
  ez_read
  id

ID is used a unique number that is used to identify if the results retrieved correspond to the parameters sent.

Results include the ID and the actual content (the results themselves) are split into five fields.
Each field contains three articles. The fields were split up due to the 255 char limit for each ThingSpak field.

On slave startup, it retrieves the latest params from the params server. The ID is incremented by 1. This ID is sent
to the master when the master requests it on startup. The increment is to insure that the ID will be unique (the Python
script increments the latest ID as well).

When the master calls the scrape function, the params are sent to the slave and waits to get the OK that the params
have been posted to the params server. The slave posts the params and sends the OK to the master. The master then
tells the slave to get the reuslts, and the slave sends a get request to the results server. At the same time,
the Python script scrapes the results and posts it to the results server. The slave may not immediately get the 
correct results so it wait until it occurs and sends the data in 'pulses' to the master. Pulses are defined as a set of
bytes which will be sent at once to the master since there is a limit on how many bytes can be sent to the master (32 byte limit).

Every 1st and 2nd pulse contain 28 bytes (56 bytes total) which will consist of the four lines that will be displayed on the LCD.
The first two lines are in the first pulse, the last two lines are in the second pulse. The third pulse consists of 16 bytes which
consists of the score. The maximum number of pulses is the number of results * 3. 


NOTES
------------------------------
If it were possible, the entire project would have been done on the Arduino UNO WiFi device. However, there was
not sufficient dynamic memory space on it (only 2K bytes available). Therefore two devices and I2C had to be used.
The project became more complicated as a result due to the necessary synchronization of the Arduino-to-Arduino
communication and various I2C issues. Even with the two microcontrollers, the master still had memory problems 
and the code had to be tweaked slightly for optimization (specifically replacing strings with char arrays when possible. 
The slave has sufficient space for more variables/data.

Additionally, the Arduino UNO WiFi is not very well supported and caused issues when first exploring it in the beginning.
There were some issues in determining the best firmware (ESP or WiFi-Link) and overall the WiFi-link was chosen due to its
slightly better library support than the ESP (which has poor documentation of its libraries).

