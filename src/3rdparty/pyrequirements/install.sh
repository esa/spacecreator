#!/bin/bash

# How to use this with python virtual environment:
#
# If you have installed and activated the environment, get rid of it:
# $ deactivate # if you have activated venv
# $ rm -rf ./venv
#
# Create and activate the environment:
# $ virtualenv --python=python3.7 ./venv
# $ source ./venv/bin/activate
#
# Run this script:
# $ ./install.sh -q=6.4.0 -p=./venv/bin -s=~/develop/esa/spacecreator.env6/Qt/6.4.0/gcc_64

#Set here the version of Qt in use
export QT_VER_FULL=6.4.3
export PYTHON_PATH="/usr/bin"
SPACECREATOR_QT_PATH=/home/taste/envs/projectcreatorenv6/Qt/$QT_VER_FULL/gcc_64

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

QT_VER_SHORT=${QT_VER_FULL%.*}
PROJECT_PATH=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PYTHON3_EXE=${PYTHON_PATH}/python3
PIP3_EXE=${PYTHON_PATH}/pip3

echo -e "QT_VER_FULL\t= ${QT_VER_FULL}"
echo -e "PYTHON_PATH\t= ${PYTHON_PATH}"
echo -e "PYTHON3_EXE\t= ${PYTHON3_EXE}"
echo -e "PIP3_EXE\t= ${PIP3_EXE}"
echo -e "PROJECT_PATH\t= ${PROJECT_PATH}"
echo -e "SPACECREATOR_QT_PATH\t= ${SPACECREATOR_QT_PATH}"

if ! [ -f ${PYTHON3_EXE} ]; then
  echo "Python interpreter not found: ${PYTHON3_EXE}"
  exit 1
fi

if ! [ -f ${PIP3_EXE} ]; then
  echo "PIP not found: ${PIP3_EXE}"
  exit 1
fi

#installing dependencies
sudo apt-get remove llvm-7
sudo apt-get install llvm-13 llvm-13-dev libclang-13-dev clang-13

#llvm-config symlink for shiboken
sudo ln -s /usr/bin/llvm-config-13 /usr/bin/llvm-config

#Debian 10 cmake setup
OS=$(lsb_release -i)
OS_VERSION=$(lsb_release -ar | grep -i release | cut -s -f2 )

echo -e "OS\t= ${OS}"
echo -e "OS_VERSION\t= ${OS_VERSION}"

if ([[ $OS == *Debian* ]] && [[ $OS_VERSION == 10 ]]) then
	sudo apt update
	sudo apt install -t buster-backports cmake
fi

#Shiboken dependencies
sudo apt install patchelf ninja-build
FROM_QT="--index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io"
${PIP3_EXE} install shiboken6==$QT_VER_SHORT $FROM_QT
${PIP3_EXE} install shiboken6-generator==$QT_VER_SHORT $FROM_QT
${PIP3_EXE} install pyside6==$QT_VER_SHORT setuptools GitPython aqtinstall --break-system-packages

PYTHON_VERSION=$(${PYTHON3_EXE} -c "import platform; print(platform. python_version())")
PYSIDE6_QT_LIBS=$(${PYTHON3_EXE} -c 'import sysconfig; print(sysconfig.get_paths()["purelib"])')
echo -e "PYSIDE6_QT_LIBS\t= ${PYSIDE6_QT_LIBS}"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PYSIDE6_QT_LIBS/PySide6/Qt/lib/

CPU_CORE_COUNT=$(nproc --all)
if [ $CPU_CORE_COUNT -gt 1 ]; then
  CPU_CORE_COUNT=$((CPU_CORE_COUNT-1)) # to not freeze a weaky laptop :)
fi

echo "CPUs: ${CPU_CORE_COUNT}"

mkdir -p $PROJECT_PATH/build
cd $PROJECT_PATH/build
cmake .. -DCMAKE_PREFIX_PATH=$SPACECREATOR_QT_PATH
make -j${CPU_CORE_COUNT}
