#!/bin/sh

if [ ! $1 ] ; then
    echo "Usage: './run_tests.sh <directory of the binary build>'"
    exit 1
fi


BUILD_DIR=$1

export QT_QPA_PLATFORM=offscreen

cd $BUILD_DIR
BASE_DIR=`pwd`

EXIT_CODE=0
TESTS=`find . -name "tst_*" -type f -executable`
for TEST in $TESTS
do
    echo $TEST
    TEST_NAME=$(basename $TEST)
    TEST_DIR=$(dirname $TEST)
    ARGS="-xunitxml -o $BASE_DIR/$TEST_NAME-result.xml"
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

return $EXIT_CODE
