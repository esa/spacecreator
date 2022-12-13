#!/bin/sh

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

FAILED_TESTS=" "

EXIT_CODE=0
TESTS=`find . -name "tst_tmc_subtyping" -type f -executable`
for TEST in $TESTS
do
    echo "Running test: $TEST"
    TEST_NAME=$(basename $TEST)
    TEST_DIR=$(dirname $TEST)
    ARGS="-o $BASE_DIR/$TEST_NAME-result.xml,junitxml"
    cd $BASE_DIR
    cd $TEST_DIR
    ./$TEST_NAME $ARGS
    LAST_EXIT=$?
    if [ $LAST_EXIT -ne 0 ] ; then
        EXIT_CODE=1
        FAILED_TESTS="$FAILED_TESTS,$TEST_NAME"
        # Run failing test again, to print the output in stdout as well
        echo "Test '$TEST_NAME' FAILED. Running it again for the std output."
        ./$TEST_NAME
    else
        echo "Test '$TEST_NAME' did pass."
    fi
done

if [ $EXIT_CODE -ne 0 ] ; then
    echo "List of failed tests: $FAILED_TESTS"
fi

echo "Testing finished"
echo "Tests exit code is: $EXIT_CODE"

exit $EXIT_CODE
