# Final Crusade

Final Crusade is a top down shooter that tasks you with capturing 6 file fragments in order to hack the system. Be sure to avoid the security system, they won't be happy that you're snooping around.

This project was built with SDL2 as a backend. The SDL code is encapsulated within my own custom wrapper that was built during my Game Architecture class in Spring 2018. The architecture of the project makes use of the Singleton, Object Pool, and Observer patterns among others. All game code was written, compiled, and debugged in Visual Studio 2017.

## Building Source Code
SDL2 is required to compile the project.

<strong>NOTE:</strong> This repo does not contain the required lib files. The lib files for the four SDL folders can be found using the links below. 

#### For the [SDL2-2.0.8](https://www.libsdl.org/download-2.0.php) folder
1) Download the SDL2-devel-2.0.8-VC.zip (Visual C++ 32/64-bit) under Development Libraries
2) Once the download is complete, move the lib folder into the SDL2-2.0.8 folder within the shared folder 

####  For the [SDL2_ttf-2.0.14](https://www.libsdl.org/projects/SDL_ttf/) folder
1) Download the  SDL2_ttf-devel-2.0.14-VC.zip (Visual C++ 32/64-bit) under Development Libraries
2) Once the download is complete, move the lib folder into the SDL2_ttf-2.0.14 folder within the shared folder 

####  For the [SDL2_mixer-devel-2.0.2-VC](https://www.libsdl.org/projects/SDL_ttf/) folder
1) Download the SDL2_mixer-devel-2.0.2-VC.zip (Visual C++ 32/64-bit) under Development Libraries
2) Once the download is complete, move the lib folder into the SDL2_mixer-2.0.2 folder located within shared/SDL2_mixer-devel-2.0.2-VC

####  For the [SDL2_image-2.0.3](https://www.libsdl.org/projects/SDL_image/) folder
1) Download the SDL2_image-devel-2.0.3-VC.zip (Visual C++ 32/64-bit) under Development Libraries
2) Once the download is complete, move the lib folder into the SDL2_mixer-2.0.2 folder located within shared/SDL2_mixer-devel-2.0.2-VC

Next, copy the individual .lib files from SDL2_ttf-2.0.14 and SDL2_mixer-devel-2.0.2-VC into SDL2-2.0.8/lib. The VS solution currently links to that folder only.

##

<strong>NOTE:</strong> SDL DLLs must be placed within the main folder. All DLLs can be found within the shared folder.
Required:

* libfreetype-6.dll
* libjpeg-9.dll
* libpng16-16.dll
* libtiff-5.dll
* libwebp-7.dll
* SDL2.dll
* SDL2_image.dll
* SDL2_mixer.dll
* SDL2_ttf.dll
* zlib.dll

