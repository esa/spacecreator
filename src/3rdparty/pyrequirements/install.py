
from importlib.metadata import version
from subprocess import check_call, CalledProcessError
import os
import pathlib
import shutil

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

def cmake_prepare(QtPrefixPath, environ=os.environ):
    try:
        check_call(['cmake', '..', "-DCMAKE_PREFIX_PATH=" + QtPrefixPath], env=environ)
    except CalledProcessError as e:
        print(e.output)

def cmake_build(ncpus=1, environ=os.environ):
    try:
        check_call(['make', '-j' + str(ncpus)], env=environ)
    except CalledProcessError as e:
        print(e.output)

def check_install_qt_dev(version):
    try:
        qt_install_dir = os.path.dirname(os.path.realpath(__file__)) + "/Qt"
        if not os.path.exists(qt_install_dir):
            cmd = "aqt install-qt --outputdir " + qt_install_dir + " --base 'https://download.qt.io' linux desktop " + version
            import shlex
            check_call(shlex.split(cmd))
        return qt_install_dir
    except CalledProcessError as e:
        print(e.output)

PySide6_version = module_version("PySide6")
check_or_raise("pip")
check_or_raise("PySide6")
check_or_install("shiboken6", PySide6_version, "--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io")
check_or_install("shiboken6-generator", module_version("PySide6"), "--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io")

#Install system dependencies using apt
apt_install(["vim", "llvm-13", "llvm-13-dev", "libclang-13-dev", "clang-13", "patchelf", "ninja-build"])

#Create symlink to llvm-13-config on /usr/bin
try:
    os.symlink("/usr/bin/llvm-config-13", "/usr/bin/llvm-config")
except FileExistsError:
    print ("The symlink already exists")

#Create build directory in this script folder and change to it
build_dir = os.path.dirname(os.path.realpath(__file__)) + "/build"
shutil.rmtree(build_dir)
os.mkdir(build_dir)
os.chdir(build_dir)

#Get number of cores
ncpus = os.cpu_count()

#Check (and install if necessary a Qt development libraries))
Qt_path = check_install_qt_dev(PySide6_version)

#Set LD_LIBRARY_PATH to point to Qt PySide6 Qt libraries
env_copy = os.environ.copy()
env_var = env_copy.get("LD_LIBRARY_PATH")
if env_var:
    env_copy["LD_LIBRARY_PATH"] += os.pathsep + os.path.join(Qt_path, PySide6_version, "gcc_64","lib")
else:
    env_copy["LD_LIBRARY_PATH"] = os.path.join(Qt_path, PySide6_version, "gcc_64","lib")

#Building PyRequirements
cmake_prepare(os.path.dirname(os.path.realpath(__file__)) + "/Qt/" + PySide6_version + "/gcc_64", env_copy)
cmake_build(ncpus - 1, env_copy)
