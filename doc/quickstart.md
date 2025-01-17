# SpaceCreator
This document describes how to clone, build and run SpaceCreator. The first sections
details how to set up an environment in which to build. The second section how to build and run
SpaceCreator on the commandline, and the final section deals with getting the build and run
steps to work from with in QtCreator.

## GitLab
[https://gitrepos.estec.esa.int/taste/spacecreator](https://gitrepos.estec.esa.int/taste/spacecreator)

Clone to ~/project/spacecreator
```commandline
~/project$ git clone https://gitrepos.estec.esa.int/taste/spacecreator.git
```

## Project

### Introduction
SpaceCreator is a plugin for QtCreator that enables the user to define systems in a visual editor, much like a Visual UML Editor. Since SpaceCreator is also developed in QtCreator, two QtCreators are at work: The one used by the developer (that’s you) to develop the SpaceCreator plugin, and the one used by the user (the nice people at ESA) to load the SpaceCreator plugin and create space systems. To distinguish the two we call them:

**Black QtC**<br>
Used by developer to develop SpaceCreator plugin.

**Blue QtC**<br>
Used by users to load SpaceCreator plugin. It is a normal QtCreator that has headers for plugin development placed along side it.

Blue QtCreator and the SpaceCreator plugin needs to be compiled against the EXACT same Qt version. Furthermore, Blue QtCreator needs to be the source from that exact Qt version as well. To garantee this, the SpaceCreator project has a script for downloading and installing a specific version of Qt and the corresponding QtCreator sourcecode.

## Prerequisites and building environment
Before we can work with SpaceCreator we need tools to be available on the computer
and some libraries to compile against. To simplify this, we will use a ansible scripts that will install the dependencies
as well as download and building all the subproject required to build spacecreator.

For this we need to install ansible with the following command: <br>

`$ sudo apt install -y ansible`


Now we can run the ansible script located in `<project_dir>/scripts/Ansible_scripts/` with the following parameters:

`ansible-playbook build.yml
-e env_dir=/home/taste/envs/projectcreatorenv6
-e env_qt_version=6.4.3
-e qtc_version=11.0.3
-e project_dir=/home/taste/project/spacecreator
-e patch_version=0
-e download_dir=/home/taste/Downloads
`

This will create a folder (~/project/spacecreatorenv6) containing (among other) the files:
```
├── asn1scc/
├── Qt/
│   └── 6.4.3/
│       └── gcc_64/
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

## Running SpaceCreator

Before running the blue QtCreator we need to copy the plugin from the build folder
to the spacecreator.AppDir folder.
In addition, we need to copy templates for creating spacecreator projects and spacecreator files.
This is done by running the following command:
```commandline
~/project/spacecreator$ python3 ./scripts/postbuild.py
--build_dir $HOME/project/spacecreator_build
--app_dir $HOME/project/spacecreatorenv6/spacecreator.AppDir
```

Then the blue QtCreator can be run:<br>
`$ $HOME/project/spacecreatorenv6/spacecreator.AppDir/bin/qtcreator -color blue`<br>
and it will now be possible to load a sample project.



## Setting up QtCreator
To develop the spacecreator plugin in black QtCreator we need to setup a project.
Note that values entered in QtCreator is not substituted like in a shell, so
$HOME and ~/ does not work. Instead we assume the user to be 'taste'

### Manage Kits...
We want to use the Qt installation in our build environment (/home/taste/project/spacecreatorenv6/Qt)
So we go to ***Kits / Qt Version*** and create a manual Qt Version named ***spacecreatorenvqt6***.
Then we make a new Manual Kit named ***spacecreator6*** where we select the Qt version above.

In the ***CMake Configuration*** inside the kit, click the ***Change...*** button.
Add two lines there:<br>
QTC_SOURCE:PATH=/home/taste/project/spacecreatorenv6/spacecreator.AppDir<br>
QTC_INSTALL:PATH=/home/taste/project/spacecreatorenv6/spacecreator.AppDir


### Build Settings
Variables:<br>
Turn ENABLE_FORMAT_CHECK off.

Click ***Run CMake***

### Run Settings
We need to have QtCreator run the postbuild.py script after the build stop, so we add a
***Deployment Method*** that is a Custom Process Step. The command is
`python3` and the arguments are:

`%{ActiveProject:Path}/scripts/postbuild.py --build_dir=%{buildDir} --app_dir=/home/taste/project/spacecreatorenv6/spacecreator.AppDir`
This will call the correct **postbuild.py** script and tell it where the plugins were build and where to put them.<br>
The variables `%{ActiveProject:Path}` and `%{buildDir}` are substituted by QtCreator.

Now we need a Run configuration of the type ***Custom Executable***:<br>
**Executable**: `/home/taste/project/spacecreatorenv6/spacecreator.AppDir/bin/qtcreator`<br>
**Command line arguments**: `-color blue`



