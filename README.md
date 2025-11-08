# MMW Chat Demo
This is a chat application created for the purpose of demonstrating the Minimal Middleware library.

# Getting Started
Clone this repository and run
```bash
git submodule update --init --recursive
```
This will pull the wxWidgets submodule.

## Building for Linux
Ensure you have all the required dependencies installed
```bash
# Install required packages on Ubuntu
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    g++ \
    python3 \
    python3-dev \
    python3-pip \
    libwxgtk3.0-gtk3-dev \
    libsqlite3-dev
```
Note: libwxgtk3.0-gtk3-dev is for Ubuntu versions older than 24. For Ubuntu 24, use libwxgtk3.2-dev.

You can now build the project. From the root of the project directory, run the following
```bash
mkdir -p build/
cd build/
cmake ../ -DBUILD_BROKER=ON
make
```

## Building for Windows
If you're using Windows, I'm sorry, but you'll have to build wxWidgets from source since vcpkg seems to fail on some of its dependencies.

First, use vcpkg to install dependencies for the broker
```bash
vcpkg sqlite3
```

Then you can build the application
```bash
mkdir -p build/
cd build/
cmake ../ -DBUILD_BROKER=ON -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
make
```
Make sure you pass the path to your cmake toolchain file to cmake when you build.

## Running the demo
Running the build commands above should have generated the ```chatdemo``` application in the current directory, as well as the ```broker``` in ```_deps/mmw-build/```. Run the broker and then as many chatdemo instances as you'd like.
