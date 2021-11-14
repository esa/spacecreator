 sedsconverter --from SEDS --to ASN.1 --skip-validation -i resources/test_led_demo.xml \
  --asn1-filepath-prefix output/ --acn-filepath-prefix output/
sedsconverter --from SEDS --to InterfaceView --skip-validation -i resources/test_led_demo.xml \
  --out output/led_demo.xml --iv-config config.xml
sedsconverter --from SEDS --to SDL --skip-validation -i resources/test_led_demo.xml \
  --sdl-filepath-prefix output/ --iv-config config.xml
cp resources/test_led_demo.system_structure output/system_structure.pr
cp resources/test_led_demo.dataviewuniq output/dataview-uniq.asn
cd output
opengeode --toC system_structure.pr LedDemo.pr
asn1scc -c --type-prefix asn1Scc dataview-uniq.asn
gcc -c LedDemo.c
