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
Before we can work with SpaceCreator we need tools to be available on the computer 
and some libraries to compile against.

### Tools

**Apt**<br>
Install tools by typing: <br>
`$ apt install -y  build-essential git g++ cmake cmake-data ninja-build make gcovr lcov ccache cccc doxygen graphviz zip p7zip curl wget default-jre uuid-dev pkg-config fuse python3-pip clang-format spin python3-pygraphviz python3-singledispatch python3-stringtemplate3 python3-pexpect`

**CMake** <br>
Because Qt 6 requires CMake 3.16 or later, we need to install a backport when running Debian 10 (buster). 
This done by editing `/etc/apt/sources.list` and adding the line <br>
`deb http://ftp.debian.org/debian buster-backports main` <br>
Then update Apt for the changes to take effect<br>
`$ sudo apt update`<br>
Finally install CMake 18 by typing<br>
`$ sudo apt install -t buster-backports cmake`

**Python**<br>
Python needs some additional packages which can be installed by typing: <br>
`$ pip3 install black==21.10b0 multipledispatch pyside6 pytest py7zr aqtinstall GitPython`

### Build environment

Blue QtCreator and the SpaceCreator plugin needs to be compiled against the EXACT same Qt version. Furthermore, Blue QtCreator needs to be the source from that exact Qt version as well. To garantee this, the SpaceCreator project has a script for downloading and installing a specific version of Qt and the corresponding QtCreator sourcecode. 
Running the following command:

`~/projects/spacecreator$ python3 ./scripts/prebuild.py --output_dir $HOME/opt/spacecreatorenv6 --qt_version 6.3.1 --qtcreator_version 8.0.1
`

will create a folder (~/opt/spacecreatorenv6) containing (among other) the files:
```
├── asn1scc/
├── Qt/
│   └── 6.3.1/
└── spacecreator.AppDir/
    ├── bin/
    ├── include/
    ├── lib/
    │   └── Qt/
    │       └── bin/
    │       └── lib/
    │           ├── grantlee/
    │           │   ├── 5.2/
    │           │   │   ├── grantlee_defaultfilters.so
    │           │   │   └── ... 4 more grantlee_x.so files
    │           │   └── libGrantlee_Templates.so  (these libGrantlee_Templates.so needs the grantlee_x.so files above)
    │           ├── qtcreator/
    │           │   └── plugins/
    │           │        ├── libSpaceCreatorPlugin.so
    │           │        ├── libSedsPlugin.so
    │           │        ├── libDeviceGeneratorPlugin.so
    │           │        ├── libFunctionTesterPlugin.so
    │           │        └── ...
    │           └── libXxx.so          
    ├── libexec/
    └── share/
        └── qtcreator/
            └── templates/
                └── wizards/
                   ├── files/
                   └── projects/
   
```  



## Building the SpaceCreator Plugin

Running the following command will build the SpaceCreator plugin:

`~/projects/spacecreator$ python3 ./scripts/build_spacecreator.py
--env_dir $HOME/opt/spacecreatorenv6
--env_qt_dir  $HOME/opt/spacecreatorenv6/Qt/6.3.1/gcc_64`

This will build the plugin in `~/projects/spacecreator/build`

## Running SpaceCreator

Before running the blue QtCreator we need to copy the plugin from the build folder 
to the spacecreator.AppDir folder.
In addition, we need to copy templates for creating spacecreator projects and spacecreator files.
This is done by running the following command:

`~/projects/spacecreator$ python3 ./scripts/postbuild.py --env_dir=$HOME/opt/spacecreatorenv6`

Then the blue QtCreator can be run:<br>
`$ ~/opt/spacecreator.AppDir/bin/qtcreator -color blue`<br>
and it will now be possible to load a sample project.



## Setting up QtCreator
To develop the spacecreator plugin in black QtCreator we need to setup a project.

### Manage Kits...
We want to use the Qt installation in our build environment (~/opt/spacecreatorenv6/Qt)
So we go to ***Kits / Qt Version*** and create a manual Qt Version named ***spacecreatorenvqt6***.
Then we make a new Manual Kit named ***spacecreator6*** where we select the Qt version above.

In the ***CMake Configuration*** inside the kit, click the ***Change...*** button.
Add two lines there:<br>
QTC_SOURCE:PATH=$HOME/opt/spacecreatorenv5/spacecreator.AppDir<br>
QTC_INSTALL:PATH=$HOME/opt/spacecreatorenv5/spacecreator.AppDir


### Build Settings
Variables:<br>
Turn ENABLE_FORMAT_CHECK off.

Click ***Run CMake***

### Run Settings
We need to have QtCreator run the postbuild.py script after the build stop, so we add a 
***Deployment Method*** that is a Custom Process Step. The command is
`python3` and the arguments are:

`%{ActiveProject:Path}/scripts/postbuild.py --env_dir=$HOME/opt/spacecreatorenv6 --env_qt_dir=$HOME/opt/spacecreatorenv6/Qt/6.3.1/gcc_64 --build_dir=%{buildDir}`

This will call the correct **postbuild.py** script and tell it where the build environment is and where the plugins were build.<br>
The variables `%{ActiveProject:Path}` and `%{buildDir}` are substituted by QtCreator.

Now we need a Run configuration of the type ***Custom Executable***:<br>
**Executable**: `$HOME/opt/spacecreatorenv6/spacecreator.AppDir/bin/qtcreator`<br>
**Command line arguments**: `-color blue`



