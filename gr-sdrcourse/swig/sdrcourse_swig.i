/* -*- c++ -*- */

#define SDRCOURSE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sdrcourse_swig_doc.i"

%{
#include "sdrcourse/start_pending.h"
#include "sdrcourse/random_sourcecd.h"
#include "sdrcourse/preamble_insert.h"
#include "sdrcourse/symbol_mapper.h"
#include "sdrcourse/pulse_shaper.h"
%}

%include "sdrcourse/start_pending.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, start_pending);
%include "sdrcourse/random_sourcecd.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, random_sourcecd);
%include "sdrcourse/preamble_insert.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, preamble_insert);
%include "sdrcourse/symbol_mapper.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, symbol_mapper);

%include "sdrcourse/pulse_shaper.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, pulse_shaper);
