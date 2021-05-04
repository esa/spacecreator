# 1 Existent layout

**Steps**

* Open file `examples/msc/manual_tests/checklist.sample1.msc`

**Check for**

* Instances with long name/kind, regular and global messages (horizontal only)

**Further steps**

* Open file `examples/msc/manual_tests/checklist.sample2.msc`

**Check for**

* Regular horizontal messages, dynamically created/stoped instances

**Further steps**

* Open file `examples/msc/manual_tests/checklist.sample2.cif.msc`

**Check for**

* Same result as the file before. But geometry is stored as CIF information.


**Further steps**

* Open file `examples/msc/manual_tests/checklist.sample3.msc`

**Check for**

* Same result as checklist.sample2, But the order of instances is changed (No CIF).

**TODO:** add shared condition, coregion, global messages, more different timers, comments; add these files into the repo.


# 2 Message autoconnection

* Create new doc
* Add instance1
* Add instance2
* Move instance2 manually to the right
* Create new message from the top of instance1 to the Chart's bottom right corner
* Save file
* Load file

**Check for**

* Geometry restored completely

**Further Steps**

* Add new instance

**Check for**

* There are 3 instances, message is still global, points in direction "top of instance1, bottom right of the Chart".


# 3 Default size - 2 instances with CIF, chart with CIF

Same as in 2, but before saving:

* Resize Chart manually to provide some CIF for it

**Expected result:** content area restored completely (well, except the "one pixel shift" issue).


# 4 Geometry restored

**Steps**

* Create a doc with 2 instances
* Provide CIF for all items, including the Chart (resize chart, move instance1 a bit to the right, move instance2 close to the right edge of Chart)
* Save file
* Load file

**Check for**

* Geometry restored completely

**Further steps**

* Create new instance

**Check for**

* Chart automatically resized to contain all 3 instances.


# 5 Quick message connection

**Steps**

* Create a doc
* Create two instances
* Press Ctrl and mouse press on one instance
* Move the pressed mouse to the other instance and release the mouse button

**Check for**

* A message is created connection the two instances
