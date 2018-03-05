
print("if you see me I am lua")

function Initialize()
	print("lua init")
	Start()
end
function Start()
	print("this is start method")
	
end
function Update()
	-- print("Update Function Gets called")
	if(Game.CollisionBetween("Player","obj2")) then
	Game.PushBackObj("Player")
	print("i work")
	end	
end
