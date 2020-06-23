#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/rslab/src/gr-tutorial/python
export PATH=/home/rslab/src/gr-tutorial/build/python:$PATH
export LD_LIBRARY_PATH=/home/rslab/src/gr-tutorial/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/rslab/src/gr-tutorial/build/swig:$PYTHONPATH
/usr/bin/python2 /home/rslab/src/gr-tutorial/python/qa_my_qpsk_demod_cb.py 
