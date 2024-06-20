include "../../Premake/common.lua"

project "TNP - UnitTest"
	location(dirs.projectfiles)
	language "C++"
	cppdialect "C++17"
	kind "SharedLib"

	files {
		dirs.shared.."**.h",
		dirs.shared.."**.cpp",
		dirs.shared.."**.hpp",
	}

	includedirs { 
		"$(VCInstallDir)Auxiliary/VS/UnitTest/include",
		dirs.engine,
		dirs.game,
	}


	libdirs { 
		dirs.lib, 
		dirs.dependencies,
		"$(VCInstallDir)Auxiliary/VS/UnitTest/lib" 
	}


	debugdir "%{dirs.bin}"
	targetdir ("%{dirs.bin}")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

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