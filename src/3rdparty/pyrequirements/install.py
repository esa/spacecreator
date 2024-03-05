#!/usr/bin/python3

from importlib.metadata import version
from subprocess import check_call, CalledProcessError
import os
import pathlib
import shutil
import platform
import distro
import pip

def module_version(moduleName):
    """
        Returns the version of the module moduleName. 
        Otherwise returns the string "N/A" 
    """
    try: 
        return(version(moduleName))
    except:
        return('N/A')


def module_path(moduleName):
    try:
        module = __import__(moduleName)
        return os.path.dirname(module.__file__)
    except:
        return('N/A')


class SystemInfo:
    def __init__(self):
        self.python_version = platform.python_version()
        self.qt_version = module_version("PySide6")
        if self.qt_version == "N/A":
            self.qt_version = "6.5.3"
        self.qt_path = os.path.dirname(os.path.realpath(__file__)) + "/Qt"
        self.qt_base_path = os.path.join(self.qt_path, self.qt_version, "gcc_64")
        self.qt_lib_path = os.path.join(self.qt_base_path, "lib")
        self.distribution_name = distro.id()
        self.distributin_version = distro.version()
        self.source_dir = os.path.dirname(os.path.realpath(__file__))
        self.build_dir = self.source_dir + "/build"
    
    def print_info(self):
        print("Python version      : " + self.python_version)
        print("Qt/PySide version   : " + self.qt_version)
        print("Qt path             : " + self.qt_path)
        print("Distribution        : " + self.distribution_name)
        print("Distribution version: " + self.distributin_version)
        print("Source directory    : " + self.source_dir)
        print("Build  directory    : " + self.build_dir)


def check_or_raise(moduleName):
    if (module_version(moduleName) == "N/A"):
        raise RuntimeError(moduleName + " was not found. Please install it and run this script again") 


def check_or_install(pkgName, version, extra_args=""):
    pkg_ver = pkgName if not version else pkgName + "==" + version
    pkg_ver_args = pkg_ver if not extra_args else pkg_ver + extra_args
    if (module_version(pkgName) == "N/A"):
        import sys
        check_call([sys.executable, '-m', 'pip', 'install', pkg_ver, "--break-system-packages"])


def apt_install(packagesNames):
    """
        packagesNames is an array of strings that contains
        the names of the packages to install using apt
    """
    try:
        check_call(['sudo', 'apt', 'install', '-y', *packagesNames], stdout=open(os.devnull,'wb'))
    except CalledProcessError as e:
        print(e.output)


def clang_check():
    config_file = '/usr/bin/llvm-config-'
    if  distro.id() == 'debian':
        if distro.version() == "11":
            config_file += '13'
            #Install system dependencies using apt
            apt_install(["vim", "llvm-13", "llvm-13-dev", "libclang-13-dev", "clang-13", "patchelf", "ninja-build"])

    if  distro.id() == 'ubuntu':
        if distro.version() == "20.04":
            config_file += '12'
            #Install system dependencies using apt
            apt_install(["vim", "llvm-12", "llvm-12-dev", "libclang-12-dev", "clang-12", "patchelf", "ninja-build"])
            
    #Create symlink to llvm-config on /usr/bin
    try:
        os.symlink(config_file, "/usr/bin/llvm-config")
    except FileExistsError:
        print ("The symlink already exists")
    except OSError:
        check_call(['sudo', 'ln', '-s', config_file, '/usr/bin/llvm-config'])


def cmake_prepare(info, environ=os.environ):
    try:
        check_call(['cmake', '-S', info.source_dir, '-B', info.build_dir, "-DCMAKE_PREFIX_PATH=" + info.qt_base_path], env=environ)
    except CalledProcessError as e:
        print(e.output)


def cmake_build(info, ncpus=1, environ=os.environ):
    try:
        check_call(['cmake', '--build', info.build_dir, '-j' + str(ncpus)], env=environ)
    except CalledProcessError as e:
        print(e.output)

def cmake_install(info):
    try:
        check_call(['cmake', '--install', info.build_dir])
    except CalledProcessError as e:
        print(e.output)


def check_install_qt_dev(info):
    try:
        if not os.path.exists(info.qt_path):
            cmd = "aqt install-qt --outputdir " + info.qt_path + " --base 'https://download.qt.io' linux desktop " + info.qt_version
            import shlex
            check_call(shlex.split(cmd))
        return info.qt_path
    except CalledProcessError as e:
        print(e.output)


# Main
print("** Gain system info")
info = SystemInfo()
info.print_info()

print("** Check / install PySide + shiboken")
check_or_install("aqtinstall", "", "--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io")
check_or_install("PySide6", info.qt_version, "--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io")
check_or_install("shiboken6", info.qt_version, "--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io")
check_or_install("shiboken6-generator", info.qt_version, "--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io")

import aqt

print("** Check / install clang")
clang_check()

print("** Check (and install if necessary a Qt development libraries))")
check_install_qt_dev(info)

print("** Clear build directory")
# Create build directory in this script folder and change to it
build_dir = info.build_dir
if os.path.isdir(build_dir):
    shutil.rmtree(build_dir)
os.mkdir(build_dir)
#os.chdir(build_dir)

print("** Build module")
# Set LD_LIBRARY_PATH to point to Qt PySide6 Qt libraries
env_copy = os.environ.copy()
env_var = env_copy.get("LD_LIBRARY_PATH")
if env_var:
    env_copy["LD_LIBRARY_PATH"] += os.pathsep + info.qt_lib_path
else:
    env_copy["LD_LIBRARY_PATH"] = info.qt_lib_path
print("* info: using LD_LIBRARY_PATH: " + env_copy["LD_LIBRARY_PATH"])
# Building PyRequirements
cmake_prepare(info, env_copy)
# Get number of cores
ncpus = os.cpu_count()
cmake_build(info, ncpus - 1, env_copy)

print("** Install module")
cmake_install(info)
