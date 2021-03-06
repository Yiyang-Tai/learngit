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
#include "random_sourcecd_impl.h"

namespace gr {
  namespace sdrcourse {

    random_sourcecd::sptr
    random_sourcecd::make(int payload_length, float bits_ratio)
    {
      return gnuradio::get_initial_sptr
        (new random_sourcecd_impl(payload_length, bits_ratio));
    }

    /*
     * The private constructor
     */
    random_sourcecd_impl::random_sourcecd_impl(int payload_length, float bits_ratio)
      : gr::block("random_sourcecd",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	d_payload_length(payload_length),
	d_bits_ratio(bits_ratio)
	
    {
	d_generating = false;

	message_port_register_in(pmt::mp("IN"));
	set_msg_handler(pmt::mp("IN"),
		boost::bind(&random_sourcecd_impl::generate_bitsvec, this, _1));

	message_port_register_out(pmt::mp("OUT"));
	if (d_payload_length < 1)
           throw std::runtime_error("payload length must be >= 1");
	if (d_bits_ratio < 0 || d_bits_ratio > 1 )
           throw std::runtime_error("bits ratio must be >= 0 & <=1");
    }

    /*
     * Our virtual destructor.
     */
    random_sourcecd_impl::~random_sourcecd_impl()
    {
    }

    void
    random_sourcecd_impl::generate_bits()
    {
	d_generating = true;
    	int len = d_payload_length;
	int random;
	double val = (double)rand() / RAND_MAX;
	std::vector<unsigned char> vec(len);

	for (int i = 0; i < len; i++)
	{
            if (val < d_bits_ratio)
	    	vec[i] = 0;
	    else
		vec[i] = 1;
	}
	message_port_pub(pmt::mp("OUT"), pmt::cons(pmt::make_dict(), pmt::init_u8vector(vec.size(), vec)));
	
    }

  } /* namespace sdrcourse */
} /* namespace gr */

