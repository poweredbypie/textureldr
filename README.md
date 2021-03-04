# textureldr
open source, integrated texture pack management system for geometry dash

# note
**auto inject only works with mega hack v6.2 beta as of now.**
**if your geometry dash does not open, please try installing the latest microsoft visual c++ 2019 redistributable [here](https://aka.ms/vs/16/release/vc_redist.x86.exe)**

# known issues
- none as of now, which is good 

# usage (without mega hack v6)
- download "textureldr.dll" and "libtiff.dll" in releases (to the right)
- drag both dlls into the folder where "GeometryDash.exe" is located. replace the existing libtiff.dll with the new one.
- launch the game.
- click the folder button that is now in place of the "more games" button.
- add packs by clicking the folder icon and adding packs folders to the directory it opens.
- select a texture pack and click the apply button!

# usage (mega hack v6.2 beta)
- download "textureldr.dll" in releases (to the right)
- drag the dll into the folder where "GeometryDash.exe" is located.
- find the file "absolutedlls", open it in notepad, and add a new line with the text "textureldr.dll". save the file.
- launch the game.
- click the folder button that is now in place of the "more games" button.
- add packs by clicking the folder icon and adding packs folders to the directory it opens.
- select a texture pack and click the apply button!

# usage (advanced)
- clone this repo.
- build the solution **in release mode**.
- open geometry dash.
- use a dll injector (such as [this one that i use](https://github.com/guided-hacking/GuidedHacking-Injector)) to inject the newly compiled dll into geometrydash.
- enjoy! you can play around with the functionality all you want.

# thanks
thank you to absolute, cos8o, andre, italian apk downloader, adaf, and many others from the GD programming server for helping me in my learning process. especially italian apk downloader, who released a repo of all the decompiled gd classes, which was super helpful while initially reversing texture reloading functionality.
