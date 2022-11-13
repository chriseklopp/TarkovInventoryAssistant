# TarkovInventoryAssistant
Escape from Tarkov Inventory Assistant.




![Alt text](/../../../chriseklopp/blob/main/TIA_screenshot2.PNG?raw=true "Title")

*Releases:*
https://github.com/chriseklopp/TarkovInventoryAssistant/releases


Motivation:
------------------------------------------------------------------------------------------------------------------------
Escape from Tarkov doesn't have a method of itemizing your hideout Stash (item storage). The only useful information the
game gives you is your total stash value to the Traders. With the complex barter system in game, it can be a pain to
keep track of what items you have, need, or should sell to the Traders or on the flea market. This tool to itemize an
inventory into a readable list (and potentially other functions) will make inventory management a lot easier and less of
a headache.
------------------------------------------------------------------------------------------------------------------------

This project was Converted to C++ from Python.
All work on this project from now onward will be in this repo.

Outline:
------------------------------------------------------------------------------------------------------------------------
Use perceptual hashing and template matching to identify and catalog items from screen shots of a Tarkov stash.
Should be able to recognize all items.
Should be able to avoid  double counting rows if they are present in multiple screenshots.
Able to calculate available barters through traders, as well as highlight barters that are a net $ value gain.

VERSION 1:
Correctly Itemize all items in a single screenshot.

VERSION 2:
Correctly Itemize all items in MULTIPLE screenshots.

VERSION 3:
GUI interface. Select screenshots and visually show details about inventory analysis.

VERSION 4:
Calculate and show available barters and recommend optimal traders selection for selling.

VERSION 5:
(This would be beyond the expected scope of the project, due to the potential issues with getting this information)
Get and use Flea Market Data to determine items to post on market VS selling to vendors. Detect cases where an item
could be bought off the flea market and used in a barter that would cause a net gain in value.
