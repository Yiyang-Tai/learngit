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
#include "sdrcourse/matched_filter.h"
#include "sdrcourse/symbol_sync_max.h"
#include "sdrcourse/framing.h"
#include "sdrcourse/header_framing.h"
#include "sdrcourse/crc_add.h"
#include "sdrcourse/mix.h"
#include "sdrcourse/rotator.h"
#include "sdrcourse/baseband_derotation.h"
#include "sdrcourse/demod.h"
#include "sdrcourse/header_payload_demux.h"
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
%include "sdrcourse/matched_filter.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, matched_filter);

%include "sdrcourse/symbol_sync_max.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, symbol_sync_max);

%include "sdrcourse/framing.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, framing);
%include "sdrcourse/header_framing.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, header_framing);
%include "sdrcourse/crc_add.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, crc_add);


%include "sdrcourse/mix.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, mix);
%include "sdrcourse/rotator.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, rotator);
%include "sdrcourse/baseband_derotation.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, baseband_derotation);

%include "sdrcourse/demod.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, demod);
%include "sdrcourse/header_payload_demux.h"
GR_SWIG_BLOCK_MAGIC2(sdrcourse, header_payload_demux);
