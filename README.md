# textureldr
open source, integrated texture pack management system for geometry dash

# note
**auto inject does not work with mega hack v6 as of now. i will work on catching and displaying errors at some point in the future.**
i'm pretty sure this is relatively stable, but i can't make any guarantees. if you want 0% chance of anything breaking, please do not use this mod, and if you don't trust me, please back up your savefiles (%localappdata%/GeometryDash/ + CCGameManager.dat / + CCLocalLevels.dat).

# known issues
- the textureldr button still appears after ejecting the dll. clicking the button will cause the game to crash, as the game is trying to access deallocated memory. (same goes for ejecting while inside of the loader menu)
- it's ugly.
- removing 10 or more packs from the packs folder and clicking the reload button will end up with the selector looking weird, although i dont think it breaks the app

# to do
see todo.txt.

# usage
- download "textureldr.dll" and "libtiff.dll" in releases (to the right)
- drag both dlls into the folder where "GeometryDash.exe" is located. replace the existing libtiff.dll with the new one.
- find the "packs" folder (or create one if you haven't launched the game yet) in the directory where your geometry dash exe is. drag in folders with the textures you'd like to use in-game.
- launch the game, if you haven't already.
- click the folder button that is now in place of the "more games" button.
- select a texture pack and click the apply button!

# usage (advanced)
- clone this repo.
- make sure the solution / vs project is in x86 (geometry dash is 32bit).
- build the solution.
- open geometry dash.
- use a dll injector (such as [this one that i use](https://github.com/guided-hacking/GuidedHacking-Injector)) to inject the newly compiled dll into geometrydash.
- enjoy! you can play around with the functionality all you want.


# thanks
thank you to absolute, italian apk downloader, and many others from the GD programming server for helping me in my learning process. especially italian apk downloader, who released a repo of all the decompiled gd classes, which was super helpful while attempting to reverse the required functionality.
