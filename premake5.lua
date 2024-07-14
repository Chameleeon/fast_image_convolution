workspace "Convolutor"
architecture "x64"
startproject "Main"

configurations {
  "Debug",
  "Release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ConvLib"
location "ConvLib"
kind "SharedLib"
language "C++"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("obj/" .. outputdir .. "/%{prj.name}")

files {
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp"
}

includedirs {
  "%{prj.name}/src"
}

cppdialect "C++20"
staticruntime "On"
systemversion "latest"

prebuildcommands {
  ("{MKDIR} ../bin/" .. outputdir .. "/Main")
}

postbuildcommands {
  ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Main")
}

project "Main"
location "Main"
kind "ConsoleApp"
language "C++"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("obj/" .. outputdir .. "/%{prj.name}")

files {
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp"
}

includedirs {
  "ConvLib/src",
  "%{prj.name}/src"
}

links {
  "ConvLib"
}

cppdialect "C++20"
staticruntime "On"
systemversion "latest"
