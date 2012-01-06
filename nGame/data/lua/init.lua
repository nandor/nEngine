--[[
	Init script

	(c) 2011 Licker Nandor.
	All rights reserved.
]]--


-- Print the version of the lua API
Console.log(VERSION_STRING)

-- Load the core resource group
Resources.loadGroup('core', 'fs://data/core.json')

-- Unload temporary files
Resources.unloadGroup('tmp')