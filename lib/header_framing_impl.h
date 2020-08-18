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

#ifndef INCLUDED_SDRCOURSE_HEADER_FRAMING_IMPL_H
#define INCLUDED_SDRCOURSE_HEADER_FRAMING_IMPL_H

#include <sdrcourse/header_framing.h>

namespace gr {
  namespace sdrcourse {

    class header_framing_impl : public header_framing
    {
     private:
        int d_frame_type;
	int d_frame_index;
	int d_destination_address;
	int d_source_address;
	int d_reserved_field_I;
	int d_reserved_field_II;
	int d_len_payload_length; // Bytes
	int d_modulation; 
	int d_payload_length;
	int d_len_num_transmission;
	void catagorization(pmt::pmt_t data_in);
	pmt::pmt_t data_frame_formation(pmt::pmt_t rx_payload);
	pmt::pmt_t ack_frame_formation(pmt::pmt_t rx_data);
	pmt::pmt_t frame_header_formation(std::vector<unsigned char> *frame_header, int frame_type, int frame_index, int destination_address, int source_address, int reserved_field_I, int reserved_field_II, int payload_length, int num_transmission, int modulation);
	void intToByte(int i, std::vector<unsigned char> *bytes, int size);
	void Bytetoint(int *a, std::vector<unsigned char> bytes, int size);
	void Bytetobits(std::vector<unsigned char> *bits, std::vector<unsigned char> bytes);
     public:
      header_framing_impl(int destination_address, int source_address, int frame_type, int frame_index, int reserved_field_I,  int reserved_field_II, int modulation, int len_payload_length, int len_num_transmission);
      ~header_framing_impl();

      
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_HEADER_FRAMING_IMPL_H */

