
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
	
	--Backgrounds
	backgroundNoSkull = Game.NewSprite(prefix.."Background without Skull.png", 0, 0, -1, -1)
	background = Game.NewSprite(prefix.."Menu - Options - Credits Background.png", 0, 0, -1, -1)
	pauseBG = Game.NewSprite(prefix.."Pause - Resolution Background.png", 0, 0, -1, -1)

	--Main Menu
	play=Game.NewButton(prefix.."Play.png",100,100)
	options=Game.NewButton(prefix.."Options.png",100,200)
	credits=Game.NewButton(prefix.."Credits.png",100,300)
	exit=Game.NewButton(prefix.."Exit.png",100,400)
	
	Game.SetButtonSpriteHover(play, prefix.."Glow/Play G.png")
	Game.SetButtonSpriteHover(options, prefix.."Glow/Options G.png")
	Game.SetButtonSpriteHover(credits, prefix.."Glow/Credits G.png")
	Game.SetButtonSpriteHover(exit, prefix.."Glow/Exit G.png")

	Game.SetEvent(play,"LevelMenu")	
	Game.SetEvent(options,"Options")
	Game.SetEvent(credits,"Credits")
	Game.SetEvent(exit,"Exit")
	
	Game.AddToMenu("MainMenu",play,options,exit,credits,background)
	
	--Options
	sfxOff = Game.NewButton(prefix.."SFX Off.png", 100, 100)
	sfxOn = Game.NewButton(prefix.."SFX On.png", 100, 100)
	soundsOff = Game.NewButton(prefix.."Sounds Off.png", 100, 200)
	soundsOn = Game.NewButton(prefix.."Sounds On.png", 100, 200)
	
	Game.SetButtonSpriteHover(sfxOff, prefix.."Glow/SFX Off G.png")
	Game.SetButtonSpriteHover(sfxOn, prefix.."Glow/SFX On G.png")
	Game.SetButtonSpriteHover(soundsOff, prefix.."Glow/Sounds Off G.png")
	Game.SetButtonSpriteHover(soundsOn, prefix.."Glow/Sounds On G.png")
	
	Game.AddToMenu("Options", sfxOff, sfxOn, soundsOff, soundsOn, background) 
	
	--Level Menu
	level1=Game.NewButton("Level 1.png",150,100)
	level2=Game.NewButton("Level 2.png",150,200)
	level3=Game.NewButton("Level 3.png",150,300)
	level4=Game.NewButton("Level 4.png",150,400)
	level5=Game.NewButton("Level 5.png",150,500)
	level6=Game.NewButton("Level 6.png",150,600)
	back=Game.NewButton("Back.png",	150,700)
	
	Game.SetEvent(level1,"OpenLevel1")
	Game.SetEvent(level2,"OpenLevel2")
	Game.SetEvent(level3,"Credits")
	Game.SetEvent(level4,"OpenLevel4")
	Game.SetEvent(level5,"OpenLevel5")
	Game.SetEvent(level6,"OpenLevel6")
	Game.SetEvent(back,"MainMenu")
	
	Game.AddToMenu("LevelMenu", level1, level2, level3, level4, level5, level6, back, background)

	--Pause Menu
	restart = Game.NewButton(prefix.."Restart.png", 100, 500)
	backToMenu=Game.NewButton("Main Menu.png",100,600)
	-- backToGame=Game.NewButton("BackToGame.png",500,800)
	backToGame=Game.NewButton(prefix.."Resume.png",100,700)
	
	Game.SetButtonSpriteHover(restart, prefix.."Glow/Restart G.png")
	Game.SetButtonSpriteHover(backToGame, prefix.."Glow/Resume G.png")

	Game.SetEvent(restart, "Restart")
	Game.SetEvent(backToMenu,"MainMenu")
	Game.SetEvent(backToGame,"BackToGame")
	
	Game.AddToMenu("PauseMenu",restart,backToGame,backToMenu, pauseBG)
	
	--Credits
	local creditsFont = "Assets/Fonts/celtic.ttf"
	creditDesigners = Game.NewText("Design and Art:", 100, 100, 32, "Black", creditsFont)
	creditIvan = Game.NewText("Ivan Ascic", 150, 150, 32, "Black", creditsFont)
	creditClaudia = Game.NewText("Claudia Sczygiol", 150, 200, 32, "Black", creditsFont)
	creditDanny = Game.NewText("Danny Seraphim", 150, 250, 32, "Black", creditsFont)
	creditErik = Game.NewText("Erik Smid", 150, 300, 32, "Black", creditsFont)
	creditRobin = Game.NewText("Robin van de Van", 150, 350, 32, "Black", creditsFont)
	creditProgrammers = Game.NewText("Programming:", 100, 400, 32, "Black", creditsFont)
	creditNikita = Game.NewText("Nikita Balabuiev", 150, 450, 32, "Black", creditsFont)
	creditSlavi = Game.NewText("Slavi Stoev", 150, 500, 32, "Black", creditsFont)
	
	creditLibraries = Game.NewText("Libaries and Frameworks Used:", 550, 100, 32, "Black", creditsFont)
	creditsOPENGL = Game.NewText("OpenGL", 600, 150, 32, "Black", creditsFont)
	creditsSFML = Game.NewText("SFML", 600, 200, 32, "Black", creditsFont)
	creditsGLM = Game.NewText("GLM", 600, 250, 32, "Black", creditsFont)
	creditsASSIMP = Game.NewText("Assimp", 600, 300, 32, "Black", creditsFont)
	creditsGLEW = Game.NewText("GLEW", 600, 350, 32, "Black", creditsFont)
	
	-- Game.AddToMenu("Credits", creditDesigners, creditProgrammers, creditIvan, creditNikita, creditClaudia, creditDanny, creditErik, creditSlavi, creditRobin, creditLibaries, back, backgroundNoSkull) 
	Game.AddToMenu("Credits", creditDesigners, creditIvan, creditClaudia, creditDanny, creditErik, creditRobin) 
	Game.AddToMenu("Credits", creditProgrammers, creditNikita, creditSlavi, back, backgroundNoSkull) 
	Game.AddToMenu("Credits", creditLibraries, creditsOPENGL, creditsSFML, creditsGLM, creditsASSIMP, creditsGLEW) 
	
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