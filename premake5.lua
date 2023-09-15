include "Deps.lua"

workspace "TurboGE"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"





group "Dependencies"
	include "TurboGE/vendor/GLFW"
	include "TurboGE/vendor/Glad"
	include "TurboGE/vendor/imgui"
	include "TurboGE/vendor/yaml-cpp"
	include "TurboGE/vendor/box2d"
group ""
	
	
project "TurboGE"
	location "TurboGE"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
		
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.yamlcpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.Python}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"yaml-cpp",
		"opengl32.lib",
		"Box2D"
	}
	
	filter "files:TurboGE/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }
		
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TGE_DLL_BUILD",
			"TGE_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE"
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		runtime "Debug"
		symbols "on"
		
		links
		{
			"%{Lib.ShaderC_Debug}",
			"%{Lib.SPIRV_Cross_Debug}",
			"%{Lib.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "TGE_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Lib.ShaderC_Release}",
			"%{Lib.SPIRV_Cross_Release}",
			"%{Lib.SPIRV_Cross_GLSL_Release}"
		}
		
	filter "configurations:Dist"
		defines "TGE_DIST"
		runtime "Release"
		optimize "on"
		
		links
		{
			"%{Lib.ShaderC_Release}",
			"%{Lib.SPIRV_Cross_Release}",
			"%{Lib.SPIRV_Cross_GLSL_Release}"
		}

project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"TurboGE"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TGE_WINDOWS",
			"YAML_CPP_STATIC_DEFINE"
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		symbols "on"
		links
		{
			"%{Lib.Python_Debug}"
		}
		postbuildcommands {
			'{COPY} %{Dll.Python_Debug} ../bin/' .. outputdir .. '/%{prj.name}'
		}

	filter "configurations:Release"
		defines "TGE_RELEASE"
		optimize "on"
		links
		{
			"%{Lib.Python_Release}"
		}
		postbuildcommands {
			'{COPY} %{Dll.Python_Release} ../bin/' .. outputdir .. '/%{prj.name}'
		}
	
		
	filter "configurations:Dist"
		defines "TGE_DIST"
		optimize "on"
		links
		{
			"%{Lib.Python_Release}"
		}
		postbuildcommands {
			'{COPY} %{Dll.Python_Release} ../bin/' .. outputdir .. '/%{prj.name}'
		}