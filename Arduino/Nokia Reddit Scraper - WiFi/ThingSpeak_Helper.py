from bs4 import BeautifulSoup
import requests
import thingspeak
import pyphen                   # Syllabify words for EZ Read
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

'''
Retrieve params from the Reddit Params ThingSpeak channel
'''
def retrieve_params(startup):
    params_channel = thingspeak.Channel(id=params_channel_id, write_key=params_write_key, api_key=params_read_key)
    new_sub = params_channel.get_field_last(field=1)
    new_num = params_channel.get_field_last(field=2)
    new_id = params_channel.get_field_last(field=3)
    new_ez_read = params_channel.get_field_last(field=4)

    '''
    Isolate the actual desired text
    '''
    new_sub = new_sub.split(",")
    new_sub = new_sub[2].split(":")
    new_sub = new_sub[1]
    new_sub = new_sub[1:len(new_sub)-2]

    new_num = new_num.split(",")
    new_num = new_num[2].split(":")
    new_num = new_num[1]
    new_num = new_num[1:len(new_num)-2]

    new_id = new_id.split(",")
    new_id = new_id[2].split(":")
    new_id = new_id[1]
    new_id = new_id[1:len(new_id)-2]

    new_ez_read = new_ez_read.split(",")
    new_ez_read = new_ez_read[2].split(":")
    new_ez_read = new_ez_read[1]
    new_ez_read = new_ez_read[1:len(new_ez_read)-2]
    new_ez_read = bool(new_ez_read)

    # Check to see if these are the correct params - whether the retrieved ID matches the startup ID + 1
    if not startup and int(new_id) != sess_id:
        print("Wrong params. Need to wait")
        time.sleep(35)
        return []

    # These are the right params
    else:
        print("Sub:", new_sub)
        print("Num:", new_num)
        print("ID:", new_id)
        print("EZ Read:", new_ez_read)

        if startup:
            if new_id == "null":
                new_id = 0

            else:
                new_id = int(new_id) + 1
            print("Next ID needs to be:", new_id)
        parameters = [new_sub, new_num, new_id, new_ez_read]
        return parameters


'''
Send results to the Reddit Results ThingSpeak channel
'''
def send_results(results_string, sess_id):
    results_channel = thingspeak.Channel(id=results_channel_id, write_key=results_write_key, api_key=results_read_key)

    # Need to separate the results into multiple strings because ThingSpeak only allows strings of size 255
    # Have each result hold 3 entries
    if len(results_string) > 213:
        results1 = results_string[0:213]
    else:
        results1 = results_string[0:]
        results_channel.update({1: sess_id, 2: results1})
        return

    if len(results_string) > 426:
        results2 = results_string[213:426]
    else:
        results2 = results_string[213:]
        results_channel.update({1: sess_id, 2: results1, 3:results2})
        return

    if len(results_string) > 639:
        results3 = results_string[426:639]
    else:
        results3 = results_string[426:]
        results_channel.update({1: sess_id, 2: results1, 3: results2, 4: results3})
        return

    if len(results_string) > 852:
        results4 = results_string[639:852]
        results5 = results_string[852:]
        results_channel.update({1: sess_id, 2: results1, 3: results2, 4: results3, 5: results4, 6:results5})
        return
    else:
        results4 = results_string[639:]
        #results_channel.update({1:results1, 2:results2, 3:results3, 4:results4, 6:sess_id})
        results_channel.update({1:sess_id, 2:results1, 3:results2, 4:results3, 5:results4})
        return

'''
Params Channel (receiving)
'''
params_write_key = "ZIE5W42CTL5250M2"
params_read_key = "OUR7Z8B1RDN7KMLY"
params_channel_id = 316708

'''
Results channel (sending)
'''
results_write_key = "SNOB4G4PRCAH6BTZ"
results_read_key = "ZEPXT4FHP9OO2ONG"
results_channel_id = 316731

# pyphen initialization
pyphen.language_fallback('en_US')
dic = pyphen.Pyphen(lang='en_US')


sub = "popular"
num = 10
sess_id = -5
ez_read = True
params = retrieve_params(startup=True)
sess_id = params[2]
print("Reddit Scraper\n")

while(1):
    # Hold the results
    results_string = ""


    params = []
    while params == []:
        params = retrieve_params(startup=False)
    print()
    sub = params[0]
    num = params[1]
    sess_id = params[2]
    ez_read = params[3]


    print("Scraping...")

    r = requests.get("https://www.reddit.com/r/" + sub + "/", headers = {'User-agent': 'scraper_bot'})
    print("Status:", r.status_code)
    if r.status_code != 200:
        # Write to the serial monitor that there was an error finding the subreddit
        results_string = "exit26"
        send_results(results_string, sess_id)
        sess_id = sess_id + 1

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
                if(ez_read):
                    title = EZ_Read(title,13,14)
                    title = EZ_Read(title,27,28)
                    title = EZ_Read(title,41,42)

                # Add spaces so that the program knows when the end of a result is reached
                # 56 is the number of total characters possible for 4 lines. 14 characters each
                while len(title) < 56:
                    title += " "

                for i in range(0, 56):
                    if title[i] == "\"" or title[i] == "\'":
                        # When sending data to ThingSpeak, the data length is messed up due to these chars
                        results_string += " "
                    else:
                        results_string += title[i]

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
                        results_string += score[i]

                except:
                    print("Unable to obtain score")
                    score = "unknown"
                    while len(score) < 15:
                        score += " "
                    for i in range (0,15):
                        results_string += score[i]

                print("Curr num:", x)
                print("Num:", num)
                print("\n")
                if int(x) == int(num):
                    print("X = Num")
                    break

            # Ignore children that are <div class="clearleft"></div> (i.e. not articles on Reddit)
            # Should not happen for any other reason. If so, the number of displayed results will be incorrect and
            # must be due to an error in a function
            except:
                print("Pass")
                pass

        # results_string += "exit25"
        send_results(results_string, sess_id)

        # Next results will be this id. Arduino will auto increment from its side as well
        sess_id = int(sess_id) + 1
        print("Finished scraping", sub)

        # Remove this break to make the while loop continue
        # i.e. if the user wants to continue scraping on the Nokia without restarting the python script
        break


