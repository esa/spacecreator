all: compile-linux

clean:
	rm -rf obj ./xmi

edit:
	matlab -desktop -sd src

generate-code: ./xmi/Calculate_built

./xmi/Calculate_built: ./src/Calculate.slx
	$(warning ---------------------------------------------------------------------)
	$(warning NOTE: The following is a patch to generate source code automatically.)
	$(warning ---------------------------------------------------------------------)
	matlab -batch \
	    "cd src; \
	     run ./Simulink_DataView_asn.m; \
	     qgen_export_xmi ('Calculate.slx', '-o', '../xmi'); \
	     exit;"
	qgenc ./xmi/Calculate.xmi --gen-entrypoint --wrap-io --pre-process-xmi --incremental \
	    --no-misra --language c --output src
	touch $@

./src/Calculate%slx : ./src/.qgenhost_script.m.md5
	$(warning Edit the Simulink model(s) before the system build.)
	matlab -batch \
	    "cd src;  \
	     run qgenhost_script.m;  \
	     exit;"

./src/.qgenhost_script.m.md5: ./src/qgenhost_script.m
	test -f $@ || md5sum $< > $@
	md5sum -c $@ &> /dev/null || md5sum $< > $@


compile-linux: generate-code
	mkdir -p obj && cd obj && gcc -c ../src/*.c ../code/*.c ../wrappers/*.c

.PHONY: all clean edit generate-code compile-linux
