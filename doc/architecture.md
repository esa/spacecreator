# Architecture {#ArchitecturePage}

[TOC]

## General

![Basic setup](architecture01.png)

SpaceCreator is implemented via plugins for the [QtCreator](https://www.qt.io/product/development-tools) application. One plugin is for editing ASN.1/ACN files in a text editor.

The SpaceCreator plugin for QtCreator contains 3 graphical editors:

* Deployment view editor (DV)
* Interface view editor (IV)
* MSC editor

In addition to the editor in the plugin, dedicated stand alone applications for each editor exists. Mainly for easier development.


Each of the editor is implemented in a separate library (or set of libraries). Common code that is used by
more than one editor is in the libraries:

## Common used libraries

* **shared** Generally useful code for all or at least 2 editor

* **templates** Common code to save data via grantlee.

* **asn1library** Has the basic ASN.1 data and loading of .asn files.

* **asn1editor** Is a UI editor to edit ASN.1 values.


## ASN.1 editor

* **asn1library** Contains the data and parsing for ASN1 files.

* **asn1plugin** The actual editor as a pure QtCtreator plugin


## DV editor

* **dvcore** Contains the data structures for deployment view. The data is stored in a custom XML format.

* **libdvedtor** Contains the application, including: UI, graphics elments, file handling


## IV editor

* **ivcore** Contains the data structures for interface view. The data is stored in a custom XML format.

* **libivedtor** Contains the application, including: UI, graphics elments, file handling


## MSC editor

The MSC editor code is separated in 2 different libraries:

* **msccore** Handling the .msc file and data structure. For parsing .msc files [ANTLR4](https://www.antlr.org) is used. For writing .msc files, [Grantlee](https://github.com/steveire/grantlee) is used.
This includes handling of CIF information for types and geometry, which is stored as comments of the msc elements.

* **libmsceditor** Has the Ui elements an the application itself. With the class [msc::MainModel](@ref msc::MainModel) as the
central hub of the MSC editor.
The charts are drawn using the QGraphicsScene framework. The layout of the more complex chart view is controlled by [msc::ChartLayoutManager](@ref msc::ChartLayoutManager).


### Main MSC building blocks {#MSCMainBuildingBlocks}

[msc::MscModel](@ref msc::MscModel) holds the data of one .msc file. Whenever the data changes, a signal is emitted and the UI updates accordingly.
Where the UI can be either the tre view showing the hierarchy of documents. The hierarchy showing the documents graphically.
The textual representaion of the whole data (like in a .msc file). Or one single chart.

The most complex view is the one for the chart. It's main class is [msc::ChartLayoutManager](@ref msc::ChartLayoutManager).

For laoding and saving .msc files the classes [msc::MscReader](@ref msc::MscReader) and [msc::MscWriter](@ref msc::MscWriter) are used.


## SpaceCreator

SpaceCreator is combining everything (except the asn.1 plugin) in one plugin for QtCreator.

* **spacecreatorsystem** Is one library to do checks, suggestions, set constraints, etc. based on the other parts of the system.
And put all files/data of one project in one common place.

* **spacecreatorplugin** Is the actual QtCreator plugin. Mainly a rather thin wrapper around the existing parts.
