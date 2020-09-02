workspace "LunaGL"
	architecture "x64"
	configurations {
		"Debug",
		"Release"
	}
	flags {
		"MultiProcessorCompile"
	}
	startproject "LunaGL"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["assimp"] = "C:/Program Files/Assimp/include"
IncludeDir["GLFW"] = "LunaGL/vendor/GLFW/include"
IncludeDir["Glad"] = "LunaGL/vendor/Glad/include"
IncludeDir["ImGui"] = "LunaGL/vendor/imgui"
IncludeDir["glm"] = "LunaGL/vendor/glm"
IncludeDir["stb_image"] = "LunaGL/vendor/stb_image"

LibraryDir = {}
LibraryDir["assimp"] = "C:/Program Files/Assimp/lib/x64"

group "Dependencies"
	include "LunaGL/vendor/GLFW"
	include "LunaGL/vendor/Glad"
	include "LunaGL/vendor/imgui"
group ""

project "LunaGL"
	location "LunaGL"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "LunaGL/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	libdirs {
		"%{LibraryDir.assimp}"
	}

	links {
		"assimp-vc140-mt.lib",
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"