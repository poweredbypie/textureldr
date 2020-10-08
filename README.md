# textureldr
open source, integrated texture pack management system for geometry dash

# note
i'm pretty sure this is relatively stable, but i can't make any guarantees. if you want 0% chance of anything breaking, please do not use this mod, and if you don't trust me, please back up your savefiles (%localappdata%/GeometryDash/ + CCGameManager.dat / + CCLocalLevels.dat).

# known issues
- the textureldr button still appears after ejecting the dll. clicking the button will cause the game to crash, as the game is trying to access deallocated memory. (same goes for ejecting while inside of the loader menu)
- it's ugly.
- removing 10 or more packs from the packs folder and clicking the reload button will end up with the selector looking weird, although i dont think it breaks the app

# to do
see todo.txt.

# usage
- download the .dll in releases (to the right)
- inject it with some dll injector (mega hack provides one). do NOT click ok on the message box that pops up, or you will exit the program.
- find the "packs" folder in the directory where your geometry dash exe is. drag in a folder with the textures you'd like to use in-game.
- click the folder button that is now in place of the "more games" button.
- select a texture pack and click the apply button!


# thanks
thank you to absolute, italian apk downloader, and many others from the GD programming server for helping me in my learning process. especially italian apk downloader, who released a repo of all the decompiled gd classes, which was super helpful while attempting to reverse the required functionality.
