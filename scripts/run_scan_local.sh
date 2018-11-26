#!/bin/sh

BuildDir="../clang"
ReportDir="../clang-analyze"

if [ ! -d $BuildDir ] ; then
	mkdir $BuildDir
fi

cd $BuildDir
qmake ..
scan-build --status-bugs -o $ReportDir make -j8

