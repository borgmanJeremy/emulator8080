#!/bin/bash
set -e
cd build/test
ctest --output-on-failure
