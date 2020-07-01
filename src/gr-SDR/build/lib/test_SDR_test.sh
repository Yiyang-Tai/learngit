#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/rslab/src/gr-SDR/lib
export PATH=/home/rslab/src/gr-SDR/build/lib:$PATH
export LD_LIBRARY_PATH=/home/rslab/src/gr-SDR/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-SDR 
