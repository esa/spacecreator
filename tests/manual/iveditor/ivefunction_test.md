**File IO**

1. Steps:

	- Open file `examples/interfaceview/allitems.xml`

Expected results:

* connections shouldn't overlap with any Function-like items they are not connected to
* Function and FunctionType items should not cover other items and should be bound within parent function geometry


2. Steps:

    * Export file using interfaceviews.tmplt and save exported content
    * Open previously saved file

Expected results:

* all items should have the same position/geometry as in previous check step


**Layout**

3. Steps:

    * Move or resize any item (Function, FunctionType, Comment) and release when it overlaps another item

Expected results:

* Item should restore it previous position/geometry


4. Steps:

    * Move and resize any Function item and release when doesn't overlap another item

Expected results:
* Interfaces should be placed on the edges of changed item, without overlapping each other, connections (if any) should update layout without overlapping another items


5. Steps:

    * Create Function-like item within any Function item, move or resize it

Expected results:

* Affected item should keep its geometry within parent Function item, and restore its previous state in case of overlapping with other items



**UndoStack**

6. Steps:

    * Perform Undo action

Expected results:

* Affected Item, its interfaces with linked connections should restore theirs previous geometry


7. Steps:
    * Perform Redo action

Expected results:

* Affected item, its interfaces with linked connections should get new geometry
