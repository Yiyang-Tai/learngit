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

 /*
     Generate payload where each element can be rewritten to an unsigned 8-bit integer array.
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
    	int len = d_payload_length*8;
	int random;
	double val = (double)rand() / RAND_MAX;
	std::vector<unsigned char> vec(len);
 /*
     * Generate a vector contains 0 and 1 with 8 times payload length.

 */
	for (int i = 0; i < len; i++)
	{
            if (val < d_bits_ratio)
	    	vec[i] = 0;
	    else
		vec[i] = 1;
	}
	std::vector<unsigned char> out;
 /*
     * convert every 8 bits to an integer and store to the output

 */
	for (int j = 0; j < len; j++)

	{ 
	  if (j%8 == 0){
	  std::vector<unsigned char> temp;
	  int num;
	  for (int k = 0; k < 8; k++)
	  {
            temp.insert(temp.end(), vec[j], vec[j]+8);
	   }
	  num = pow(2,temp[j])+pow(2,temp[j+1])+pow(2,temp[j+2])+pow(2,temp[j+3])+pow(2,temp[j+4])+pow(2,temp[j+5])+pow(2,temp[j+6])+pow(2,temp[j+7]);
	  out.push_back(num);
	  }
	else{continue;}
	}
	message_port_pub(pmt::mp("OUT"), pmt::cons(pmt::make_dict(), pmt::init_u8vector(out.size(), out)));
	
    }

  } /* namespace sdrcourse */
} /* namespace gr */

