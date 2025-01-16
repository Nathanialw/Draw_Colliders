<h1>Linux</h1>

Required Libraries:
    
    sudo apt install libsdl2-dev
    sudo apt install libsdl2-image-dev
    sudo apt install libsdl2-mixer-dev
    sudo apt install libsdl2-ttf-dev


Run instructions:

    git clone git@github.com:Nathanialw/Draw_Colliders.git
    cd Draw_Colliders
    chmod +x build.sh           #makes the build.sh file executable
    ./build.sh                  #will launch upon successful build 


<h1>Windows</h1>

<h3>WSL</h3>
Will NOT run properly using WSL on Windows 10
<h3>Native</h3>
Note: it mostly works, but it seems the dialog box library is not working properly.

    git clone git@github.com:Nathanialw/Draw_Colliders.git
    cd Draw_Colliders
    mkdir build
    cd build
    cmake ..                           #outputs a visual studio solution
    cmake --build . --config Release
    cd Realease 
    cp -r ../../assets .               #copies the assets folder to the bin directory
    cp -r ../../dlls/* .               #copies the dlls folder to the bin directory

<h1>Draw Colliders</h1>

A tool for use with the Box2D library that lets you graphically define the collision boundaries of sprites. Outputs to XML, JSON or SQLite3 formats.

![Draw_Coliders_Example](https://github.com/Nathanialw/Draw_Colliders/assets/52517284/6a69fcc0-d828-48dc-9e04-553d5809a0ff)
