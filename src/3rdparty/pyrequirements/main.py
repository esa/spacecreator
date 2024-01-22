# This Python file uses the following encoding: utf-8
import TasteQtWidgets
import sys
from PySide6.QtWidgets import QApplication, QWidget, QHBoxLayout

app = QApplication(sys.argv)

def main():
    man = TasteQtWidgets.requirement.RequirementsManager()
    mod = TasteQtWidgets.requirement.RequirementsModelBase()
    reqWid = TasteQtWidgets.requirement.RequirementsWidget("", man,mod)
    widget = QWidget()
    layout = QHBoxLayout()
    widget.setLayout(layout)
    layout.addWidget(reqWid)
    widget.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
