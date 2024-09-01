include "../../Premake/common.lua"

project "TNP - Launcher"
	location(dirs.projectfiles)
	language "C++"
	cppdialect "C++17"
	kind "ConsoleApp"

	debugdir(dirs.bin)
	targetdir(dirs.bin)
	targetname("TNP - Launcher".."_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

	files {
		dirs.launcher.."**.h",
		dirs.launcher.."**.cpp",
		dirs.launcher.."**.hpp",
	}

	-- includedirs { 
	-- 	dirs.external,  
	-- 	dirs.engine, 
	-- 	dirs.server .. "/Source", 
	-- 	dirs.game.. "/Source",
	-- 	dirs.shared.. "/Source", 
	-- }

	links {"Engine"}

	libdirs { dirs.lib, dirs.dependencies, dirs.Shared }

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"

	filter "system:windows"
	staticruntime "off"
	symbols "On"
	systemversion "latest"
	warnings "Extra"

	flags {
		"FatalCompileWarnings",
		"MultiProcessorCompile"
	}

	defines {
		"WIN32",
		"_LIB", 
		"TGE_SYSTEM_WINDOWS" 
	}