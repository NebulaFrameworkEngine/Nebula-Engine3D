project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Nebula/src",
		"%{wks.location}/Nebula/vendor",
		"%{IncludeDirs.glm}",
	}

	links
	{
		"Nebula"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"NB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NB_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NB_DIST"
		runtime "Release"
		optimize "on"
