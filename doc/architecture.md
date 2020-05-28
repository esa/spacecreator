# Architecture

[TOC]

## General

![Basic setup](architecture01.png)

Space Creator consits of 3 major parts. The 3 editors are for firrerent file formats: aadl, asn.1 and msc.
Each of the editor is implemented in a separate library (or set of libraries). Common code that is used by
more than one editor is in the libraries:

## Common used libraries

* **shared** Generally useful code for all or at lesat 2 editor

* **templates** Common code to save data via grantlee.


## AADL/IV editor

* **aadl** Contains the data strucures for aadl.

* **libivedtor** Contains the application, including: UI, graphics elments, file handling


## ASN.1 editor

* **asn1library** Contains the data and parsing for ASN1 files.


## MSC editor

The MSC editor code is separated in 3 different libraries:

* **msclibrary** Handling the .msc file and data structure. For parsing .msc files [ANTLR4](https://www.antlr.org) is used. For writing .msc files, [Grantlee](https://github.com/steveire/grantlee) is used.
This includes handling of CIF information for types and geometry, which is stored as comments of the msc elements.

* **libmsceditor** Has the Ui elements an the application itself. With the class [MainModel](@ref MainModel) as the
central hub of the MSC editor.

The library **asn1editor** will probably be replaced by the ASN.1 editor (asn1library).
