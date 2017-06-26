from bs4 import BeautifulSoup
import requests

search = input("Enter search term:")
params = {"q": search}
r = requests.get("http://www.bing.com/search", params = params)

soup = BeautifulSoup(r.text, "html.parser")
# print(soup.prettify())                     # Returns html source


# search through the soup - looking for element "ol" - ordered list, looking for attribute id == b_results
results = soup.find("ol", {"id": "b_results"})

# looking through results for lists which has attribute class == b_algo
links = results.findAll("li", {"class": "b_algo"})

for item in links:
    item_text = item.find("a").text             # find text of element -- print search result
    item_href = item.find("a").attrs["href"]    # find specific attribute -- print url of search reuslt

    if item_text and item_href:
        print(item_text)
        print(item_href)

print("\n\nReddit front page")
r = requests.get("http://www.reddit.com")
soup = BeautifulSoup(r.text, "html.parser")
results = soup.find("div", {"class": "content", "role": "main"})

results2 = results.find("div", {"id": "siteTable", "class": "sitetable linklisting"})
results3 = results2.findAll("p", {"class": "title"})

#print(results2)

for item in results3:
    item_text = item.find("a").text
    item_href = item.find("a").attrs["href"]

    if item_text and item_href:
        print(item_text)
        print(item_href)
        print("")
