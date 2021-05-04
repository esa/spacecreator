# General

The tool `mscconverter` can be used to convert MSC file to some other format. The fiel to be converted is given with the `-m` parameter. The output file is set using the `-x` parameter. The grantlee template file used for the conversion is set using `-t`. If no template file is set, the default internal msc template is used.


# 1 Simple conversion

**Prerequisites**

Have a variable BUILDIR pointing to the build directory.
Have a variable SOURCEDIR pointing to the checked out repository.

**Steps**

Run the command:

`$BUILDIR/mscconverter -m $SOURCEDIR/examples/msc/test8.msc -t $SOURCEDIR/src/libs/msclibrary/mscresources/mscmodel.tmplt -x test8_conv.msc`

**Check for**

The result file `test8_conv.msc` is created. It has the same content as the original `test8.msc`, but no indentations.
