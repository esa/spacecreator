# Testing function with sequence type param

**1. Steps:**

* Open project `tests/manual/qgenc/projects/not-working/ia5stringparam/ia5stringparam.pro`
* Open the `interfaceview.xml` file
* Try to build the project

Results:

* Project build fails with error:

For type MyIA5String:
    IA5String and types that depend on them cannot be used as a parameter.
    Use OCTET STRINGs instead!
    (file dataview/dataview-uniq.asn, line 5)
