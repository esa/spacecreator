# Architecture

[TOC]

## General

![Basic setup](architecture01.png)

Space Creator consits of 3 major parts. The 3 editors are for firrerent file formats: aadl, asn.1 and msc.
Each of the editor is implemented in a separate library (or set of libraries). Common code that is used by
more than one editor is in the libraries:

## Common used libraries

* **shared** Generally useful code for all or at lesat 2 editor

* **templates** Code to save data via grantlee.


## AADL/IV editor

* **aadl** contains the data strucures for aadl

* **libivedtor** contains the application, including: UI, graphics elments, file handling


## ASN.1 editor

TBD


## MSC editor

The msc editor is separated in 3 different libraries:

* **msclibrary** handling the .msc file and data structure

* **mscwidgets** containing all graphical items

* **libmsceditor** has the application itself. With the class [MainModel](@ref MainModel) as the
central hub of the msc editor.

The library **asn1editor** will be replaces by the ASN.1 editor.
