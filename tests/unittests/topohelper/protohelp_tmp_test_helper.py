"""

A helper to visualize geometry during unit tests development.

$ wget https://bootstrap.pypa.io/get-pip.py
$ ./venv/bin/python get-pip.py
$ ./venv/bin/pip install pyside6

Then edit this file and run

$ ./venv/bin/python ./protohelp_tmp_test_helper.py
"""

import sys
from PySide6.QtWidgets import QApplication, QWidget
from PySide6.QtGui import QPainter, QColor, QPainterPath, QPolygonF
from PySide6.QtCore import Qt, QRectF, QLineF, QPointF


class Widget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.__path_input = QPainterPath()
        self.__path_expected = QPainterPath()
        self.__path_result = QPainterPath()


    def __createPathInput(self):
        path = QPainterPath()
        path.addRect(5, 5, 5, 5)
        path.moveTo(0, 0)
        path.lineTo(10, 10)
        path.moveTo(20, 20)
        path.lineTo(30, 30)

        return path

    def __createPathExpected(self):
        # points = [QPointF(0,70),QPointF(25,70), QPointF(25,34), QPointF(175,34), QPointF(175,70), QPointF(200,70)]
        points = [QPointF(0,0), QPointF(0,0), QPointF(10,10), QPointF(10,10), QPointF(20,20), QPointF(20,20), QPointF(30,30), QPointF(20,20)]

        path = QPainterPath()
        path.moveTo(points[0])
        path.addPolygon(QPolygonF(points))


        return path

    def __createPathResult(self):
        path = QPainterPath()
        # path.moveTo(QPointF(0,70))
        # for pnt in [QPointF(0,70), QPointF(25,70), QPointF(25,34), QPointF(166,34), QPointF(166,34), QPointF(183,34), QPointF(183,70), QPointF(200,70), QPointF(175,70)]:
        #     path.lineTo(pnt)
        return path

    def paintEvent(self, e):
        painter = QPainter(self)
        painter.fillRect(self.rect(), Qt.darkGray)

        pen = painter.pen()

        painter.save()
        pen.setWidthF(2)
        clr = QColor(Qt.red)
        pen.setColor(clr)
        painter.setPen(pen)

        self.__updatePathInput()
        painter.drawPath(self.__path_input)
        painter.restore()

        painter.save()
        pen.setWidthF(1.5)
        clr = QColor(Qt.green)
        clr.setAlphaF(0.8)
        pen.setColor(clr)
        painter.setPen(pen)

        self.__updatePathExpected()
        painter.drawPath(self.__path_expected)
        painter.restore()

        painter.save()
        pen.setWidthF(1)
        clr = QColor(Qt.blue)
        clr.setAlphaF(0.7)
        pen.setColor(clr)
        painter.setPen(pen)

        self.__updatePathResult()
        painter.drawPath(self.__path_result)
        painter.restore()

    def __updatePathInput(self):
        path = self.__createPathInput()
        if self.__path_input != path:
            self.__path_input = path

    def __updatePathExpected(self):
        path = self.__createPathExpected()
        if self.__path_expected != path:
            self.__path_expected = path

    def __updatePathResult(self):
        path = self.__createPathResult()
        if self.__path_result != path:
            self.__path_result = path



if __name__ == "__main__":
    app = QApplication([])
    window = Widget()
    window.show()
    sys.exit(app.exec())
