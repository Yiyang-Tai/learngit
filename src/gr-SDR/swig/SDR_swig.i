/* -*- c++ -*- */

#define SDR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "SDR_swig_doc.i"

%{
#include "SDR/Random_Bits_Generator.h"
%}

%include "SDR/Random_Bits_Generator.h"
GR_SWIG_BLOCK_MAGIC2(SDR, Random_Bits_Generator);
