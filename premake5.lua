workspace "TurboGE"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TurboGE/vendor/GLFW/include"
IncludeDir["Glad"] = "TurboGE/vendor/Glad/include"
IncludeDir["Imgui"] = "TurboGE/vendor/imgui"
IncludeDir["glm"] = "TurboGE/vendor/glm"
IncludeDir["stb"] = "TurboGE/vendor/stbImage"
IncludeDir["entt"] = "TurboGE/vendor/entt/include"

group "Dependencies"
	include "TurboGE/vendor/GLFW"
	include "TurboGE/vendor/Glad"
	include "TurboGE/vendor/imgui"
	
	
group ""

project "TurboGE"
	location "TurboGE"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tgepch.h"
	pchsource "TurboGE/src/tgepch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stbImage/**.cpp",
		"%{prj.name}/vendor/stbImage/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TGE_DLL_BUILD",
			"TGE_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TGE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TGE_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TurboGE/vendor/spdlog/include",
		"TurboGE/src",
		"TurboGE/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"TurboGE"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TGE_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "TGE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "TGE_DIST"
		optimize "on"
		
project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TurboGE/vendor/spdlog/include",
		"TurboGE/src",
		"TurboGE/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"TurboGE"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TGE_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "TGE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "TGE_DIST"
		optimize "on"