from bs4 import BeautifulSoup   # Parse through html source
import pyphen                   # Syllabify words for EZ Read
import requests                 # Send request to reddit to retrieve html source
import serial                   # Allow serial communication
import time                     # Allow delay
import re                       # Regex to filter out special characters

'''
Remove any unnecessary spaces at the beginning of a line if EZ Read is On
Position is the location where any space will be removed (14,28, or 42)
'''
def remove_space(title, position):
    if position+1 < len(title) and title[position] == " ":
        title = title[:position] + title[position+1:]

    return title

'''
Remove any non-standard characters (e.g. emojis) from the title. Otherwise the display system does not work.
Score may appear on the second page and the display system becomes desynced.
Replace special chars with a space

Function basically looks at every char in the title and if it is a standard char or a space, it replaces it with 
nothing. The length of the char is then checked. If the length(char) == 0, then there is no problem. Otherwise
a space is added to the new title instead.
'''
def remove_special(line):
    new_title = ""
    for i in range(0, len(line)):
        character = re.sub(r'[A-Za-z0-9!@#$%\^&*().\-_+=\[{}\]\\|:;\"\'<>,.?/~` ]', '', line[i])
        if len(character) == 0:
            new_title += line[i]
        else:
            new_title += " "
    return new_title

'''
EZ_Read makes words more easier to read by splitting words into syllables
line_start - last position of the top line
line_end - first position of the below line
line(number) - line_start = 14*number-1, line_end = 14*number
    where number is the line number from 1 to 3
    line 4 is not included because there is no reason to hyphenate any words if there is no line 5
'''
def EZ_Read(title,line_start,line_end):
    if line_end + 1 > len(title):
        if line_start == 13:
            print("Line1 is too short")
        elif line_start == 27:
            title = remove_space(title,14)
            print("Line2 is too short")
        else:
            title = remove_space(title,28)
            print("Line3 is too short")

        return title

    if line_end != 14:
        title = remove_space(title,line_end-14)

    if line_end == 42:
        title = remove_space(title,42)

    # If a word is split, go through the syllabication process
    if (title[line_start].isalpha() and title[line_end].isalpha()):
        # Find the start and end of the split word
        start = line_start
        end = line_end
        while (title[start].isalpha()):
            start -= 1

        while (title[end].isalpha() and end != len(title) - 1):
            end += 1

        start += 1  # remove a space char
        word = title[start:end]
        print("Word:", word)
        syllables = dic.inserted(word)
        print("Syllables:", syllables)

        pos_dash = 0
        while syllables[pos_dash] != "-":
            if pos_dash == len(syllables) - 1:
                pos_dash = len(syllables) - 1
                break
            pos_dash += 1

        print("Position of dash:", pos_dash)

        # The word can be divided into syllables
        if pos_dash != len(syllables) - 1:
            # The word cuts off to the next line before a dash is encountered -- must move entire word to
            # next line
            if start + pos_dash > line_end:
                print("Case: New line")
                new_title = title[:start]
                distance = line_end - start
                spaces = 0
                while (spaces < distance):
                    new_title += " "
                    spaces += 1
                new_title += title[start:]
                print(new_title)
                title = new_title
                print("\n")

                return title

            # Word can be split into two lines because a dash is encountered before the end of the line
            else:
                print("Case: Split word")
                new_title = title[:start + pos_dash] + "-"
                distance = line_end - start - pos_dash - 1
                spaces = 0
                while spaces < distance:
                    new_title += " "
                    spaces += 1

                new_title += title[start + pos_dash:]
                print(new_title)
                title = new_title
                print("\n")

                return title

        # Word cannot be divided into syllables
        else:
            print("Case: Indivisible word")
            distance = line_end - start
            new_title = title[:start]
            spaces = 0
            while spaces < distance:
                new_title += " "
                spaces += 1
            new_title += title[start:]
            print(new_title)
            print("\n")
            title = new_title

            return title
    else:
        if line_start == 13:
            line_num = 1
        elif line_start == 27:
            line_num = 2
        else:
            line_num = 3

        print("Line" + str(line_num) + " is OK")
        return title

# pyphen initialization
pyphen.language_fallback('en_US')
dic = pyphen.Pyphen(lang='en_US')

# serial communication
ser = serial.Serial('COM3', 9600)

while(1):
    sub = ""                    #Scraped sub
    string_num = ""             #Number of results
    string_readability = ""     #Enable readability (hyphenation)

    while (string_num == ""):
        string_num = ser.readline().decode().strip()
    while(sub == ""):
        sub = ser.readline().decode().strip()
    while(string_readability == ""):
        string_readability = ser.readline().decode().strip()

    if string_readability == "true":
        readability = True
    else:
        readability = False

    num = int(string_num)
    print("Num of results", num)
    print("Sub:", sub)
    print("EZ Read:", readability)

    print("Reddit Scraper\n")

    r = requests.get("https://www.reddit.com/r/" + sub + "/", headers = {'User-agent': 'scraper_bot'})
    print("Status:", r.status_code)
    if r.status_code != 200:
        # Write to the serial monitor that there was an error finding the subreddit
        ser.write(b'exit26')

    else:
        soup = BeautifulSoup(r.text, "html.parser")

        # Grabs all the content beneath the header
        results = soup.find("div", {"class": "content", "role": "main"})

        if results:
            # Parent of the list of entries
            entry_parent = results.find("div", {"id": "siteTable", "class": "sitetable linklisting"})

            # The list of entries
            children = entry_parent.children

        x = 0
        url_list = []   # Store URLs of the comment threads
        for item in children:
            try:
                title = item.find("p", {"class": "title"}).find("a").text
                url = item.find("a").attrs["href"]
                x += 1
                if url[0] == "/":   # Entry does not direct to another site
                    url_list.append(url)
                    # Keep track of URLs for later - should revise code to only look for valid children
                    # to save space

                else:       # Entry directs to another site - need to make sure the added URL is the comment thread
                    thread = item.find("li", {"class": "first"}).find("a").attrs["href"]
                    url_list.append(thread)

                score = item.find("div", {"class": "score unvoted"}).text
                print("Title" + str(x) + ": " + title)

                # Add a space to deal with a titles that have only a word between two lines and nothing after the word
                # Prevent any errors when the end of the split word is trying to be determined (doesn't go out of bounds)
                # Example:
                # Line1: The comp
                # Line2: uter
                title += " "
                title = remove_special(title)

                # If the user wants EZ Read
                print(len(title))
                if(readability):
                    title = EZ_Read(title,13,14)
                    title = EZ_Read(title,27,28)
                    title = EZ_Read(title,41,42)

                # Add spaces so that the program knows when the end of a result is reached
                # 56 is the number of total characters possible for 4 lines. 14 characters each
                while len(title) < 56:
                    title += " "

                for i in range(0, 56):
                    ser.write(title[i].encode())
                    time.sleep(.01)

                try:
                    print("URL:", url)
                except:
                    print("Unable to obtain URL")

                try:
                    print("Score:", score)
                    score = str(score)
                    score = re.sub(r'[^0-9\.k]', '', score)
                    if len(score) == 0:
                        score = "unknown"
                    score = "Score" + str(x) + ": " + score

                    while len(score) < 15:
                        score += " "

                    for i in range(0, 15):
                        ser.write(score[i].encode())
                        time.sleep(.01)

                except:
                    print("Unable to obtain score")
                    score = "unknown"
                    while len(score) < 15:
                        score += " "
                    for i in range (0,15):
                        ser.write(score[i].encode())
                        time.sleep(.01)

                print("Curr num:", x)
                print("\n")
                if x == num:
                    break
                time.sleep(3)

                # Determine if the user wants to pause or quit
                status = ser.readline().decode().strip()
                print("Status:", status)
                while status == "wait":
                    status = ser.readline().decode().strip()

                if status == "quit":
                    break
                print("-------------------------------------------------------------------------")

            # Ignore children that are <div class="clearleft"></div> (i.e. not articles on Reddit)
            # Should not happen for any other reason. If so, the number of displayed results will be incorrect and
            # must be due to an error in a function
            except:
                print("Pass")
                pass

        # Write to the serial monitor that the end has been reached
        if status != "quit":
            ser.write(b'exit25')
        print("Finished scraping", sub)
