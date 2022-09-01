# SpaceCreator
Building SpaceCreator

## GitLab
[https://gitrepos.estec.esa.int/taste/spacecreator](https://gitrepos.estec.esa.int/taste/spacecreator)

Clone to ~/projects/spacecreator

## Project

### Introduction
SpaceCreator is a plugin for QtCreator that enables the user to define systems in a visual editor, much like a Visual UML Editor. Since SpaceCreator is also developed in QtCreator, two QtCreators are at work: The one used by the developer (that’s you) to develop the SpaceCreator plugin, and the one used by the user (the nice people at ESA) to load the SpaceCreator plugin and create space systems. To distinguish the two we call them:

**Black QtC**<br>
Used by developer to develop SpaceCreator plugin.

**Blue QtC**<br>
Used by users to load SpaceCreator plugin. It is a normal QtCreator that has headers for plugin development placed along side it.

## Prerequisites
Blue QtCreator and the SpaceCreator plugin needs to be compiled against the EXACT same Qt version. Further more, Blue QtCreator needs to be the source from that exact Qt version as well. To garantee this, the SpaceCreator project has a script for downloading and installing a specific version of Qt and the corresponding QtCreator sourcecode. 
Running the following command:

`~/projects/spacecreator$ python3 ./scripts/prebuild 
    –output_dir /home/<user>/opt/spacecreatorenv6
    –qt_version 6.3.1 
    –qtcreator_version 8.0.1
`

will create a folder (~/opt/spacecreatorenv6) containing:
* Qt 6.3.1
* The template library Grantlee
* A folder named **spacecreator.AppDir** containing QtCreator with dev files

The idea is to have a build environment that is contained in a single folder

## Building the SpaceCreator Plugin

Running the following command will build the SpaceCreator plugin:

`~/projects/spacecreator$ python3 ./scripts/build_spacecreator.py
--env_dir /home/<user>/opt/spacecreatorenv6
--env_qt_dir  /home/<user>/opt/spacecreatorenv6/Qt/6.3.1/gcc_64`

This will build the plugin in ~/projects/spacecreator/build

## Running SpaceCreator

Before running the blue QtCreator we need to copy the plugin from the build folder 
to the spacecreator.AppDir folder.
In addition, we need to copy templates for creating spacecreator projects and spacecreator files.
This is done by running the following command:

`~/projects/spacecreator$ python3 ./scripts/postbuild.py
--env_dir=/home/<user>/opt/spacecreatorenv6 
`

Then the blue QtCreator can be run:
$ ~/opt/spacecreator.AppDir/bin/qtcreator -color blue
and it will now be possible to load a sample project.


## Setting up QtCreator
To develop the spacecreator plugin in black QtCreator we need to setup a project.

### Manage Kits...
We want to use the Qt installation in our build environment (~/opt/spacecreatorenv6/’Qt)
So we go to ***Kits / Qt Version*** and create a manual Qt Version named ***spacecreatorenvqt6***.
Then we make a new Manual Kit named ***spacecreator6*** where we select the Qt version above.

### Build Settings
The CMakeLists.txt file of the project needs to be able to find the grantlee template library and the Qt instance in our build environment, so we give it Additional CMake options: 

-DCMAKE_PREFIX_PATH="/home/fisker/opt/spacecreatorenv6/Qt/6.3.1/gcc_64/lib/cmake;/home/fisker/opt/spacecreatorenv6/grantlee/lib/cmake/Grantlee5"

This should result in the 
CMAKE_PREIFX_PATH variable being updated in the ***Current Configuration*** tab.
Grantlee5_DIR variable being set

Other variables:<br>
QTC_SOURCE - Path to where /lib and /include for QtCreators dev code is located
/home/fisker/opt/spacecreatorenv5/spacecreator.AppDir<br>
QTC_INSTALL - Path to where the spacecreator-plugin is to be put.
/home/fisker/opt/spacecreatorenv5/spacecreator.AppDir

Turn ENABLE_FORMAT_CHECK off.

Click ***Run CMake***

### Run Settings
We need to have QtCreator run the postbuild.py script after the build stop, so we add a 
***Deployment Method*** that is a Custom Process Step. The command is
python3 and the arguments are:

`%{ActiveProject:Path}/scripts/postbuild.py --env_dir=/home/<user>/opt/spacecreatorenv6 --build_dir=%{buildDir}
`

This will call the correct **postbuild.py** script and tell it where the build environment is and where the plugins was build.

Now we need a Run configuration of the type ***Custom Executable***:<br>
**Executable**: `/home/<user>/opt/spacecreatorenv6/spacecreator.AppDir/bin/qtcreator`<br>
**Command line arguments**: `-color blue`



