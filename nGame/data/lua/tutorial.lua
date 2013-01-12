--[[
	Tutorial map for nEngine

	(c) 2011 Licker Nandor.
	All rights reserved.
]]--

Tutorial = {}

Tutorial.turn = 6

function Tutorial.initField(map, pos)
	Tutorial.position = pos
	for i = pos.x, pos.x + 4, 2 do
		for j = pos.y, pos.y + 4, 2 do
			local tile = map:getTile(i, j)
			tile.id = 5
			tile.blocked = true
			tile.action = Tutorial.flipTile
		end
	end
end

function Tutorial.flipTile (tile)
	if tile.id == 5 then
		tile.id = Tutorial.turn
		
		Tutorial.turn = 13 - Tutorial.turn
		
		local win = Tutorial.checkWin()
		
		if win ~= 0 then
			print (win.." wins!")
		end
	end
end

function Tutorial.checkWin()

end

function Tutorial.onSceneInit (map)			
	nGame.initCharacter()
	
	local john = NPC.new('john1', 'john')		
	john:setSpawn(Vec2.new(2, 3))
	Scene.add(john)
		
	Scene.get('character').tile = Vec2.new(9, 9)
	
	local cam = Camera.new('cam')
	cam:follow('character')
	Scene.add(cam)
	Scene.setCamera('cam')
	
	local fire = Particles.new('fire', 'fire')
	fire.tile = Vec2.new(10, 10)
	Scene.add(fire)
	
	local tile = map:getTile(10, 10)
	tile.id = 8
	tile.blocked = true
	
	local lever = map:getTile(1, 5)
	lever.id = 2
	lever.blocked = true
	
	local state = 1
	
	lever.action = function ()
		Tutorial.initField(map, Vec2.new(3, 3))	
		lever.id = 2 + state
		state = 1 - state
	end
	
	Tutorial.initField(map, Vec2.new(3, 3))	
	
	Scene.start()
end
