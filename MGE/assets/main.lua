
print("if you see me I am lua")

function Initialize()
	print("lua init")
end
function Start()
	play=Game.NewButton("Play.png",	100,100)
	options=Game.NewButton("Options.png",100,200)
	credits=Game.NewButton("Credits.png",100,300)
	exit=Game.NewButton("Exit.png",100,400)
	
	level1=Game.NewButton("Play.png",	100,100)
	level2=Game.NewButton("Options.png",100,200)
	level3=Game.NewButton("Credits.png",100,300)
	level4=Game.NewButton("Exit.png",	100,400)
	level5=Game.NewButton("Play.png",	100,500)
	level6=Game.NewButton("Options.png",100,600)
	back=Game.NewButton("Exit.png",		100,700)



	Game.SetEvent(play,"LevelMenu")	
	Game.SetEvent(options,"Options")
	Game.SetEvent(credits,"Credits")
	Game.SetEvent(level1,"OpenLevel1")
	Game.SetEvent(level2,"Credits")
	Game.SetEvent(level3,"Credits")
	Game.SetEvent(level5,"OpenLevel4")
	Game.SetEvent(level6,"OpenLevel5")
	Game.SetEvent(back,"MainMenu")
	Game.SetEvent(exit,"Exit")


	Game.AddToMenu("LevelMenu",level1,level2,level3,level4,level5,level6,back)
	Game.AddToMenu("MainMenu",play,options,exit,credits)
	

	


	-- Game.SetEvent(exit,"Exit")

	
	
	
end
function Update()	
	print("lua work")
	--obj1="Player"
	--obj2="obj2"
	--if(Game.CollisionBetween(obj1,obj2)) then
		--Game.PushBackObj(obj1,obj2)
		--print("i work")
	--end		
	if(Game.GetGameTime()>1) then
		Game.ShowHint("sth")
 	end
-- 	if(Game.CollisionBetween("Player","gate")) then
-- 		Game.PushBackObj("Player","gate")
-- 		print("lua scripting in work")
-- 	end	
-- 	if(Game.SphereCollisionBetween("Player","crate1")) then
-- 		Game.PushBackObj("Player","crate1")
-- 		print("lua scripting in work")
-- 	end	
-- 	if(Game.SphereCollisionBetween("Player","crate2")) then
-- 		Game.PushBackObj("Player","crate2")
-- 		print("lua scripting in work")
-- 	end
-- if(Game.SphereCollisionBetween("Player","crate3")) then
-- 		Game.PushBackObj("Player","crate3")
-- 		print("lua scripting in work")
-- 	end	
-- if(Game.SphereCollisionBetween("Player","crate4")) then
-- 		Game.PushBackObj("Player","crate4")
-- 		print("lua scripting in work")
--	end		
	
end
