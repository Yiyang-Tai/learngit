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
#include "mix_impl.h"

namespace gr {
  namespace sdrcourse {

    mix::sptr
    mix::make()
    {
      return gnuradio::get_initial_sptr
        (new mix_impl());
    }

    /*
     * The private constructor
     */
    mix_impl::mix_impl()
      : gr::block("mix",
               gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {message_port_register_in(pmt::mp("IN1"));
     message_port_register_in(pmt::mp("IN2"));
     set_msg_handler(pmt::mp("IN2"), boost::bind(&mix_impl::save, this, _1 ));
     set_msg_handler(pmt::mp("IN2"), boost::bind(&mix_impl::save, this, _1 ));
     message_port_register_out(pmt::mp("OUT"));
	}

    /*
     * Our virtual destructor.
     */
    mix_impl::~mix_impl()
    {
    }

    void
    mix_impl::mix(pmt::pmt_t data1_in)
    {
     if(pmt::is_pair(data1_in))
     {
       pmt::pmt_t meta = pmt::car(data1_in);
       pmt::pmt_t header = pmt::cdr(data1_in);
       std::vector<unsigned char> data_header = pmt::u8vector_elements(header);
       data_header.insert(data_header.end(), data2_payload.begin(), data2_payload.end());
	size_t lens = data_header.size();
	pmt:: pmt_t out = pmt::init_u8vector(lens, data_header);
	message_port_pub(pmt::mp("OUT"), pmt::cons(meta, out));
       

	}
    }

    void
    mix_impl::save(pmt::pmt_t data2_in)
    {data2_payload.clear();
     if(pmt::is_pair(data2_in))
     {
       pmt::pmt_t meta = pmt::car(data2_in);
       pmt::pmt_t payload = pmt::cdr(data2_in);
       data2_payload = pmt::u8vector_elements(payload);
      }
    }
  } /* namespace sdrcourse */
} /* namespace gr */

