#!/bin/sh

cd `git rev-parse --show-toplevel`
cd doc
doxygen Doxygen
