from bs4 import BeautifulSoup
import requests

'''
Created with guidance from Python example
Find the first 25 (usually) entries on a Reddit page
Allows delving into a comment thread of a corresponding entry afterwards
'''

# Main function - scrapes any Reddit page that is a subreddit
def scrapeReddit():
    print("Reddit Scraper\n")
    sub = input("Enter a subreddit (default is popular):")
    if len(sub) == 0:
        sub = "popular"

    r = requests.get("https://www.reddit.com/r/" + sub + "/", headers = {'user-agent': 'shiperl_bot'})

    print("Status:", r.status_code)
    if r.status_code != 200:
        print("Reddit is unavailable. Try again later")
        return

    else:
        print("Reddit is available. Finding first 25 results\n")

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
                url = "https://www.reddit.com" + url

                url_list.append(url)
                # Keep track of URLs for later - should revise code to only look for valid children
                # to save space

            else:       # Entry directs to another site - need to make sure the added URL is the comment thread
                thread = item.find("li", {"class": "first"}).find("a").attrs["href"]
                url_list.append(thread)

            score = item.find("div", {"class": "score unvoted"}).text
            print("Title" + str(x) + ": " + title)
            print("URL:", url)
            print("Score:", score)
            print("\n")

        # Ignore children that are <div class="clearleft"></div> - should work code around to filter this out later
        except:
            pass

    print("Select a number corresponding to a title to delve into a Reddit comment thread. 0 to quit")
    choice = int(input())

    if choice == 0:
        return

    else:
        choice -= 1
        url = url_list[choice]
        print("You chose", url)

        print("Choose how comments are sorted by (select number):")
        types = {"1": "confidence", "2": "top", "3": "new", "4": "controversial", "5": "old"}
        print("1. best (default)")
        print("2. top")
        print("3. new")
        print("4. controversial")
        print("5. old")
        choice = input()

        if types[choice]:
            thread = url + "?sort=" + types[choice]
        else:
            thread = url + "?sort=confidence"

        comment_scrape(thread)


# After finding a URL, scrape the comment thread
def comment_scrape(url):
    r = requests.get(url, headers = {'user-agent': 'shiperl_bot'})

    print("Status:", r.status_code)
    if r.status_code != 200:
        print("Reddit is unavailable. Try again later")
        return

    soup = BeautifulSoup(r.text, "html.parser")
    list = soup.find("div", {"class": "commentarea"})
    comments = list.find("div", {"class": "sitetable nestedlisting"}).children
    x = 0
    for comment in comments:
        try:
            text = comment.find("div", {"class": "md"}).find("p").text
            score = comment.find("span",{"class": "score unvoted"}).text
            print("Comment:", text)
            print("Score:", score)
            print("\n")
            x += 1
        except:
            pass

scrapeReddit()
