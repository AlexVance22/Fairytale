workspace "Fairytale"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Fairytale"
	location "Fairytale"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"FTALE_PLATFORM_WINDOWS",
			"FTALE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "FTALE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "FTALE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "FTALE_DIST"
		optimize "On"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"Fairytale/src"
	}

	links
	{
		"Fairytale"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"FTALE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "FTALE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "FTALE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "FTALE_DIST"
		optimize "On"