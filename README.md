# textureldr
open source, integrated texture pack management system for geometry dash

# note
not stable. please do not download this if you are not willing to take risks with geometry dash.

# known issues
- pressing escape on the creator layer, level layer, or garage layer will cause the textureldr button not to appear on the menu layer.
- the textureldr button still appears after ejecting the dll. clicking the button will cause the game to crash, as the game is trying to access deallocated memory. (same goes for ejecting while inside of the loader menu)
- clicking the back button on the garage layer (when entered from a level) still instantiates the callback to main menu.

# to do
see todo.txt.

# usage
- download the .dll in releases 
- inject it with some dll injector (mega hack provides one, or you can use another one if you'd like more options)
- find the "packs" folder in the directory where your geometry dash exe is. drag in a folder with the textures you'd like to use in-game.
- click the newly added button at the left side of the main menu (if this doesn't appear, enter another menu and exit out - make sure you use the physical button instead of your keyboard, since i did not hook those callbacks)
- select a texture pack and click apply!


# thanks
thank you to absolute, italian apk downloader, and many others from the GD programming server for helping me in my learning process. especially italian apk downloader, who released a repo of all the decompiled gd classes, which was super helpful while attempting to reverse the required functionality.
