# Generating skeleton for Simulink model

**0. Dependencies:**

* 1. MatLab Script (called `TASTEExporter`) to export Simulink model to .xml file is required to be installed in MatLab.
*    Without `TASTEExporter`, SimulinkImporterPlugin won't work

**1. Steps:**

* Open unmodified project `tests/manual/simulinkimporterplugin/demo_project/demo_project.pro` file
* Open `interfaceview.xml` file
* From main menu go to `Tools` -> `Simulink` -> and click `Import model from SLX`
* Choose `iface.slx` file from `tests/manual/simulinkimporterplugin/resources` directory
* Enter the name for imported function block (the default name is `simulink`)
* After a few seconds, a new block with defined interface (called `iface`) should appear in InterfaceView without any errors
* Connect `iface` interface from imported block to `controller` function block
* Save project
* Right click at any place in InterfaceView and choose "Generate/update code skeletons"
* Build project

Expected results:

* Project `demo_project` should compile correctly without any errors

**2. Resources:**

* If project contains file with data types definitions, it will be a `workspace.mat` file.
* There is one project `tests/manual/simulinkimporterplugin/resources/Aliases/InOutPortsDimension-1WorkspaceMFile`
*   that contains `workspace.m` file instead of `workspace.mat` to test handling .m files with data types definitions.

* List of working projects:
    1. "tests/manual/simulinkimporterplugin/resources/BasicTypes/InOutPortsDimension-1",
    
    2. "tests/manual/simulinkimporterplugin/resources/Aliases/InOutPortsDimension-1WorkspaceMFile",
    3. "tests/manual/simulinkimporterplugin/resources/Aliases/InOutPortsDimension-1",
    
    4. "tests/manual/simulinkimporterplugin/resources/AliasesToAliases/InOutPortsDimension-1",
    
    5. "tests/manual/simulinkimporterplugin/resources/BusMembersBasicTypes/InOutPortsDimension-1MemberDimension1",

* List of non working projects:
    1. "tests/manual/simulinkimporterplugin/resources/BasicTypes/InOutPortsDimension2",
    2. "tests/manual/simulinkimporterplugin/resources/BasicTypes/InOutPortsDimension[2 2]",

    3. "tests/manual/simulinkimporterplugin/resources/Aliases/InOutPortsDimension2",
    4. "tests/manual/simulinkimporterplugin/resources/Aliases/InOutPortsDimension[2 2]",

    5. "tests/manual/simulinkimporterplugin/resources/AliasesToAliases/InOutPortsDimension2",
    6. "tests/manual/simulinkimporterplugin/resources/AliasesToAliases/InOutPortsDimension[2 2]",

    7. "tests/manual/simulinkimporterplugin/resources/Enums/InOutPortsDimension-1",
    8. "tests/manual/simulinkimporterplugin/resources/Enums/InOutPortsDimension2",
    9. "tests/manual/simulinkimporterplugin/resources/Enums/InOutPortsDimension[2 2]",

    10. "tests/manual/simulinkimporterplugin/resources/BusMembersBasicTypes/InOutPortsDimension-1MemberDimension2",
    11. "tests/manual/simulinkimporterplugin/resources/BusMembersBasicTypes/InOutPortsDimension-1MemberDimension[2 2]",

    12. "tests/manual/simulinkimporterplugin/resources/BusMembersAliases/InOutPortsDimension-1MemberDimension1",
    13. "tests/manual/simulinkimporterplugin/resources/BusMembersAliases/InOutPortsDimension-1MemberDimension2",
    14. "tests/manual/simulinkimporterplugin/resources/BusMembersAliases/InOutPortsDimension-1MemberDimension[2 2]",

    15. "tests/manual/simulinkimporterplugin/resources/BusMembersEnums/InOutPortsDimension-1MemberDimension1",
    16. "tests/manual/simulinkimporterplugin/resources/BusMembersEnums/InOutPortsDimension-1MemberDimension2",
    17. "tests/manual/simulinkimporterplugin/resources/BusMembersEnums/InOutPortsDimension-1MemberDimension[2 2]",

    18. "tests/manual/simulinkimporterplugin/resources/BusMembersBasicTypesAliasesEnums/InOutPortsDimension-1MemberDimension1",
    19. "tests/manual/simulinkimporterplugin/resources/BusMembersBasicTypesAliasesEnums/InOutPortsDimension-1MemberDimension2",
    20. "tests/manual/simulinkimporterplugin/resources/BusMembersBasicTypesAliasesEnums/InOutPortsDimension-1MemberDimension[2 2]"
