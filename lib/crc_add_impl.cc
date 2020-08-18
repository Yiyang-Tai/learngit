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
#include "crc_add_impl.h"
#include <gnuradio/digital/crc32.h> 
#include <volk/volk.h>
#include <boost/crc.hpp>

namespace gr {
  namespace sdrcourse {

    crc_add::sptr
    crc_add::make()
    {
      return gnuradio::get_initial_sptr
        (new crc_add_impl());
    }

    /*
     * The private constructor
     */
    crc_add_impl::crc_add_impl()
      : gr::block("crc_add",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
	message_port_register_in(pmt::mp("IN"));
        set_msg_handler(pmt::mp("IN"), boost::bind(&crc_add_impl::crc32_bb_calc, this, _1 ));
	message_port_register_out(pmt::mp("OUT"));
	
	}

    /*
     * Our virtual destructor.
     */
    crc_add_impl::~crc_add_impl()
    {
    }

    pmt::pmt_t
    crc_add_impl::crc32_bb_calc(pmt::pmt_t msg)
    {
      // extract input pdu
      pmt::pmt_t meta(pmt::car(msg));
      pmt::pmt_t bytes(pmt::cdr(msg));

      unsigned int crc;
      size_t pkt_len(0);
      const uint8_t* bytes_in = pmt::u8vector_elements(bytes, pkt_len);
      uint8_t* bytes_out = (uint8_t*)volk_malloc(4 + pkt_len*sizeof(uint8_t),
                                                 volk_get_alignment());

      _crc_impl.reset();
      _crc_impl.process_bytes(bytes_in, pkt_len);
      crc = _crc_impl();
      memcpy((void*)bytes_out, (const void*)bytes_in, pkt_len);
      memcpy((void*)(bytes_out + pkt_len), &crc, 4); // FIXME big-endian/little-endian, this might be wrong
     std::vector<unsigned char> bits_out;
     Bytetobits(&bits_out, bytes_out);
      pmt::pmt_t output = pmt::init_u8vector(bits_out.size(), bits_out); // this copies the values from bytes_out into the u8vector
      return pmt::cons(meta, output);
    } 


    void 
    crc_add_impl::Bytetobits(std::vector<unsigned char> *bits, uint8_t* bytes)
    {
	int i;
	for (i=0; i<sizeof(bytes)*8; i++)
	{
	  bits->insert(bits->end(), (unsigned char) (((1<< (i%8)) & (bytes[i/8])) >> (i%8)));	
	}
     }

  } /* namespace sdrcourse */
} /* namespace gr */

