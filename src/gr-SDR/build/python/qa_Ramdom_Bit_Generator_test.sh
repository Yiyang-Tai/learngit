#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/rslab/src/gr-SDR/python
export PATH=/home/rslab/src/gr-SDR/build/python:$PATH
export LD_LIBRARY_PATH=/home/rslab/src/gr-SDR/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/rslab/src/gr-SDR/build/swig:$PYTHONPATH
/usr/bin/python2 /home/rslab/src/gr-SDR/python/qa_Ramdom_Bit_Generator.py 
