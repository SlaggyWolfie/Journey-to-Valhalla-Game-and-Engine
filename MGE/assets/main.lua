
print("if you see me I am lua")

function Initialize()
	print("lua init")
	Start()
end
function Start()
	print("this is start method")
	
end
function Update()
	--obj1="Player"
	--obj2="obj2"
	--if(Game.CollisionBetween(obj1,obj2)) then
		--Game.PushBackObj(obj1,obj2)
		--print("i work")
	--end		
	if(Game.GetGameTime()>1) then
		Game.ShowHint("sth")
	end
	if(Game.CollisionBetween("Player","gate")) then
		Game.PushBackObj("Player","gate")
		print("lua scripting in work")
	end	
	if(Game.SphereCollisionBetween("Player","crate1")) then
		Game.PushBackObj("Player","crate1")
		print("lua scripting in work")
	end	
	if(Game.SphereCollisionBetween("Player","crate2")) then
		Game.PushBackObj("Player","crate2")
		print("lua scripting in work")
	end
if(Game.SphereCollisionBetween("Player","crate3")) then
		Game.PushBackObj("Player","crate3")
		print("lua scripting in work")
	end	
if(Game.SphereCollisionBetween("Player","crate4")) then
		Game.PushBackObj("Player","crate4")
		print("lua scripting in work")
	end		
	
end
