#!/bin/sh

cd `git rev-parse --show-toplevel`

pushd doc > /dev/null
doxygen Doxygen
popd > /dev/null

pushd doc-tests > /dev/null
doxygen Doxygen
popd > /dev/null
