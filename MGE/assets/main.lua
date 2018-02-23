
print("if you see me I am lua")

function Initialize()
	print("lua init")
	Start()
end
function Start()
	print("this is start method")
	Game.ShowHint("This is a placeHolder For Hint",1,1,1)
end
function Update()
	print("Update Function Gets called")
end
