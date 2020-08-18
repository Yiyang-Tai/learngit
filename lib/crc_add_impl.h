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

#ifndef INCLUDED_SDRCOURSE_CRC_ADD_IMPL_H
#define INCLUDED_SDRCOURSE_CRC_ADD_IMPL_H

#include <sdrcourse/crc_add.h>
#include <gnuradio/digital/crc32.h>
#include <boost/crc.hpp>

namespace gr {
  namespace sdrcourse {

    class crc_add_impl : public crc_add
    {
     private:
      boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true> _crc_impl;
      pmt::pmt_t crc32_bb_calc(pmt::pmt_t msg);
      void Bytetobits(std::vector<unsigned char> *bits, uint8_t* bytes);
     public:
      crc_add_impl();
      ~crc_add_impl();
      
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_CRC_ADD_IMPL_H */

