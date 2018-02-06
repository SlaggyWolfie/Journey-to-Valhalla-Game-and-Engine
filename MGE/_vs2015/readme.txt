This is the VS2015 project folder for the MicroGameEngine.

To run the engine in Visual Studio take the following steps:

1. Download and extract the vs_extended bundle (visualstudio20xx_cx_20xx_20xx.zip) from Blackboard.
2. Extract the bundle somewhere on your harddrive on a path without spaces.
3. Download and install VS2015:
---------------------------------
https://www.visualstudio.com/downloads/

* Open up mge_paths.props in your text editor:
	* Update the include and lib folder to where you extracted the vs_extended bundle
* Open up the project sln or vxproj in Visual Studio.
* Select one of the build targets (debug/release & dynamic/static) and build the engine.
* In case your version complains about missing dll's, copy all files from the vs_extended\bin folder to your exe folder.

Deploying your game?
--------------------
Build a release version, copy all dll's in case you chose the dynamic version and assets to the same folder, zip and deploy.

Setting up your own vs project?
------------------------------------------------------
Copy the mge folder structure but replace the sources.
