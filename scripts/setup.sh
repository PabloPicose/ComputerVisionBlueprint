#!/bin/bash

# Source the os-release to get distribution info
source /etc/os-release

setupConan() {
    echo "Setting up Conan..."
    # if the command conan --version returns a non-zero status code then install conan via pip install conan==1.60
    if ! conan --version; then
        echo "Conan not found. Installing Conan..."
        pip install conan==1.60
    fi

    # check for the conan profile, if the conan profile "ComputerVisionRel" does not exist then create it
    if ! conan profile show ComputerVisionRel; then
        echo "Creating Conan profile..."
        conan profile new --detect --force ComputerVisionRel
        conan profile update settings.compiler.libcxx=libstdc++11 ComputerVisionRel
        echo "Updating conan profile package mode in sudo mode for install"
        conan profile update conf.tools.system.package_manager:mode=install ComputerVisionRel
        conan profile update conf.tools.system.package_manager:sudo=True ComputerVisionRel
        conan profile update settings.build_type=Release ComputerVisionRel
    fi
    # check for the conan profile, if the conan profile "ComputerVisionDeb" does not exist then create it
    if ! conan profile show ComputerVisionDeb; then
        echo "Creating Conan profile..."
        conan profile new --detect --force ComputerVisionDeb
        conan profile update settings.compiler.libcxx=libstdc++11 ComputerVisionDeb
        echo "Updating conan profile package mode in sudo mode for install"
        conan profile update conf.tools.system.package_manager:mode=install ComputerVisionDeb
        conan profile update conf.tools.system.package_manager:sudo=True ComputerVisionDeb
        conan profile update settings.build_type=Debug ComputerVisionDeb
    fi
}

# Function to setup dependencies for Ubuntu
setupUbuntuDeps() {
    echo "Setting up dependencies for Ubuntu..."
    sudo apt install -y libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render-util0 libxcb-sync1 libxcb-xfixes0 libxcb-shape0 libx11-xcb1 python3-venv ninja-build build-essential cmake libxkbcommon-x11-0 libxcb-icccm4 pkg-config git libegl1-mesa libegl1-mesa-dev libpulse-dev fuse
}
zc

# Main script starts here
# Check if the distribution is Ubuntu
if [ "$ID" = "ubuntu" ]; then
    setupUbuntuDeps
else
    echo "This script is designed to run on Ubuntu. Detected: $PRETTY_NAME"
    exit 1
fi

# Create a virtual environment on the current directory .venv
python3 -m venv .venv
source .venv/bin/activate

# Upgrade pip
pip install --upgrade pip
setupConan
# Install conan dependencies
conan install . -pr ComputerVisionRel -if .conan_rel -b missing
conan install . -pr ComputerVisionDeb -if .conan_debug -b missing

conan remove "*" -s -b -f

# create the 3rdparty directory
mkdir -p 3rdparty
# clone inside the 3rdparty directory https://github.com/paceholder/nodeeditor.git
cd 3rdparty
git clone https://github.com/paceholder/nodeeditor.git
cd ..