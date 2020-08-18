/* -*- c++ -*- */
/* 
 * Copyright 2020 gnuradio.org.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "symbol_sync_max_impl.h"
#include <math.h>
#include <numeric>

namespace gr {
  namespace sdrcourse {

    symbol_sync_max::sptr
    symbol_sync_max::make(float sps)
    {
      return gnuradio::get_initial_sptr
        (new symbol_sync_max_impl(sps));
    }

    /*
     * The private constructor
     */
    symbol_sync_max_impl::symbol_sync_max_impl(float sps)
      : gr::block("symbol_sync_max",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
	d_sps(sps)
    {}

    /*
     * Our virtual destructor.
     */
    symbol_sync_max_impl::~symbol_sync_max_impl()
    {
    }

    void
    symbol_sync_max_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    symbol_sync_max_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

	for (int i = 0; i<d_sps; i++)
	{
	  for (int a = i; a<ninput_items[0]; a++)
	  {
	    inbuffer.push_back(in[a]);
	  }
	  for (int j = 0; j < inbuffer.size(); j++)
	  { 
	    if (j % d_sps == 0){
	    inbuffer2[j / d_sps] = norm(inbuffer[j]);}
	   }
	   sum[i] = std::accumulate(inbuffer2.begin(), inbuffer2.end(), 0);	
	}
	
	maxindex = std::max_element(sum.begin(), sum.end()) - sum.begin();
      	for (int b=maxindex; b<ninput_items[0] ;b++)
	{
	  newin.push_back(in[b]);
	}
	
	for (int m = 0;m<newin.size(); m++){
	  if (m % d_sps == 0){
		out[m / d_sps] = newin[m];		
	    }
	}
	consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace sdrcourse */
} /* namespace gr */

