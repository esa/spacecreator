#!/bin/bash

# How to use this with python virtual environment:
#
# If you have installed and activated the environment, get rid of it:
# $ deactivate
# $ rm -rf ./venv
#
# Create and activate the environment:
# $ virtualenv --python=python3.7 ./venv
# $ source ./venv/bin/activate
#
# Run this script:
# $ ./install.sh -q=6.4.0 -p=./venv/bin -s=~/develop/esa/spacecreator.env6/Qt/6.4.0/gcc_64

#Helper functions
#Gets the version of a python module passed as parameter
#Outputs "N/A" if the module is missing
function py_module_ver()
{
	_VERSION=$(python3 -c "exec(\"from importlib.metadata import version\ntry: print(version('$1'))\nexcept:print ('N/A')\")")
	echo "$_VERSION" 
}

#Get the path for a python modules passed as parameter
function py_module_path()
{
	_PATH=$(python3 -c "import pathlib, $1; path=pathlib.Path($1.__file__).resolve().parent.__fspath__(); print(path)")
	echo "$_PATH" 
}

#installing dependencies
sudo apt-get remove llvm-7
sudo apt-get install llvm-13 llvm-13-dev libclang-13-dev clang-13


#Debian 10 cmake setup
OS=$(lsb_release -i)
OS_VERSION=$(lsb_release -ar | grep -i release | cut -s -f2 )

echo -e "OS\t= ${OS}"
echo -e "OS_VERSION\t= ${OS_VERSION}"

if ([[ $OS == *Debian* ]] && [[ $OS_VERSION == 10 ]]) then
	sudo apt update
	sudo apt install -t buster-backports cmake
fi


#Defaults
export QT_VER_DEFAULT=6.4.3
export QT_VER_FULL=$QT_VER_DEFAULT
export PYTHON_PATH=$(which python3)

#Parsing parameters
for i in "$@"; do
  case $i in
    -q=*|--qt=*)
      QT_VER_FULL="${i#*=}"
      shift # past argument=value
      ;;
    -p=*|--python=*)
      PYTHON_PATH="${i#*=}"
      shift # past argument=value
      ;;
    -s=*|--scqt=*)
      SPACECREATOR_QT_PATH="${i#*=}"
      shift # past argument=value
      ;;
    -*|--*)
      echo "Unknown option $i"
      exit 1
      ;;
    *)
      ;;
  esac
done

#Preinstall checks

PROJECT_PATH=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PYTHON3_EXE=${PYTHON_PATH}
PIP3_EXE=$(which pip3)

#Check if Python is available
if ! [ -f ${PYTHON3_EXE} ]; then
  echo "Python interpreter not found: ${PYTHON3_EXE}"
  exit 1
fi

#Check if pip is available
if ! [ -f ${PIP3_EXE} ]; then
  echo "PIP not found: ${PIP3_EXE}"
  exit 1
fi

#Autodetect software versions
PYTHON_VER=$(python3 --version)
PYSIDE_VER=$(py_module_ver "PySide6") 
SHIBOKEN_VER=$(py_module_ver "shiboken6") 

if [[ $PYSIDE_VER != "N/A" ]]; then
	export PYSIDE_PATH=$(py_module_path "PySide6")
	PYSIDE_QT_PATH="$PYSIDE_PATH/Qt/lib"	
	QT_VER=$($PYSIDE_QT_PATH/libQt6Core.so.6 |grep version |awk 'BEGIN {FS=" Qt ";}{print $2}'| cut -d ' ' -f1)
	export QT_VER_FULL=$([ -z "$QT_VER" ] && echo "$QT_VER" || echo "$QT_VER_DEFAULT") 
else
	QT_VER_SHORT=${QT_VER_FULL%.*}
	${PIP3_EXE} install pyside6==$QT_VER_SHORT setuptools GitPython aqtinstall --break-system-packages
	export PYSIDE_PATH=$(py_module_path "PySide6")
fi
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PYSIDE_QT_PATH

if [[ $SHIBOKEN_VER = "N/A" ]]; then
	#Installing shiboken dependencies
	sudo apt install patchelf ninja-build
	#Install Shiboken/Shiboken generator
	FROM_QT="--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io"
	${PIP3_EXE} install shiboken6==$QT_VER_SHORT $FROM_QT
	${PIP3_EXE} install shiboken6-generator==$QT_VER_SHORT $FROM_QT
	
	#llvm-config symlink for shiboken
	sudo ln -s /usr/bin/llvm-config-13 /usr/bin/llvm-config

fi

SPACECREATOR_QT_PATH=/home/taste/envs/projectcreatorenv6/Qt/$QT_VER_FULL/gcc_64

echo -e "QT_VER_FULL\t= ${QT_VER_FULL}"
echo -e "PYTHON_PATH\t= ${PYTHON_PATH}"
echo -e "PYTHON3_EXE\t= ${PYTHON3_EXE}"
echo -e "PIP3_EXE\t= ${PIP3_EXE}"
echo -e "PROJECT_PATH\t= ${PROJECT_PATH}"
echo -e "SPACECREATOR_QT_PATH\t= ${SPACECREATOR_QT_PATH}"

CPU_CORE_COUNT=$(nproc --all)
if [ $CPU_CORE_COUNT -gt 1 ]; then
  CPU_CORE_COUNT=$((CPU_CORE_COUNT-1)) # to not freeze a weaky laptop :)
fi

echo "CPUs: ${CPU_CORE_COUNT}"

mkdir -p $PROJECT_PATH/build
cd $PROJECT_PATH/build
cmake .. -DCMAKE_PREFIX_PATH=$SPACECREATOR_QT_PATH
make -j${CPU_CORE_COUNT}
