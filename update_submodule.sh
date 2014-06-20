#!/bin/bash
Path=`pwd`
cd  `dirname $0`
git submodule update --init --recursive
cd "${Path}"
