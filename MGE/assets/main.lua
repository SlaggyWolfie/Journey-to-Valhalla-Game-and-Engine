
print("if you see me I am lua")

function Initialize()
	print("lua init")
	Start()
end
function Start()
	print("this is start method")
	
end
function Update()
		obj1="Player"
		obj2="obj2"
	if(Game.CollisionBetween(obj1,obj2)) then
		Game.PushBackObj(obj1,obj2)
		print("i work")
		end	
end
