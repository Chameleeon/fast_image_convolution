workspace "Convolutor"
architecture "x64"
startproject "main"

configurations {
  "Debug",
  "Release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "conv_lib"
location "conv_lib"
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
  ("{MKDIR} ../bin/" .. outputdir .. "/main")
}

postbuildcommands {
  ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/main")
}

filter "system:linux or system:macosx"
        buildoptions { "-fopenmp" }
        linkoptions { "-fopenmp" }

project "main"
location "main"
kind "ConsoleApp"
language "C++"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("obj/" .. outputdir .. "/%{prj.name}")

files {
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp"
}

includedirs {
  "conv_lib/src",
  "%{prj.name}/src"
}

links {
  "conv_lib"
}

cppdialect "C++20"
staticruntime "On"
systemversion "latest"
