
print("if you see me I am lua")

function Initialize()
	print("--------------------Lua Initialization!-------------------");
end
function Start()
	print("--------------------Lua Starts!-------------------");
	play=Game.NewButton("Play.png",	100,100)
	options=Game.NewButton("Options.png",100,200)
	credits=Game.NewButton("Credits.png",100,300)
	exit=Game.NewButton("Exit.png",100,400)
	
	level1=Game.NewButton("Level 1.png",150,100)
	level2=Game.NewButton("Level 2.png",150,200)
	level3=Game.NewButton("Level 3.png",150,300)
	level4=Game.NewButton("Level 4.png",150,400)
	level5=Game.NewButton("Level 5.png",150,500)
	level6=Game.NewButton("Level 6.png",150,600)
	back=Game.NewButton("Back.png",	150,700)

	backToMenu=Game.NewButton("Main Menu.png",500,500)
	backToGame=Game.NewButton("BackToGame.png",500,800)

	
	


	Game.SetEvent(play,"LevelMenu")	
	Game.SetEvent(options,"Options")
	Game.SetEvent(credits,"Credits")
	Game.SetEvent(level1,"OpenLevel1")
	Game.SetEvent(level2,"OpenLevel2")
	Game.SetEvent(level3,"Credits")
	Game.SetEvent(level4,"OpenLevel4")
	Game.SetEvent(level5,"OpenLevel5")
	Game.SetEvent(level6,"OpenLevel6")
	Game.SetEvent(back,"MainMenu")
	Game.SetEvent(exit,"Exit")

	Game.SetEvent(backToMenu,"MainMenu")
	Game.SetEvent(backToGame,"BackToGame")
	

	Game.AddToMenu("LevelMenu",level1,level2,level3,level4,level5,level6,back)
	Game.AddToMenu("PauseMenu",backToGame,backToMenu)
	Game.AddToMenu("MainMenu",play,options,exit,credits)


	-- Game.PlaySoundOneShot("Background music.wav")


	-- Game.SetEvent(exit,"Exit")

	
	
	
end
function Update()	

	if Game.GetGameTime()>15 and Game.GetGameTime()<16  then
		Game.ShowHint("Use WASD to move")
	 end	
	 
	if Game.GetGameTime()>23 and Game.GetGameTime()<24  then
		Game.ShowHint("Try To Open The door")
	 end	
	 
	 

	if 	Game.GetGameTime()>35 and Game.GetGameTime()<36  then
		Game.ShowHint("Click on Crate To Possess it")
 	end	
	 

end