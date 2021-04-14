# Architecture {#ArchitecturePage}

[TOC]

## General

![Basic setup](architecture01.png)

Space Creator consits of 3 major parts. The 3 editors are for different file formats: aadl, asn.1 and msc.
Each of the editor is implemented in a separate library (or set of libraries). Common code that is used by
more than one editor is in the libraries:

## Common used libraries

* **shared** Generally useful code for all or at least 2 editor

* **templates** Common code to save data via grantlee.

* **asn1editor** Is a UI editor to edit ASN.1 values


## IV editor

* **ivcore** Contains the data structures for InterfaceView.

* **libivedtor** Contains the application, including: UI, graphics elments, file handling


## ASN.1 editor

* **asn1library** Contains the data and parsing for ASN1 files.

* **asn1plugin** The actual editor as a pure QtCtreator plugin


## MSC editor

The MSC editor code is separated in 3 different libraries:

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

* **spacecreatorsystem** Is one library to mainly do checks, suggestions, etc. based on the other parts of the system. 
And put all files/data of one project in one common place.

* **spacecreatorplugin** Is the actual plugin. mainly a rather thin wrapper around the existing parts.
