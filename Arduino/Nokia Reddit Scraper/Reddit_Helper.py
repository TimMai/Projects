from bs4 import BeautifulSoup   # Parse through html source
import pyphen                   # Syllabify words for EZ Read
import requests                 # Send request to reddit to retrieve html source
import serial                   # Allow serial communication
import time                     # Allow delay

'''
EZ_Read makes words more easier to read by splitting words into a syllable
line_start - last position of the top line
line_end - first position of the below line
line(number) - line_start = 14*number-1, line_end = 14*number
    where number is the line number from 1 to 3
    line 4 is not included because there is no reason to hyphenate any words if there is no line 5
'''

def EZ_Read(title,line_start,line_end):
    try:
        # First line
        if (title[line_start].isalpha() and title[line_end].isalpha()):
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

    except:
        print("Exception\n")
        return title


numbers = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]

#pyphen
pyphen.language_fallback('en_US')
dic = pyphen.Pyphen(lang='en_US')


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
                title += " "

                # If the user wants EZ Read
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
                    if score[0] not in numbers:
                        score = "unknown"
                    score = "Score: " + score

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

                print("\n")
                if x == num:
                    break
                time.sleep(5)

            # Ignore children that are <div class="clearleft"></div> (i.e. not articles on Reddit)
            except:
               pass

        # Write to the serial monitor that the end has been reached
        ser.write(b'exit25')
