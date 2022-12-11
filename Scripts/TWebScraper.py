"""
This probably wont need to run at all during normal use of the program, but will have to be run periodically to update
the local catalog of items.


Will scrape https://escapefromtarkov.fandom.com/wiki/Loot for item information.
Eventually will probably scrape flea market data from another website.

CTRl + SHIFT + C makes your life easier.
"""


import bs4
import pandas as pd
import requests
import os
import shutil
import time
from PIL import Image
from TScrapeTarkovMarket import MarketScraper

class TWebScraper:
    def __init__(self, prices):
        # self.supplemental_item_information_gatherer("https://escapefromtarkov.fandom.com/wiki/Folder_with_intelligence")
        tarkov_wiki_urls = ["https://escapefromtarkov.fandom.com/wiki/Armor_vests",
                            "https://escapefromtarkov.fandom.com/wiki/Containers",
                            "https://escapefromtarkov.fandom.com/wiki/Weapon_mods",
                            "https://escapefromtarkov.fandom.com/wiki/Chest_rigs",
                            "https://escapefromtarkov.fandom.com/wiki/Provisions",
                            "https://escapefromtarkov.fandom.com/wiki/Loot",
                            "https://escapefromtarkov.fandom.com/wiki/Weapons",
                            "https://escapefromtarkov.fandom.com/wiki/Keys_%26_Intel",
                            "https://escapefromtarkov.fandom.com/wiki/Backpacks",
                            "https://escapefromtarkov.fandom.com/wiki/Headwear",
                            "https://escapefromtarkov.fandom.com/wiki/Eyewear",
                            "https://escapefromtarkov.fandom.com/wiki/Gear_components",
                            "https://escapefromtarkov.fandom.com/wiki/Armbands",
                            "https://escapefromtarkov.fandom.com/wiki/Face_cover",
                            "https://escapefromtarkov.fandom.com/wiki/Medical"
                            #"https://escapefromtarkov.fandom.com/wiki/Ammunition" TODO: need spec handling for this
                            ]

        self.priceTable = prices
        self.directory_builder(tarkov_wiki_urls)
        start = time.time()  # debug
        for url in tarkov_wiki_urls:
            self.catalog_builder(url)
        end = time.time()  # debug
        print("All Downloads Complete")
        print(f"Total Time:{end-start}")  # debug

    def catalog_builder(self, url: str):
        """
         https://escapefromtarkov.fandom.com/wiki/Loot
        """
        ogwd = os.getcwd()
        source = requests.get(url).text
        soup = bs4.BeautifulSoup(source, 'lxml')
        page_tables = soup.find_all("table", class_="wikitable")# sortable stickyheader jquery-tablesorter') # TODO: look for both
        for table_num, table in enumerate(page_tables):
            table_df = self.table_builder(table)
            if table_df.empty:
                continue
            os.chdir(ogwd)  # return working directory to original.
            wd = os.getcwd()
            page_name = url.split("/")[-1]
            page_directory = os.path.join(wd, "Data", "catalog", page_name)
            image_directory = os.path.join(page_directory, "images")

            os.chdir(image_directory)
            print("Downloading Images")
            self.image_downloader(table_df)

            os.chdir(page_directory)

            # Now make new df to save with only our desired columns.
            desired = ["Name", "ImagePath", "Dims", "ContainerDims"]
            error = False
            for i in desired:
                if i not in table_df:
                    error = True
                    break
                if table_df[i][0] == "":
                    error = True
            if error:
                continue
            table_df = table_df[desired]

            # Merge table with price table
            table_df = pd.merge(table_df, self.priceTable, "left", "Name")
            print()




            table_df.to_csv(f"{page_name}{table_num}.csv", index=False)
        os.chdir(ogwd)  # return working directory to original.


    def table_builder(self, table: bs4.element.Tag) -> pd.DataFrame:
        table_body = table.find("tbody")
        table_rows = table_body.findAll('tr')
        # Read table row by row, within each row read column wise.

        col_names = self.create_headers(table_rows)

        master_list = []
        for row in table_rows:
            row_contents = []
            columns = row.findAll(recursive=False)  # type: bs4.element.ResultSet
            for element in columns:  # each element is type bs4.element.Tag
                row_contents.append(self.element_parser(element))
            row_tuple = tuple(row_contents)
            master_list.append(row_tuple)



        # col_names = master_list.pop(0)  # first entry is the table header.
        # extracted_names = []
        # for item in col_names:
        #     extracted_names.append(item['text'][0])

        table_df = pd.DataFrame(master_list, columns=col_names)
        table_df = self.table_cleaner(table_df)

        if "Dims" not in table_df.columns:
            if "Item_url" in table_df.columns:
                table_df["Dims"] = table_df['Item_url'].apply(self.supplemental_item_information_gatherer)
            else:
                print("No Item_url column found")

        return table_df

    def create_headers(self, row_list: list) -> list:
        # Reads the list of rows and generates the headers
        # Cuts off rows identified as headers from row_list and returns a list of proper headers.
        # TODO : Make code clean and not copy pasted.

        header_list = []
        first_row = row_list.pop(0)
        is_multirow_header = False

        # Determine type of header (Single or Multi Row)
        first_row_columns = first_row.findAll('th')
        for column in first_row_columns:
            col_span = column.get("colspan")
            row_span = column.get("rowspan")

            if col_span:
                is_multirow_header = True  # Indicates that we need to read second row to get all true column headers
                break

        # Extract info from first header row.

        for column in first_row_columns:
            col_span = column.get("colspan")
            row_span = column.get("rowspan")

            if not is_multirow_header:
                extracted_name = self.element_parser(column)['text'][0] # Single row header extract directly into list.
                header_list.append(extracted_name)
            else:
                if row_span:  # For multirow header, only multi row span are wanted.
                    extracted_name = self.element_parser(column)['text'][0]
                    header_list.append(extracted_name)

                    # If its multirow header, get info from second row.
        if is_multirow_header:

            second_row = row_list.pop(0)
            second_row_columns = second_row.findAll('th')
            for column in second_row_columns:
                extracted_name = self.element_parser(column)['text'][0]
                header_list.append(extracted_name)

        # CHECK FOR HEADER SORT ROW.
        if len(row_list) != 0:
            third_row = row_list[0]  # dont pop it yet, just in case its not a sort row.
            third_row_columns = third_row.findAll('th')
            for column in third_row_columns:
                element_results = self.element_parser(column)
                if not element_results:
                    row_list.pop(0)
                    break

        return header_list

    @staticmethod
    def directory_builder(url_list: list):
        file_names = []
        for url in url_list:
            file_names.append(url.split("/")[-1])

        # create new catalog directory. Named catalog_YYYY_MM_DD.
        wd = os.getcwd()

        if not os.path.exists("Data"):  # create folder if it doesnt exist.
            path = os.path.join(wd, 'Data')
            os.mkdir(path)

        data_dir = os.path.join(wd, 'Data')

        catalog_path = os.path.join(data_dir, "catalog")

        if os.path.exists(catalog_path):
            old_catalog_path = os.path.join(data_dir, "catalog_old")

            if os.path.exists(old_catalog_path):
                shutil.rmtree(old_catalog_path)  # only want to keep 1 previous directory, delete the old one.
                # os.remove(old_catalog_path)

            os.rename(catalog_path, os.path.join(data_dir, 'catalog_old'))

        os.mkdir(catalog_path)

        for name in file_names:
            path = os.path.join(catalog_path, name, 'images')
            os.makedirs(path)

    def image_downloader(self, df: pd.DataFrame):

        df_columns = df.columns

        if "Image_url" not in df_columns:
            return
        if "Name" not in df_columns:
            return

        df['Image_url'] = df.apply(self.download_image, axis=1)
        df.rename({'Image_url': 'ImagePath'}, axis=1, inplace=True)

    @staticmethod
    def download_image(df_row):  # downloads image and replaces url with path.
        cwd = os.getcwd()
        url = df_row["Image_url"]
        if not url:
            print("Image_url is empty")
            return

        try:
            if ".gif" not in url:
                source = requests.get(url, stream=True)
                if source.status_code == 200:
                    with open(f"{df_row['Name']}.bmp", 'wb') as f:
                        source.raw.decode_content = True
                        shutil.copyfileobj(source.raw, f)
                    return f"{df_row['Name']}.bmp"
                else:
                    print("uh oh")

            else:  # occasionally the images are gifs that need to be dealt with properly.
                source = requests.get(url, stream=True)
                if source.status_code == 200:
                    with open(f"{df_row['Name']}.gif", 'wb') as f:
                        source.raw.decode_content = True
                        shutil.copyfileobj(source.raw, f)

                    gif_path = os.path.join(os.getcwd(), f"{df_row['Name']}.gif")
                    im = Image.open(gif_path)
                    # transparency = im.info['transparency']

                    im.save(f"{df_row['Name']}.bmp")
                    return f"{df_row['Name']}.bmp"
                else:
                    print("uh oh")
        except(requests.ConnectionError):
            print("why do you hate me?")



    def price_catalog_builder(self):
        pass

    @staticmethod
    def supplemental_item_information_gatherer(url: str) -> str:
        # This name is terrible.
        # Some information needs to be grabbed from an items specific page, (specifically we want its dimensions)
        # This will act on an Item_url and return the necessary information as a dict
        # Currently only returns raw string of dims, because its all I want rn.
        print(f"Dimensions being gathered for:{url}")
        information_dict = {}
        source = requests.get(url).text
        soup = bs4.BeautifulSoup(source, 'lxml')
        if not soup:
            return ""
        item_table = soup.find('table', class_='va-infobox')
        if not item_table:
            return ""
        table_rows = item_table.findAll('tr')
        for element in table_rows:
            infobox_labels = element.find('td', class_='va-infobox-label')  # type: bs4.element.Tag
            if infobox_labels:
                if "Grid" in str(infobox_labels.string):
                    information_dict['dims'] = element.find('td', class_='va-infobox-content').string.replace("x", "-")
                    break
                else:
                    continue

        return information_dict.get("dims")

    def table_cleaner(self, df: pd.DataFrame) -> pd.DataFrame:
        # TODO: Potentially implement a better solution to handling 'Other' column types
        # While different tables on the wiki may have different information, and ordering, there is some specific
        # information types we are looking from each entry that is shared among each table.
        # Specifically we want: name, image url, item url, and any other identifiable information on the item.
        # 1) classify columns
        # 2) Extract useful information from them
        # 3) return new cleaner df.

        cleaned_df = pd.DataFrame()
        column_names = df.columns
        classifications = self.column_classifier(df)
        for col_number, col_type in enumerate(classifications):
            specified_column = df.iloc[:, col_number]

            if col_type == "Name":
                cleaned_df["Name"] = specified_column.apply(self.information_extractor, tag="title")
                cleaned_df["Item_url"] = specified_column.apply(self.information_extractor, tag="href")
                cleaned_df["Item_url"] = cleaned_df["Item_url"].apply(self.url_fixer)

            elif col_type == "Image":
                cleaned_df["Image_url"] = specified_column.apply(self.information_extractor, tag="data-src",
                                                                 alt_tag="src")

            elif col_type == "Identifier":
                cleaned_df[column_names[col_number]] = specified_column.apply(self.information_extractor, tag="text")

            elif col_type == "Other":
                cleaned_df[column_names[col_number]] = specified_column.apply(self.information_extractor, tag="text")

            else:
                raise NameError('Incorrect column classifier')
        if "Name" not in cleaned_df.columns:
            return pd.DataFrame()
        cleaned_df['Name'] = cleaned_df['Name'].apply(self.name_cleaner)

        # apply both name_cleaner and rename_dimensional_column functions
        cleaned_column_names = [self.rename_dimensional_column(self.name_cleaner(name)) for name in cleaned_df.columns]

        cleaned_df.columns = cleaned_column_names

        # properly format dimensions.
        if "Dims" in cleaned_df:
            cleaned_df["Dims"] = cleaned_df["Dims"].apply(self.dim_cleaner)
        if "ContainerDims" in cleaned_df:
            cleaned_df["ContainerDims"] = cleaned_df["ContainerDims"].apply(self.dim_cleaner)
        else:
            # non container item.
            cleaned_df["ContainerDims"] = "0-0"

        return cleaned_df

    @staticmethod
    def url_fixer(url: str) -> str:
        # Item paths only have a relative path on the wiki, so we re-add the rest of the url.
        if "https" not in url:
            return "https://escapefromtarkov.fandom.com" + url
        else:
            return url

    @staticmethod
    def column_classifier(df: pd.DataFrame) -> list:
        # identify each column as, Image, Name, Identifier, or OTHER (other is any non-useful field, ie. Notes)
        # only want first image found as its probably the correct one.
        classifications = []
        for column_name in df.columns:
            try:  # sometimes a wiki table is screwed up and has no rows.
                col_sample = df[column_name][0]
            except IndexError:
                print("Table has no rows.")
                classifications.append("Other")  # pray the all encompassing "other" will save this
                continue

            if "Name" in column_name:
                name_value = col_sample.get('title')

                if not name_value:
                    print("A classification error may have occurred")
                    classifications.append("Other")

                else:
                    classifications.append("Name")
                continue

            detected_data_src = col_sample.get("data-src")  # sometimes only one of these is present.  Its infuriating.
            detected_src = col_sample.get("src")
            if detected_data_src or detected_src:
                if "Image" not in classifications:  # only allow one Image column.
                    classifications.append("Image")
                else:
                    classifications.append("Other")
                continue

            detected_text = col_sample.get("text")
            if detected_text:
                if len(detected_text) < 3:
                    classifications.append("Identifier")
                else:
                    classifications.append("Other")
            else:
                classifications.append("Other")
            continue

        return classifications

    def information_extractor(self, info_dict: dict, tag: str, alt_tag=None) -> str:
        # extract information from a dictionary key from each row in a df column.
        # when alt tag provided, if tag returns None, will try the alt_tag instead.
        if alt_tag:
            value = self.information_extractor(info_dict, tag)
            if not value:
                value = self.information_extractor(info_dict, alt_tag)
            return value

        if info_dict == None:
            info_dict = {}

        value = info_dict.get(tag)

        if not value:  # value is None if this key DNE
            print(f"{tag} tag not found")  # this will not happen if tag isn't found on first entry
            return None

        if type(value) == list:
            return value[0]

        return value

    @staticmethod
    def element_parser(element: bs4.element.Tag) -> dict:
        print("-------------------------")
        # Parse attributes and text from an element's descendants into a dictionary

        element_contents = {}
        for descendant in element.descendants:
            if descendant.name == 'a' or descendant.name == 'img':  # if it is a dictionary type container.
                for key, value in descendant.attrs.items():
                    element_contents[key] = value

            else:  # for not dictionary types, likely just text or empty garbage.

                text = descendant.string
                if not text:  # check for NoneType
                    continue
                text = text.strip("\n")

                if not text:
                    continue
                text = text.strip()

                if text:  # one last time ensure text still exists.
                    if "text" not in element_contents:
                        element_contents["text"] = []
                    element_contents["text"].append(text)

        # element_contents["text"] = set(element_contents["text"])

        print(element_contents)
        return element_contents

    @staticmethod
    def name_cleaner(name: str) -> str:  # applied to dataframe to remove spaces and special characters from item names.
        name = name.replace(" ", "_")
        name = name.replace('"', '')
        name = name.replace("'", "")
        name = name.replace("/", "")
        name = name.replace("*", "")
        return name

    @staticmethod
    def dim_cleaner(dim: str) -> str:  # applied to df to properly format dimensions.
        return dim.replace("x", "-")

    @staticmethod
    def rename_dimensional_column(name: str) -> str:  # rename dimensional columns to a common name.
        name_lower = name.lower()
        if "inner" in name_lower:
            return "ContainerDims"
        if "outer" in name_lower:
            return "Dims"
        return name


if __name__ == "__main__":  # run this to update the catalog from the wiki.
    ms = MarketScraper()
    priceTable = ms.run() # get price data
    #priceTable.to_csv("priceTabletest.csv")
    TWebScraper(priceTable)
