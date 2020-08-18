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
#include "matched_filter_impl.h"

namespace gr {
  namespace sdrcourse {

    matched_filter::sptr
    matched_filter::make(const std::vector<gr_complex> &taps)
    {
      return gnuradio::get_initial_sptr
        (new matched_filter_impl(taps));
    }

    /*
     * The private constructor
     */
    matched_filter_impl::matched_filter_impl(const std::vector<gr_complex> &taps)
      : gr::sync_block("matched_filter",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
         d_new_taps(taps)   
 {
 }

    /*
     * Our virtual destructor.
     */
    matched_filter_impl::~matched_filter_impl()
    {
    }

    void 
    matched_filter_impl::convolution(std::vector<gr_complex> input1, std::vector<gr_complex> input2, std::vector<gr_complex> out)
    {
	int len1 = input1.size();
	int len2 = input2.size();
	gr_complex *buffer1 = new gr_complex[len1+len2-1];
	gr_complex *buffer2 = new gr_complex[len1+len2-1];

	for(int i = 0; i<len2; i++)
	   {
	     buffer2[i] = input2[i]; 
	    }
	for(int i = len2; i<len1+len2-1; i++)
	   {
	     buffer2[i] = 0;
	    }
	for(int i = 0; i<len1+len2-1; i++)
	   {
		buffer1[i] = 0;
		int temp = (min(i, len1)) == len1 ? len1-1 : min(i, len1);
		for(int j=0; j<=temp; j++)
		{
		   buffer1[i] += (input1[j]*buffer2[i-j]); 
		}
	    }
	for(int i = 0; i<len1+len2-1; i++)
		out[i] = buffer1[i];
     }

    int
    matched_filter_impl::min(int a, int b)
    {
	return a<b?a:b;
    }


    int
    matched_filter_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *)input_items[0];
      gr_complex *out = (gr_complex *)output_items[0];
      
      for(int i = 0; i< noutput_items; i++)
	{
	   inbuffer.push_back(in[i]);
	}
      convolution(inbuffer, d_new_taps, outbuffer);

	if( !outbuffer2.empty())
	{ int m;
	  for(m = 0;m<noutput_items; m++)
	  {
	   if(m < outbuffer2.size())
		{
		   out[m] = outbuffer2[m];
		}
	   else{
	    out[m] = outbuffer[m - outbuffer2.size()];
		}
	    }
	
	outbuffer2.erase(outbuffer2.begin(), outbuffer2.end());
	}
	else{int j ;
	  for(j =0;j<outbuffer.size(); j++)
	 {if (j<noutput_items)
	     {out[j] = outbuffer[j];}
	 else
	     {outbuffer2.push_back(outbuffer[j]);}
	  }
	}

	return noutput_items;
	 
    }

  } /* namespace sdrcourse */
} /* namespace gr */

