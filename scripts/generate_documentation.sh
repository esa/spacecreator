#!/bin/sh

cd `git rev-parse --show-toplevel`
cd doc
doxygen Doxygen

cd ../doc-tests
doxygen Doxygen
