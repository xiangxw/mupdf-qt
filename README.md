# How to compile mupdf-qt

## Compile on Linux
#### Install tools and thirdparty libraries
You should install some tools and thirdparty libraries. For Ubuntu users, you can use the following command(similar for other Linux users):

    sudo apt-get install build-essential pkg-config cmake qtbase5-dev qt5-default libfreetype6-dev

#### Clone source code

    git clone https://github.com/xiangxw/mupdf-qt.git
    cd mupdf-qt
    git submodule update --init --recursive

#### Compile MuPDF Source

    cd mupdf
    make
    
#### Compile mupdf-qt

    cd ..
    mkdir build
    cd build
    cmake ..
    make

## Compile on Window with Visual Studio
#### Clone source code

    git clone https://github.com/xiangxw/mupdf-qt.git
    cd mupdf-qt
    git submodule update --init --recursive

#### Compile MuPDF Source
Go to platforms/win32, open mupdf.sln and build.

    
#### Compile mupdf-qt
Launch CMake(gui version), set source code directory and build directory. Configure and generate mupdf-qt.sln. Open mupdf-qt.sln and build it.

## Compile on Window with MSYS MinGW
#### Clone source code

    git clone https://github.com/xiangxw/mupdf-qt.git
    cd mupdf-qt
    git submodule update --init --recursive

#### Installing MSYS/MinGW 
Install MSYS and MinGW (http://www.mingw.org/wiki/Getting_Started). Using the Graphical User Interface Installer, [mingw-get-setup.exe](http://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download), is recommended. During installation, "checking/ticking" the following in "Basic Setup" is recommended:

1. mingw32-base
2. mingw32-gcc-g++
3. msys-base
4. mingw-developer-toolkit

Add ```C:\MinGW\bin;``` to your PATH system variables (at the beginning).

#### Compile MuPDF source

    cd mupdf-qt
    make build=debug NOX11=yes

For release, just change "debug" to "release". NOX11 is necessary since X11 headers are not available in Windows. Note that this will not build the "app" packaged with the MuPDF source.

#### Compile mupdf-qt

Install CMake.

Open CMakeLists.txt with Qt Creator(Qt5 built with mingw), run cmake and build mupdf-qt
