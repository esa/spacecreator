This folder contains the Space Creator grantlee templates used to convert Space Creator's XML verion of the Interface View into AADL

In particular this one generates a default Deployment View targetting a native Linux board

The templates can be used jointly with the aadlconverter tool bundled with Space Creator

The syntax is:

   $ aadlconverter  -o interfaceview.xml -t interfaceview.tmplt -x DeploymentView.aadl

Templates made by Maxime Perrotin / ESA
(c) 2020
