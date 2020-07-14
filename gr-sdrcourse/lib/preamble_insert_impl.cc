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
#include "preamble_insert_impl.h"

namespace gr {
  namespace sdrcourse {

    preamble_insert::sptr
    preamble_insert::make(const std::vector<unsigned char> &preamble)
    {
      return gnuradio::get_initial_sptr
        (new preamble_insert_impl(preamble));
    }

    /*
     * The private constructor
     */
    preamble_insert_impl::preamble_insert_impl(const std::vector<unsigned char> &preamble)
      : gr::block("preamble_insert",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	d_preamble(preamble)
    {
	message_port_register_in(pmt::mp("IN"));
	set_msg_handler(pmt::mp("IN"),
		boost::bind(&preamble_insert_impl::preamble_insert, this, _1));

	message_port_register_out(pmt::mp("OUT"));
     }

    /*
     * Our virtual destructor.
     */
    preamble_insert_impl::~preamble_insert_impl()
    {
    }

    int
    preamble_insert_impl::preamble_insert(pmt::pmt_t msg)
    { 
	pmt::pmt_t meta = pmt::car(msg);
	pmt::pmt_t vector = pmt::cdr(msg);
	std::vector<unsigned char> data = pmt::u8vector_elements(vector);
	
	data.insert(data.end(),d_preamble.begin(),d_preamble.end());

	size_t lens = data.size();
	pmt::pmt_t out = pmt::init_u8vector(lens, data);
 	
	message_port_pub(pmt::mp("OUT"), pmt::cons(meta, out));
	
     }


  } /* namespace sdrcourse */
} /* namespace gr */

