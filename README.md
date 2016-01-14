## AlphaEngine / Hangars

AlphaEngine and AlphaIDE is a game environment but to be client-server, database support, multi-player gaming platform original developed from the idea of a Hangar build that was not cross platform compatible. AlphaEngine and AlphaIDE is separated into three parts. A low-level gaming engine(Urho3D), a gaming API(AlphaEngine), and game code. The purpose of AlphaEngine is develop games that can be multi-platform and gurantee gameplay including Android, and OS/X environments in the future.

Hangars is a multi-player on-line game allowing a player to build and expand a starbase in the story lore of Existence. Existence is a game about seven races among multiple civilization in a expanded universe. Each race would have multiple personality traits, physical characteristic, and career specialty.

Hangars is a 2D and 3D game using elements of Existence and Urho3D game engine. 

## Creative
The current theme for Hangars is Starbase creation aka Hangars.

## Developers

1. Vivienne Anthony – Lead Developer / Programmer
2. Andrej Skorinko - Developer / Programmer 

## Tools
1. Blender – Open source 3d modeling and animation software 
2. Makehuman – Open source human character modeling software 
3. Urho3D – Open source 3d graphics and rendering engine (http://urho3d.github.io/)
4. GDB - Open source debugger 

## Important Links
1. http://garagedoorproductions.averyanthony.com/
2. https://github.com/GarageDoorProductions/Urho3D-Hangars

## Requirements
1. Current build requires ODBC for MySQL (HangarsServer). Generally built.

## Notes
1. Server uses Posix PThreads.

## Contact
Email: cgprojectsfx@gmail.com

## License/Copyright
Vivienne Anthony @ 2015 

URHO 3D is MIT License - https://github.com/urho3d/Urho3D
ImGUI - https://github.com/ocornut/imgui

## Building/Installation
1. Download zip file from GitHub
2. Download resource file from developer site (GameData.pak, CoreData.pak, Data.pak)
3. Decompress Github folder to a folder.
4. Go to specific folder in step 1 

>./cmake_codeblocks.sh builddirectory -DURHO3D_64BIT=1 -DURHO3D_SAMPLES=0 -DURHO3D_EXTRAS=1 -DURHO3D_DATABASE_ODBC=1 -DNANODBC_USE_BOOST_CONVERT=ON -DBuild-AlphaEngine=1 -DBuild-HangarsGame=1 -DBuild-AlphaEditor=1 -DCMAKE_BUILD_TYPE=RelWithDebInfo

5. Place #2 files in builddirectory/bin folder

## Credits
All the Developers of Urho3D
Bluefish
Scorvi

