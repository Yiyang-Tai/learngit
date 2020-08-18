/* -*- c++ -*- */
/* 
 * Copyright 2020 <+YOU OR YOUR COMPANY+>.
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
#include "Random_Bits_Generator_impl.h"



namespace gr {
  namespace SDR {

    Random_Bits_Generator::sptr
    Random_Bits_Generator::make(int length, float ratio)
    {
      return gnuradio::get_initial_sptr
        (new Random_Bits_Generator_impl(length, ratio));
    }

    /*
     * The private constructor
     */
    Random_Bits_Generator_impl::Random_Bits_Generator_impl(int length, float ratio)
      : gr::block("Random_Bits_Generator",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	d_length(length),
	d_ratio(ratio)
    {
        message_port_register_in(pmt::mp("generate"));
	set_msg_handler(pmt::mp("generate"),
			boost::bind(&Random_Bits_Generator_impl::generate_bitsvec, this, _1));

	message_port_register_out(pmt::mp("out"));
	if (length < 1)
           throw std::runtime_error("length must be >= 1");
	if (ratio < 0 || ratio > 1 )
           throw std::runtime_error("length_module must be >= 0");
     }


    bool Random_Bits_Generator_impl::start()
    {
	generate_bits();
	return true;
    }


    void
    Random_Bits_Generator_impl::generate_bits()
    {
    	int len = d_length;
	int random;
	double val = (double)rand() / RAND_MAX;
	std::vector<unsigned char> vec(len);

	for (int i = 0; i < len; i++)
	{
            if (val < d_ratio)
	    	vec[i] = 0;
	    else
		vec[i] = 1;
	}
	pmt::pmt_t vecpmt(pmt::make_blob(&vec[0], len));
        pmt::pmt_t bitsvec(pmt::cons(pmt::PMT_NIL, vecpmt));

        message_port_pub(pmt::mp("out"), bitsvec);
	
    }

  } /* namespace SDR */
} /* namespace gr */

