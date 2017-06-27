from bs4 import BeautifulSoup
import requests

'''
Created with guidance from Python example
Find the first 25 entries on a Reddit page
'''

def scrapeReddit():
    print("Reddit Scraper\n")
    sub = input("Enter a subreddit (default is popular):")
    if len(sub) == 0:
        sub = "popular"

    r = requests.get("https://www.reddit.com/r/" + sub + "/")

    print("Status:", r.status_code)
    if r.status_code != 200:
        print("Reddit is unavailable. Try again later")
        return

    else:
        print("Reddit is available. Finding first 25 results\n")

    soup = BeautifulSoup(r.text, "html.parser")
    results = soup.find("div", {"class": "content", "role": "main"})
    if results:
        results2 = results.find("div", {"id": "siteTable", "class": "sitetable linklisting"})
        children = results2.children


    for item in children:
        try:
            title = item.find("p", {"class": "title"}).find("a").text
            url = item.find("a").attrs["href"]
            if url[0] == "/":
                url = "https://www.reddit.com" + url

            score = item.find("div", {"class": "score unvoted"}).text
            print("Title:", title)
            print("URL:", url)
            print("Score:", score)
            print("\n")

        # Ignore children that are <div class="clearleft"></div> - should work code around to filter this out later
        except:
            pass

scrapeReddit()
