# Testing function with sequence type param

**1. Steps:**

* Open project `tests/manual/qgenc/projects/ia5stringparam/ia5stringparam.pro`
* Open the `interfaceview.xml` file
* Clean the project
* Right click on the interfaceview and select "Generate/update code skeletons" option
* Try to build the project

Results:

* Project should succesfully build and run.
* It should display the "Output: bcdef" message in a loop informing about the QGen function output.
* No "assertion failed" info should appear.
