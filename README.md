# TarkovInventoryAssistant
Escape from Tarkov Inventory Assistant.




![Alt text](/../../../chriseklopp/blob/main/TIA_screenshot11.PNG?raw=true "Title")

*Releases:*
https://github.com/chriseklopp/TarkovInventoryAssistant/releases

*NOTE: Still WIP, only currently works on open containers*

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
- Uses perceptual hashing and template matching to identify and list items from screen shots of a Tarkov stash.

- User friendly WxWidgets interface for efficient operation and ease of viewing.

- Displays price information about items in the image as well as total value of items detected.

- Catalog of all items in the game with search functionality
