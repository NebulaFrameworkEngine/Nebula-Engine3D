include "Dependencies.lua"

workspace "Nebula"
    architecture "x64"
    startproject "App"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

	flags
	{
		"MultiProcessorCompile"
	}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    group "Core"
    include "Nebula"
    group ""
    
    group "Dependencies"
    include "Nebula/vendor/GLFW"
	include "Nebula/vendor/Glad"
	include "Nebula/vendor/imgui"
    group ""

    group "Misc"
	include "App"
    group ""


