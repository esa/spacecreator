# run this script to create a new spacecreator project from a provided template

#!/bin/bash

new_name=$1

cp -r template $new_name

cd $new_name

mv template.asn $new_name.asn
mv template.pro $new_name.pro
sed -i 's/template.pro/'$new_name'.pro/' Makefile
sed -i 's/template.asn/'$new_name'.asn/' $new_name'.pro'
sed -i 's/template.asn/'$new_name'.asn/' interfaceview.xml

