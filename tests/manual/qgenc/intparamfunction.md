# Testing function with integer type param

**1. Steps:**

* Open project `tests/manual/qgenc/projects/integerparam/integerparam.pro`
* Open the `interfaceview.xml` file
* Clean the project
* Right click on the interfaceview and select the "Generate/update code skeletons" option
* Build and run the project

Expected results:

* Project should succesfully build and run.
* It should display the "Output: 10" message in a loop informing about the QGen function output.
* No "assertion failed" info should appear.
