
print("if you see me I am lua")

function Initialize()
	print("--------------------Lua Initialization!-------------------");
end
function Start()
	print("--------------------Lua Starts!-------------------");
	
	-- play=Game.NewButton("Play.png",100,100)
	-- options=Game.NewButton("Options.png",100,200)
	-- credits=Game.NewButton("Credits.png",100,300)
	-- exit=Game.NewButton("Exit.png",100,400)
	local prefix = 'Assets/UI/'
	
	background = Game.NewSprite(prefix.."Menu - Options - Credits Background.png", 0, 0, -1, -1)
	pauseBG = Game.NewSprite(prefix.."Pause - Resolution Background.png", 0, 0, -1, -1)

	play=Game.NewButton(prefix.."Play.png",100,100)
	options=Game.NewButton(prefix.."Options.png",100,200)
	credits=Game.NewButton(prefix.."Credits.png",100,300)
	exit=Game.NewButton(prefix.."Exit.png",100,400)
	
	Game.SetButtonSpriteHover(play, prefix.."Glow/Play G.png")
	Game.SetButtonSpriteHover(options, prefix.."Glow/Options G.png")
	Game.SetButtonSpriteHover(credits, prefix.."Glow/Credits G.png")
	Game.SetButtonSpriteHover(exit, prefix.."Glow/Exit G.png")
	
	sfxOff = Game.NewButton(prefix.."SFX Off.png", 100, 100)
	sfxOn = Game.NewButton(prefix.."SFX On.png", 100, 100)
	soundsOff = Game.NewButton(prefix.."Sounds Off.png", 100, 200)
	soundsOn = Game.NewButton(prefix.."Sounds On.png", 100, 200)
	
	Game.SetButtonSpriteHover(sfxOff, prefix.."Glow/SFX Off G.png")
	Game.SetButtonSpriteHover(sfxOn, prefix.."Glow/SFX On G.png")
	Game.SetButtonSpriteHover(soundsOff, prefix.."Glow/Sounds Off G.png")
	Game.SetButtonSpriteHover(soundsOn, prefix.."Glow/Sounds On G.png")
	
	level1=Game.NewButton("Level 1.png",150,100)
	level2=Game.NewButton("Level 2.png",150,200)
	level3=Game.NewButton("Level 3.png",150,300)
	level4=Game.NewButton("Level 4.png",150,400)
	level5=Game.NewButton("Level 5.png",150,500)
	level6=Game.NewButton("Level 6.png",150,600)
	back=Game.NewButton("Back.png",	150,700)

	restart = Game.NewButton(prefix.."Restart.png", 500, 300)
	backToMenu=Game.NewButton("Main Menu.png",500,500)
	-- backToGame=Game.NewButton("BackToGame.png",500,800)
	backToGame=Game.NewButton(prefix.."Resume.png",500,800)
	
	Game.SetButtonSpriteHover(restart, prefix.."Glow/Restart G.png")
	Game.SetButtonSpriteHover(backToGame, prefix.."Glow/Resume G.png")
	
	-- background = Game.NewSprite(prefix.."Menu - Options - Credits Background.png", 0, 0, 1000, 1000)


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

	Game.SetEvent(restart, "Restart")
	Game.SetEvent(backToMenu,"MainMenu")
	Game.SetEvent(backToGame,"BackToGame")
	

	Game.AddToMenu("LevelMenu",level1,level2,level3,level4,level5,level6,back, background)
	Game.AddToMenu("PauseMenu",restart,backToGame,backToMenu, pauseBG)
	-- Game.AddToMenu("MainMenu",play,options,exit,credits)
	Game.AddToMenu("Options", sfxOff, sfxOn, soundsOff, soundsOn,background) 
	Game.AddToMenu("MainMenu",play,options,exit,credits,background)


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