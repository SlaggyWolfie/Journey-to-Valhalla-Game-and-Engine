This is the CodeBlocks project folder for the MicroGameEngine.

To run the engine in CodeBlocks take the following steps:

1. Download and extract the MINGW_extended bundle (codeblocks_vx_20xx_20xx.zip) from Blackboard
2. Extract the bundle somewhere on your harddrive on a path without spaces.
3. Download and install codeblocks (https://sourceforge.net/projects/codeblocks/files/Binaries/16.01/Windows/codeblocks-16.01mingw-setup.exe/download)
	* Fire up codeblocks
	* Goto Settings -> Compiler
	* Copy the "GNU GCC Compiler" setting to "GNU GCC Compiler MINGW EXTENDED"
	* Make sure "GNU GCC Compiler MINGW EXTENDED" is selected in the dropdown box
	* In the tab "Toolchain executables" , change "Compiler installation directory" to your MinGW_extended folder.
	* Open the MGE project file, right click it, and select "Build Options"
	* Change the selected compiler for this project to your "GNU GCC Compiler MINGW EXTENDED"
	* Select one of the build targets (debug/release & dynamic/static) and build the engine.

Deploying your game?
--------------------
Build a release version, copy all dll's in case you chose the dynamic version and assets to the same folder, zip and deploy.

Setting up your own codeblocks project?
------------------------------------------------------
Copy the mge folder structure but replace the sources.
