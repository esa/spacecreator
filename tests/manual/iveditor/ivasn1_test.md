# ASN.1 usage

**1. Steps:**

* Open the project 'examples/project01/project01.pro' in QtCreator
* Open the `interfacevieweditor.xml` file
* Double click on function `ParamFunc` to open the properties dialog
* Press the "+" button to add a new parameter
* Select `MyEnum` from the list of combo box as type
* Double click the value
* In the new dialog select `three` as value
* Press the button "Ok" and "Ok" to close all dialogs

Expected results:

* The type combobox has 16 types
* The ASN1 value dialog can be used
* After pressing "ok", the property show the correct value `three`


**2. Steps:**

* Open the project 'examples/project01/project01.pro' in QtCreator
* Open the `interfacevieweditor.xml` file
* Open the `dataview-uniq.asn` file
* Remove the line `T-Boolean ::= BOOLEAN`
* Save the file

Expected Results:

* An error is raised
