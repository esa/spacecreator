**Drag Connection item creation between 2 Function items**

1. Steps:

	- Holding `CTRL` modifier press left mouse button on 1st Function, drag and release within 2nd Function

Expected results:

	- Required interface on the 1st Function, Provided interface on 2nd Function and Connection item linked to mentioned RI/PI should be created


2. Steps:

	- Having Provided interface item on 1st Function create connection. Holding CTRL modifier press left mouse button on that interface item, drag and release within 2nd Function

Expected results:

	- Required interface on 2nd Function and Connection item linked to mentioned RI/PI should be created


3. Steps:

	- Having Required interface item on 1st Function create connection. Holding CTRL modifier press left mouse button on that interface item, drag and release within 2nd Function

Expected results:

	- Provided interface on 2nd Function and Connection item linked to mentioned RI/PI should be created


4. Steps:

	- Having Required interface item on 1st Function and Provided interface item on 2nd Function create connection. Holding CTRL modifier press left mouse button on that interface item, drag and release on interface item on 2nd Function

Expected results:

	- Connection item linked to mentioned RI/PI should be created


5. Steps:

	- Having 2 top level Function items create connection between child/nested Function items of both top Functions. Holding CTRL modifier press left mouse button on Nested Function of 1st Top Funtion, drag and release within Nested Function of 2nd Top Function

Expected results:
	- Diverse type on interfaces on sibling Function items with linked Connection item should be created, interfaces with the same type should be created on Parent-Child connections



**Multipoint Connection item creation between 2 Function items**

6. Steps:

	- Having Required and Provided interface items create connection.

Expected results:

	- Connection item linked to mentioned RI/PI should be created



**Layout**

7. Steps:

	- Having Connection item selected create new point. Holding CTRL modifier press left mouse button on any non-edge Grip Point, drag

Expected results:

	- Additional grip point in the chain in the button release position should be created


8. Steps:

	- Having Connection item selected remove point. Press left mouse button on any Grip Point, drag over another Grip Point in the current Connection item

Expected results:

	- Hovered Grip Point item should be removed simplifying Connection Item path.


9. Steps:

	- Having Connection item selected. Press left mouse button on any edge Grip Point, drag and release on empty space

Expected results:

	- Moved Grip Point with Interface item should be placed on the nearest parent Function side, Connection item path should be regenerated
