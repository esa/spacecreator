# Initial document

** Requirements **

* Configure the HW library in the settings to point to `examples/deploymentview` in the repository.

**1. Steps:**

* Drag the `Linux on ARM` from the HW library to the graphics scene
* Drag the `Linux on x86` from the HW library to the graphics scene

Expected results:

* A new node `Linux on ARM 1` is added. A `Partition 1` is inside the node. Devices `eth0` to `eth7` are attached
* A new node `Linux on x86 1` is added. A `Partition 1` is inside the node. One Device `eth0` is attached

**2. Steps: **

* Press Ctrl while pressing the left mouse button to connect the two `eth0`

Expected results:

* A line connection the two devices is drawn

**3. Steps: **

* Press undo 3 times

Expected results:

* The clean document is shown

**4. Steps:**

* Press redo 3 times
* Save the file
* Restart the DV editor aplication
* Load the saved file

Expected results:

* The two nodes and it's partitions and devices an the connection is shown
* The nodes are shown in the tree view as well
