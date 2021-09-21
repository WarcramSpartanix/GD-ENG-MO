# GD-ENG MO : Scene Editor
 

# Members:
	- Warren Luceñara
	- Nathan Roleda
	- Jaso Santiago
	- Aljon Tungol

# Source Code : https://github.com/WarcramSpartanix/GD-ENG-MO
# Important:
	- The default branch to be downloaded is "SceneEditor" branch. The "main" branch is deprecated.
	- The main function is located in main.cpp
# Instructions:
	- Open the DirectXGame.vcxproj or DirectXGame.sln
	- Set the C++ Standard language to ISO C++ 17 Standard
	- Make sure the following project properties have the respective values
		- VC++ Directories -> Include Directories : "Libs\tinyobjloader\include;Libs\DirectXTex\include;"
		- VC++ Directories -> Library Directories : "Libs\ReactPhysics3D\;Libs\DirectXTex\bin;"
		- C/C++ -> General -> Additional Include Directories : "Libs\ReactPhysics3D\include;"
		- C/C++ -> Preprocessor -> Preprocessor Definitions : "_DEBUG;_CONSOLE;NOMINMAX;_CRT_SECURE_NO_WARNINGS;"
		- Linker -> Input -> Additional Dependencies : "d3d11.lib;d3dcompiler.lib;DirectXTexD.lib;reactphysics3d.lib;"
	- Compile the code through Local Windows Debugger