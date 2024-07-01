include "../../Premake/common.lua"

project "TNP - Shared"
	location(dirs.projectfiles)
	language "C++"
	cppdialect "C++20"
	kind "ConsoleApp"

	files {
		dirs.shared.."**.h",
		dirs.shared.."**.cpp",
		dirs.shared.."**.hpp",
	}

	includedirs { 
		dirs.external,  
		dirs.engine, 
		dirs.server .. "/Source", 
		dirs.game.. "/Source",
		dirs.shared.. "/Source", 
	}

	libdirs { dirs.lib, dirs.dependencies }

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