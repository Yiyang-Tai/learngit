/* -*- c++ -*- */
/* 
 * Copyright 2020 gr-sdrcourse author.
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
#include "baseband_derotation_impl.h"

namespace gr {
  namespace sdrcourse {

    baseband_derotation::sptr
    baseband_derotation::make(float mu, std::vector<int> symbol_values)
    {
      return gnuradio::get_initial_sptr
        (new baseband_derotation_impl(mu, symbol_values));
    }

    /*
     * The private constructor
     */
    baseband_derotation_impl::baseband_derotation_impl(float mu, std::vector<int> symbol_values)
      : gr::sync_block("baseband_derotation",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
	d_error(0), d_mu(mu), d_symbol_values(symbol_values)
    {}

    /*
     * Our virtual destructor.
     */
    baseband_derotation_impl::~baseband_derotation_impl()
    {
    }

    int
    baseband_derotation_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
	std::vector<tag_t> phi_tags;
        get_tags_in_window(phi_tags, 0, 0, noutput_items, pmt::intern("phi"));
        for(int i = 0; i < noutput_items; i++) {

          for(int j = 0; j < phi_tags.size(); j++) {
             if(phi_tags[j].offset == (nitems_written(0) + i)) {
               float phi = pmt::to_float(phi_tags[j].value);
               d_error = 0;
             }
          }


          out[i] = in[i] * std::polar(1.0f, -1.0f * d_error);

          float arg = std::arg(out[i]);
          float error = 0.0f;

	if(d_symbol_values.size() == 2) {
            if(arg > M_PI / 2.0f || arg < -M_PI / 2.0f) {
                if(arg < 0) {
                  error = arg + M_PI;
                } else {
                  error = arg - M_PI;
                }
            } else {
              error = arg;
            }
          }

          if(d_symbol_values.size() == 4) {
            if(arg >= 0 && arg < (M_PI / 2.0f)) {
              error = arg - (M_PI / 4.0f);
            }
            if(arg >= (M_PI / 2.0f)) {
              error = arg - ((3.0f * M_PI) / 4.0f);
            }
            if(arg <= (-M_PI / 2.0f)) {
              error = ((3.0f * M_PI) / 4.0f) + arg;
            }
            if(arg < 0 && arg > (-M_PI/2.0f)) {
              error = (M_PI/4.0f) + arg;
            }
          }


          d_error = d_error + d_mu * error;
          //TODO wrap _error
        }
	

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace sdrcourse */
} /* namespace gr */

