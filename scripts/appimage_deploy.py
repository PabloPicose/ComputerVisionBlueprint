import os
import sys
import subprocess
import argparse
import shutil
import tarfile


class Color:
    RED = "\033[91m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    BLUE = "\033[94m"
    END = "\033[0m"


def pInfo(text):
    print(Color.BLUE + "[INFO] " + Color.END + text)


def pError(text):
    print(Color.RED + "[ERR ] " + Color.END + text)


def pWarning(text):
    print(Color.YELLOW + "[WARN] " + Color.END + text)


def pOk(text):
    print(Color.GREEN + "[OK  ] " + Color.END + text)


# the function will create the AppDir structure in the destiny path
def createAppDirStructure(destiny: str):
    pInfo("Creating the AppDir structure in: " + destiny)
    # create the AppDir structure
    os.makedirs(destiny + "/usr/bin", exist_ok=True)
    os.makedirs(destiny + "/usr/lib", exist_ok=True)
    os.makedirs(destiny + "/usr/share/applications", exist_ok=True)
    # os.makedirs(destiny + "/usr/share/icons", exist_ok=True)
    # os.makedirs(destiny + "/usr/share/metainfo", exist_ok=True)
    # os.makedirs(destiny + "/usr/share/mime/packages", exist_ok=True)
    # os.makedirs(destiny + "/usr/share/mime", exist_ok=True)
    # os.makedirs(destiny + "/usr/share/doc", exist_ok=True)


def create_apprun_script(appdir_path, executable_name):
    apprun_content = f"""#!/bin/bash

HERE="$(dirname "$(readlink -f "${{0}}")")"
export LD_LIBRARY_PATH="${{HERE}}/usr/lib:${{LD_LIBRARY_PATH}}"
export QT_PLUGIN_PATH="${{HERE}}/usr/plugins:${{LD_LIBRARY_PATH}}"

# If your application needs other environment variables, set them here

exec "${{HERE}}/usr/bin/{executable_name}" "$@"
"""
    apprun_path = os.path.join(appdir_path, "AppRun")
    with open(apprun_path, "w") as apprun_file:
        apprun_file.write(apprun_content)

    # Make the AppRun script executable
    os.chmod(apprun_path, 0o755)


class AppImageDeploy:
    def __init__(self, binpath: str, qtpath: str) -> None:
        self.binpath = binpath
        self.qtpath = qtpath
        self.libs = set()
        self.bin = ""
        self.additionalPaths = {}
        pass

    def getLibs(self) -> set:
        return self.libs

    def getBin(self) -> str:
        return self.bin

    def getQtPath(self) -> str:
        return self.qtpath

    def getLddOutput(self, executable_path: str):
        # launch the ldd process for the executable, the output will be a list of libraries
        ldd_process = subprocess.Popen(["ldd", "-r", executable_path], stdout=subprocess.PIPE)
        ldd_output = ldd_process.stdout.read().decode("utf-8")
        # split the output by new line
        ldd_output = ldd_output.split("\n")
        # split the output by space
        ldd_output = [line.split(" ") for line in ldd_output]
        # we only need the column 2
        ldd_output = [line[2] for line in ldd_output if len(line) > 2]
        # remove the empty lines
        ldd_output = [line for line in ldd_output if line != ""]
        return ldd_output

    def process(self):
        executable_path = self.binpath + self.getExecutable()
        if not os.path.isfile(executable_path):
            pError("The executable file does not exists: " + executable_path)
            sys.exit(1)
        else:
            pOk("The executable file exists: " + executable_path)
        self.bin = executable_path

    def getExecutable(self) -> str:
        pass

    # return the required libraries, and for the required libraries if there is the need of run ldd again for each one
    def getAdditionalLibraries(self) -> dict:
        return {}

    def getAdditionalPaths(self) -> dict:
        return self.additionalPaths


class ComputerVisionBlueprint(AppImageDeploy):
    def __init__(self, binpath: str, qtpath: str):
        super().__init__(binpath, qtpath)
        pInfo("Creating a new deployment for ComputerVisionBlueprint application")
        pass

    def getExecutable(self) -> str:
        return "/ComputerVisionBlueprint"

    def process(self):
        super().process()
        # get the dependencies of the executable and add only the one that in the path contain "Qt"
        ldd_output = self.getLddOutput(self.bin)
        # copy every library that starts with "/lib/"
        requiredLibs = set()
        for inner_lib in ldd_output:
            if not inner_lib.startswith("/lib/"):
                requiredLibs.add(inner_lib)
                # print("\t Required: " + inner_lib)

        self.libs.update(requiredLibs)

    def getAdditionalLibraries(self) -> dict:
        additionalLibs = super().getAdditionalLibraries()
        # from the Qt path, get the QtPath/lib/libQt6XcbQpa.so.6
        xcbqpa_lib = self.qtpath + "/lib/libQt6XcbQpa.so.6"
        if not os.path.isfile(xcbqpa_lib):
            pError("The library does not exists: " + xcbqpa_lib)
            sys.exit(1)
        # add the libQt6OpenGL.so.6 to the additional libraries
        qtOpengl_lib = self.qtpath + "/lib/libQt6OpenGL.so.6"
        if not os.path.isfile(qtOpengl_lib):
            pError("The library does not exists: " + qtOpengl_lib)
            sys.exit(1)
        # add the libQt6Svg.so.6 to the additional libraries
        qtSvg_lib = self.qtpath + "/lib/libQt6Svg.so.6"
        if not os.path.isfile(qtSvg_lib):
            pError("The library does not exists: " + qtSvg_lib)
            sys.exit(1)
        additionalLibs[qtSvg_lib] = "lib"
        additionalLibs[xcbqpa_lib] = "lib"
        additionalLibs[qtOpengl_lib] = "lib"
        return additionalLibs


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Create a AppDir structure")
    parser.add_argument("-q", "--qt", help="The Qt path", required=True)
    parser.add_argument("-p", "--path", help="Path to executable", required=True)
    # argument -d or --destiny where the AppDir will be created
    parser.add_argument("-d", "--destiny", help="The destiny path where the AppDir will be created", required=True)

    args = parser.parse_args()

    libs = set()
    bins = set()

    computerVisionBlueprint = ComputerVisionBlueprint(args.path, args.qt)
    computerVisionBlueprint.process()
    libs.update(computerVisionBlueprint.getLibs())
    bins.add(computerVisionBlueprint.getBin())
    aditional = computerVisionBlueprint.getAdditionalLibraries()
    additionalLibrary = computerVisionBlueprint.getAdditionalLibraries()
    additionalPaths = computerVisionBlueprint.getAdditionalPaths()

    # if the appdir exists, remove it
    if os.path.exists(args.destiny):
        pInfo("The AppDir exists, removing it")
        shutil.rmtree(args.destiny)
        pOk("AppDir removed")

    # create the AppDir structure
    pInfo("Creating the AppDir structure")
    createAppDirStructure(args.destiny)
    pOk("AppDir structure created")
    # create the AppRun script
    pInfo("Creating the AppRun script")
    create_apprun_script(args.destiny, "ComputerVisionBlueprint")
    pOk("AppRun script created")
    # print the libraries
    pInfo("This libraries will be copied to the AppDir:")
    for lib in libs:
        print("\t" + lib)
    pInfo("This binaries will be copied to the AppDir:")
    for bin in bins:
        print("\t" + bin)
    # copy the additional libraries into the 'value' path
    pInfo("Copying the additional libraries to the AppDir")
    for lib, value in additionalLibrary.items():
        shutil.copy(lib, args.destiny + "/usr/" + value + "/")
    pOk("Additional libraries copied")
    # copy the libraries to the AppDir
    pInfo("Copying the libraries to the AppDir")
    for lib in libs:
        shutil.copy(lib, args.destiny + "/usr/lib/")

    # copy from the QtPath all the folders and its content from QtPath/plugins to AppDir/usr/plugins
    pInfo("Copying the Qt plugins to the AppDir")
    shutil.copytree(args.qt + "/plugins", args.destiny + "/usr/plugins")
    pOk("Qt plugins copied")

    pInfo("Copying the binaries to the AppDir")
    for bin in bins:
        shutil.copy(bin, args.destiny + "/usr/bin/")
