# Compile on Linux
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

# Compile on Window with Visual Studio
#### Clone source code

    git clone https://github.com/xiangxw/mupdf-qt.git
    cd mupdf-qt
    git submodule update --init --recursive

#### Compile MuPDF Source
Go to platforms/win32, open mupdf.sln and build.

    
#### Compile mupdf-qt
Launch CMake(gui version), set source code directory and build directory. Configure and generate mupdf-qt.sln. Open mupdf-qt.sln and build it.
