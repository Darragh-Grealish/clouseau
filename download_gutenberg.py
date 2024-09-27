import requests
from bs4 import BeautifulSoup
import os
from concurrent.futures import ThreadPoolExecutor


def process_book_list(soup: BeautifulSoup, limit: int) -> list:
    book_list = []
    for item in soup.select('ol li'):
        title_tag = item.find('a')
        if title_tag and title_tag['href']:
            title = title_tag.text.strip()

            if item.find('span', class_='subtitle'):
                author = item.find('span', class_='subtitle').text.strip()
            else:
                author = "Unknown Author"

            link = title_tag['href']
            book_list.append((title, author, link))

    return book_list[:limit]


def get_book_leaderboard() -> list:
    response = requests.get("https://www.gutenberg.org/browse/scores/top")
    if response.status_code != 200:
        print(f"Failed to retrieve data: {response.status_code}")
        return []

    soup = BeautifulSoup(response.content, 'html.parser')
    return process_book_list(soup, 100)


def download_book(book_title, book_link):
    response = requests.get(f"https://www.gutenberg.org{book_link}.txt.utf-8")
    if response.status_code == 200:
        if not os.path.exists("archive"):
            os.makedirs("archive")

        filename = os.path.join("archive", f"{book_title}.txt")
        with open(filename, 'wb') as f:
            f.write(response.content)
            print(f"Downloaded: {book_title}")
    else:
        print(f"Failed to download {book_title}: {response.status_code}")


def main():
    book_list = get_book_leaderboard()
    with ThreadPoolExecutor(max_workers=10) as executor:
        futures = [executor.submit(download_book, title, link)
                   for title, author, link in book_list]

        for future in futures:
            future.result()  # Wait for all threads to complete


if __name__ == "__main__":
    main()
