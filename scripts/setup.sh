#!/bin/bash

# Source the os-release to get distribution info
source /etc/os-release

setupConan() {
    echo "Setting up Conan..."
    # if the command conan --version returns a non-zero status code then install conan via pip install conan==2.11
    # Check if the version of conan is 2.11
    if ! conan --version | grep -q "2.11"; then
        echo "Conan version is not 2.11. Installing Conan..."
        pip install conan==2.11
    fi

    # check for the conan profile, if the conan profile "ComputerVisionRel" does not exist then create it
    conan profile detect -e
}

# Function to setup dependencies for Ubuntu
setupUbuntuDeps() {
    echo "Setting up dependencies for Ubuntu..."
    sudo apt install -y libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render-util0 libxcb-sync1 libxcb-xfixes0 libxcb-shape0 libx11-xcb1 python3-venv ninja-build build-essential cmake libxkbcommon-x11-0 libxcb-icccm4 pkg-config git libegl1-mesa libegl1-mesa-dev libpulse-dev fuse
}

# Main script starts here
# Check if the distribution is Ubuntu
if [ "$ID" = "ubuntu" ]; then
    setupUbuntuDeps
else
    echo "Dependencies autoinstall is only available in ubuntu. Detected: $PRETTY_NAME"
fi

# Create a virtual environment on the current directory .venv
python3 -m venv .venv
source .venv/bin/activate

# Upgrade pip
pip install --upgrade pip
setupConan
# Install conan dependencies
conan install . --output-folder=.conan/Debug --build=missing --settings=build_type=Debug -c tools.system.package_manager:mode=install
conan install . --output-folder=.conan/Release --build=missing --settings=build_type=Release -c tools.system.package_manager:mode=install

# check if the 3rdparty/nodeeditor directory exists
if [ -d "3rdparty/nodeeditor" ]; then
    echo "Nodeeditor already exists. Skipping..."
else 
    echo "Cloning Nodeeditor..."
    # clone inside the 3rdparty directory https://github.com/paceholder/nodeeditor.git
    git clone https://github.com/paceholder/nodeeditor.git 3rdparty/nodeeditor
fi
