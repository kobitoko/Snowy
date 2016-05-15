# Snowy
A simple game engine written in C++ using SDL2 and Liquidfun.

SDL2: http://www.libsdl.org/

SDL2_image: https://www.libsdl.org/projects/SDL_image/

SDL2_mixer: http://www.libsdl.org/projects/SDL_mixer/

SDL2_ttf: https://www.libsdl.org/projects/SDL_ttf/

SDL2_gfx: http://cms.ferzkopp.net/index.php/software/13-sdl-gfx

SDL2_net: https://www.libsdl.org/projects/SDL_net/

Liquidfun: http://google.github.io/liquidfun/

Now I use [MSYS2](https://msys2.github.io/)  
- Install a toolchain, in the MSYS2 terminal for 32-bit: pacman -S mingw-w64-i686-gcc  
Main reason I use it now because ease of installing any libraries/tools one might need. One can search the repositories by doing:  
pacman -Ss package_name_of_something_i_want_to_install  

Using code::blocks now as IDE, if you really want a makefile, could use [cbp2make](https://sourceforge.net/projects/cbp2make/)  
For Msys2: cbp2make.exe -in project.cbp -unix and rename the "something-project.mak" to "Makefile"  
