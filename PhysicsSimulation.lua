
no_project = true
dofile "./premake4.lua"

-- PhysicsSimulation
gfx_physicsSimulation_dir = path.getabsolute(".")

physicsSimulation_files = {	gfx_physicsSimulation_dir .. "/PhysicsSimulation/src/script/*.cpp", 
							gfx_physicsSimulation_dir .. "/PhysicsSimulation/src/script/*.h"
	}
	
project("physicsSimulation")
    language "C++"
    kind "ConsoleApp"
    targetdir ( gfx_physicsSimulation_dir .. "/bin" )
	includedirs { gfx_physicsSimulation_dir .. "/PhysicsSimulation/src/script/" }
    files ( gkit_files )
    files ( physicsSimulation_files )
