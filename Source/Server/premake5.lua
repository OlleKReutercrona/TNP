include "../../Premake/common.lua"

project "TNP - Server"
	location(dirs.projectfiles)
	language "C++"
	cppdialect "C++17"
	kind "ConsoleApp"

	debugdir(dirs.bin.."Server/")
	targetdir(dirs.bin.."Server/")
	targetname("TNP - Server".."_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

	files {
		dirs.server.."**.h",
		dirs.server.."**.cpp",
		dirs.server.."**.hpp",
	}

	includedirs { dirs.external,  dirs.engine, dirs.server }

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