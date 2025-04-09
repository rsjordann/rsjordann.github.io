''' 
Student Name:  Raelyn Jordan
Program Name:  Job Scraper
Creation Date:  12/9/2024
Last Modified Date:  12/10/2024
CSCI Course:  CSCI 301 Survey of Scripting Languages
Grade Received:  100
Design Comments: This project can be easily extended by allowing the user to choose the keyword for job filtering.
'''

#!/usr/bin/env python3

import requests
from bs4 import BeautifulSoup

# URL of the Python job board
URL = "https://www.python.org/jobs/"
page = requests.get(URL)

# Parse the page content
soup = BeautifulSoup(page.content, "html.parser")

# Find the jobs section
job_listings = soup.find("ol", class_="list-recent-jobs list-row-container menu")

# Find all job listing items
job_cards = job_listings.find_all("li")

# Define keywords to search in job titles
keywords = ["engineer"]

# Extract and print job titles, companies, locations, and posted date with keyword filtering
for job_card in job_cards:
    title_element = job_card.find("a")  # Job title in <a> tag
    company_element = job_card.find("span", class_="listing-company-name")  # Company in <span class="listing-company-name">
    location_element = job_card.find("span", class_="listing-location")  # Location in <span class="listing-location">
    posted_element = job_card.find("span", class_="listing-posted")  # Posted date in <span class="listing-posted">
    
    # Extract the job title, company, location, and posted date
    title = title_element.get_text(strip=True) if title_element else "No title"
    
    # Extract company name (strip extra text)
    company = company_element.get_text(strip=True) if company_element else "No company listed"
    
    location = location_element.get_text(strip=True) if location_element else "No location listed"
    posted = posted_element.get_text(strip=True) if posted_element else "No posted date"

    # Check if the job title contains any of the keywords
    if any(keyword.lower() in title.lower() for keyword in keywords):
        # Print only the filtered jobs that match the keyword criteria
        print(f"Job Title: {title}")
        print(f"Company: {company}")
        print(f"Location: {location}")
        print(f"Posted: {posted}")
        print()
