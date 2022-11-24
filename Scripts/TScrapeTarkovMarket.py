# Tarkov Market has decided to make their website a PITA to scrape.
# Pull out the big guns and run Selenium to load the page and scrape it anyway.
# This will be very slow but this script doesnt need to be run often at all so I dont care too much.

import bs4
import pandas as pd
import requests
import os
import shutil
import time
from PIL import Image
from selenium import webdriver
from selenium.webdriver.common.by import By


class MarketScraper:

    def __init__(self,
                 chromeDriverExe=r"C:\MyWorkspace\WebScraping\chromedriver.exe"
                 ):
        self.url = "https://tarkov-market.com"
        self.m_driver = webdriver.Chrome(chromeDriverExe)


    def run(self):
        self.m_driver.get(self.url)
        # Click btn to enable infinite scrolling
        self.clickLoadMoreBtn()
        self.scrolltobottom()
        ret = self.extractInformation()
        self.m_driver.quit()
        return ret

    def scrolltobottom(self):
        # As we scroll and spam the site with requests, they may start to throttle us.
        # The sleep time will dynamically increase as this happens.
        waitTime = .3
        last_height = self.m_driver.execute_script("return document.body.scrollHeight")

        timeSinceRep = 0
        while True:
            reps = 0
            while True:
                # Scroll down to the bottom.
                self.m_driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
                time.sleep(waitTime)
                new_height = self.m_driver.execute_script("return document.body.scrollHeight")
                if new_height == last_height:
                    break
                last_height = new_height
                reps += 1

            if reps > 0:
                timeSinceRep = 0
            else:
                timeSinceRep += waitTime
                if timeSinceRep > 8:  # break if no new info has loaded in 8 seconds.
                    break

            waitTime += .1  # slightly increase waittime.

    def clickLoadMoreBtn(self):
        # Click load more button to enable the infinite scroll.
        loadMoreBtnXpath = r'//button[@class = "big bold w-100 text-center py-10"]'
        loadMoreBtn = self.m_driver.find_element_by_xpath(loadMoreBtnXpath)
        loadMoreBtn.click()

    def extractInformation(self):

        # <div class="table-list"
        #   <div class="row"
        #       <div class="cell" (Image; dont care)
        #       <div class="cell" (Name) *******
        #           -Recursive find for <span class="name"
        #       <div class="cell" (Wiki link; Dont care)
        #       <div class="cell alt" (Avg price, avg PPS) ******
        #           -Recursive find for <span class="price-main"
        #           -Recursive find for <span class="price-sec"
        #       <div class="cell plus" (24hr diff; Dont care)
        #       <div class="cell plus" (7 day diff; Dont care)
        #       <div class="cell" (Sell to trader; name and price) ******
        #       <div class="cell" (Pro status AD; Dont care)

        tlSource = self.m_driver.find_element(By.CLASS_NAME, "table-list").get_attribute("outerHTML")
        #html = self.m_driver.page_source
        soup = bs4.BeautifulSoup(tlSource, "html.parser")

        tableList = soup.find("div", class_='table-list')

        col_headers = ["Name", "Price", "PricePerSlot", "SellToTrader", "TraderName"]
        row_contents = []
        rows = tableList.find_all('div', ['row'], recursive=False)
        for row in rows:
            if len(row.attrs['class']) != 1:  # we have picked up unwanted class tags
                continue

            outContent = []
            cellDivs = row.find_all('div', ['cell'], recursive=False)
            self.extractName(cellDivs[1], outContent)
            self.extractPriceInfo(row.find("div", class_="cell alt", recursive=False), outContent)
            self.extractSellToTraderInfo(cellDivs[6], outContent)
            row_contents.append(outContent)


        table_df = pd.DataFrame(row_contents, columns=col_headers)
        print("Price table Created")
        return table_df

    @staticmethod
    def extractName(cellTag, outList):
        outList.append(cellTag.find("span", class_="name").text.replace(" ", "_").replace('"', "").replace(",", ""))

    @staticmethod
    def extractPriceInfo(cellTag, outList):

        price = cellTag.find("span", class_="price-main").text.replace('"', "").replace(",", "'")
        pps = ""
        pricePerSlot = cellTag.find("span", class_="price-sec")
        if pricePerSlot:
            pps = pricePerSlot.text.replace("\n", "").replace(" ", "").replace('"', "").replace(",", "'")

        outList.append(price)
        outList.append(pps)

    @staticmethod
    def extractSellToTraderInfo(cellTag, outList):
        cells = cellTag.findChild("div").findChildren("div")
        outList.append(cells[0].text.replace('"', "").replace(",", "'"))
        if len(cells) == 1:
            return

        outList.append(cells[1].text.replace('"', "").replace(",", "'"))
        return


if __name__ == "__main__":
    ms = MarketScraper()
    res = ms.run()
    print()
