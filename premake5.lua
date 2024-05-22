workspace "CLog"
	architecture "x64"
	startproject "Main"

	configurations {
		"Debug",
		"Release"
	}
	
	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CLog"
	location "CLog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CLog_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CLog_RELEASE"
		runtime "Release"
		optimize "speed"

project "Main"
	location "Main"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs {
		"CLog/src"
	}

	links {
		"CLog"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CLOG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CLOG_RELEASE"
		runtime "Release"
		optimize "on"