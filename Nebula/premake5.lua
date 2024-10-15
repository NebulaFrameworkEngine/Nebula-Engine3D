project "Nebula"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. outputdir)
    
	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}

    includedirs
	{
		"src/Nebula",
		"vendor",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.Glad}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.stb_image}",
	}

    links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"NB_PLATFORM_WINDOWS",
			"NB_BUILD_DLL"
		}

		links
		{
		}

	filter "configurations:Debug"
		defines "NB_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
		}

	filter "configurations:Release"
		defines "NB_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
		}

	filter "configurations:Dist"
		defines "NB_DIST"
		runtime "Release"
		optimize "on"

		links
		{
		}
