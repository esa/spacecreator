#!/bin/sh
set -e

if [ ! $1 ] ; then
    echo "Usage: './run_tests.sh <directory of the binary build>'"
    exit 1
fi

if [ $QTC_INSTALL ]; then
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTC_INSTALL/lib/qtcreator:$QTC_INSTALL/lib/qtcreator/plugins
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/qtcreator:/usr/lib/x86_64-linux-gnu/qtcreator/plugins

BUILD_DIR=$1

export QT_QPA_PLATFORM=offscreen
# Non-colliding alias that can be used outside of this script
export SPACECREATOR_BUILD_DIR=$BUILD_DIR

cd $BUILD_DIR
BASE_DIR=`pwd`

EXIT_CODE=0
TESTS=`find . -name "tst*_*" -type f -executable`
for TEST in $TESTS
do
    echo $TEST
    TEST_NAME=$(basename $TEST)
    TEST_DIR=$(dirname $TEST)
    ARGS="-o $BASE_DIR/$TEST_NAME-result.xml,junitxml"
    cd $BASE_DIR
    cd $TEST_DIR
    ./$TEST_NAME $ARGS
    LAST_EXIT=$?
    if [ $LAST_EXIT -ne 0 ] ; then
        EXIT_CODE=1
        # Run failing test again, to print the output in stdout as well
        ./$TEST_NAME
    fi
done

exit $EXIT_CODE
