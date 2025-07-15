import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin, urlparse
import time

visited = set()
exclusions = set()

def crawl(url, keyword="bayesian", depth=0, max_depth=2):
    
    print("Curr : ", end="")
    for _ in range(depth):
        print("  ", end="")
    print(url)
    
    if url in visited or depth > max_depth:
        return
    visited.add(url)
    
    try:
        response = requests.get(url, timeout=5)
        content_type = response.headers.get("Content-Type", "")
        if "text/html" not in content_type:
            return

        html = response.text
        if keyword.lower() in html.lower():
            print(f"[FOUND] {keyword} in {url}")
        
        soup = BeautifulSoup(html, 'html.parser')
        for link_tag in soup.find_all('a', href=True):
            next_url = urljoin(url, link_tag['href'])
            if urlparse(next_url).netloc == urlparse(url).netloc:
                crawl(next_url, keyword, depth + 1, max_depth)
                
        time.sleep(0.5)
    
    except Exception as e:
        print(f"[ERROR] {url}: {e}")

# 시작점 예시
# start_url = "https://directory.seas.upenn.edu/computer-and-information-science/"
start_url = "https://directory.seas.upenn.edu/jacob-gardner/"
# exclusions.add()
crawl(start_url)
