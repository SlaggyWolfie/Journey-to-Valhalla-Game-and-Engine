Folder contents:
----------------

src			- all sources for the Micro Game Engine
assets		- all assets for the Micro Game Engine
_vs2015		- the project files for opening the MGE in Visual Studio (2015 or 2017)
_codeblocks	- the project files for opening the MGE in CodeBlocks

Choose your platform (codeblocks/visualstudio) and check the readme for that platform:
_vs2015\readme.txt
_codeblocks\readme.txt

Tips for setting up your own game in the MGE:
---------------------------------------------
Create a folder under src/ eg src/mygame
Create a folder under assets/ eg assets/mygame

Copy src/mge/config.hpp, src/mge/MGEDemo.* to src/mygame
Update src/mygame/config.hpp to use the new assets path
Update src/mygame/MGEDemo.* to src/mygame/MyGame.* 

Keep your own files separate from the mge folder.

An even better but more involved approach is not to use just one extra folder mygame,
but also an additional folder called mygameengine.
Decide for every file that you create whether it is a general extension of the core engine 
or a file specific to your game and put it in mygame or mygameengine accordingly.
