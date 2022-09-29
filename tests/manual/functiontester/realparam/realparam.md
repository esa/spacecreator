# Testing function with real type param

**1. Steps:**

* Open project `tests/manual/functiontester/projects/realparam/realparam.pro`
* Open the `interfaceview.xml` file
* Build the project
* Select an interface by clicking on the arrow
* Select Tools -> Test Interface -> Test using data from CSV
* Select board (x86 Linux CPP and GRC712RC RTEMS6 SMP QDP are currently supported)
* Click OK
* Choose csv file with test vectors
* Set max delta and click OK to accept

**2. Expected results:**

* Project should succesfully build
* It should launch the default browser with a page displaying results in the form of the array
* All the numbers should be green and there should be no missing cells and no error info
