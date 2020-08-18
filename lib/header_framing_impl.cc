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
#include "header_framing_impl.h"

namespace gr {
  namespace sdrcourse {

    header_framing::sptr
    header_framing::make(int destination_address, int source_address, int frame_type, int frame_index, int reserved_field_I,  int reserved_field_II, int modulation, int len_payload_length, int len_num_transmission)
    {
      return gnuradio::get_initial_sptr
        (new header_framing_impl(destination_address, source_address, frame_type, frame_index, reserved_field_I, reserved_field_II, modulation, len_payload_length, len_num_transmission));
    }

    /*
     * The private constructor
     */
    header_framing_impl::header_framing_impl(int destination_address, int source_address, int frame_type, int frame_index, int reserved_field_I,  int reserved_field_II, int modulation, int len_payload_length, int len_num_transmission)
      : gr::block("header_framing",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	d_frame_type(frame_type),
	d_frame_index(frame_index),
	d_destination_address(destination_address),
	d_source_address(source_address),
	d_reserved_field_I(reserved_field_I),
	d_reserved_field_II(reserved_field_II),
	d_len_payload_length(len_payload_length), // Bytes
	d_len_num_transmission(len_num_transmission),
	d_modulation(modulation) 
    {
	message_port_register_in(pmt::mp("IN"));
        set_msg_handler(pmt::mp("IN"), boost::bind(&header_framing_impl::catagorization, this, _1 ));
	message_port_register_out(pmt::mp("OUT"));
	d_len_num_transmission = 1;
	}

    /*
     * Our virtual destructor.
     */
    header_framing_impl::~header_framing_impl()
    {
    }

    void
    header_framing_impl::catagorization(pmt::pmt_t data_in)
    {
      pmt::pmt_t generated_frame;
      if(pmt::is_pair(data_in))
      {
        if(d_frame_type == 1)
        {
          generated_frame = data_frame_formation(data_in);
        }
        else if(d_frame_type == 2)
        {
          generated_frame = ack_frame_formation(data_in);
        }
        
        message_port_pub(pmt::mp("OUT"), generated_frame);
      }
    }

    pmt::pmt_t
    header_framing_impl::data_frame_formation(pmt::pmt_t rx_payload)
    {
      /*
       * Generate a data frame
       */
      pmt::pmt_t frame_info;
      pmt::pmt_t frame_out;
      if(pmt::is_pair(rx_payload)) 
      {
        d_frame_type = 1;
        pmt::pmt_t payload_pmt;
        std::vector<unsigned char> payload_array; 
   
        pmt::pmt_t meta = pmt::car(rx_payload);
        payload_pmt = pmt::cdr(rx_payload);
                
        if(pmt::is_u8vector(payload_pmt))
        {
          payload_array = pmt::u8vector_elements(payload_pmt);
          d_payload_length = payload_array.size(); 
          std::vector<unsigned char> frame_header;        
          d_frame_index = pmt::to_long(rx_payload);
          frame_info = frame_header_formation(&frame_header, d_frame_type, d_frame_index, d_destination_address, d_source_address, d_reserved_field_I, d_reserved_field_II, d_payload_length, 1, d_modulation);
 	  std::vector<unsigned char> frame;
	  Bytetobits(&frame, frame_header);
          frame_out = pmt::cons(meta, pmt::init_u8vector(frame.size(), frame));
          
        }
        else
          std::cout << "pmt is not a u8vector" << std::endl;
      }
      else 
        std::cout << "pmt is not a pair" << std::endl;
      return frame_out;
    }

    pmt::pmt_t
    header_framing_impl::ack_frame_formation(pmt::pmt_t rx_data)
    {
      /*
       * Generate an ack frame
       */
      pmt::pmt_t frame_info;
      pmt::pmt_t frame_out;
      if(pmt::is_pair(rx_data))
      {
        pmt::pmt_t meta = pmt::car(rx_data);
        pmt::pmt_t frame_init = pmt::cdr(rx_data);
	std::vector<unsigned char> frame_array;
	frame_array = pmt::u8vector_elements(frame_init); 
        // generate an ack frame
	std::vector< unsigned char> ack_address_byte = {frame_array[4]};
	std::vector< unsigned char> ack_index_byte = {frame_array[2]};
	std::vector< unsigned char> ack_num_transmission_byte = {frame_array[5]};
        
	int ack_address; 
	Bytetoint(&ack_address, ack_address_byte, 1);
        int ack_index; 
	Bytetoint(&ack_index, ack_index_byte, 1);
        int ack_num_transmission; 
	Bytetoint(&ack_num_transmission, ack_num_transmission_byte, 1);
        
	std::vector<unsigned char> frame_header;
        frame_info = frame_header_formation(&frame_header, 2, ack_index, ack_address, d_source_address, d_reserved_field_I, d_reserved_field_II, 0, ack_num_transmission, d_modulation);
        std::vector<unsigned char> frame;
	Bytetobits(&frame, frame_header);
        frame_out = pmt::cons(meta, pmt::init_u8vector(frame.size(), frame));
      }
      else 
        std::cout << "Error: pmt is not a pair, cannot generate an ack frame. please check your connections." << std::endl;
      return frame_out;
    }


    pmt::pmt_t
    header_framing_impl::frame_header_formation(std::vector<unsigned char> *frame_header, int frame_type, int frame_index, int destination_address, int source_address, int reserved_field_I, int reserved_field_II, int payload_length, int num_transmission, int modulation)
    {
      std::vector< unsigned char > vec_frame_type;
      std::vector< unsigned char > vec_frame_index;
      std::vector< unsigned char > vec_destination_address;
      std::vector< unsigned char > vec_source_address;
      std::vector< unsigned char > vec_transmission;
      std::vector< unsigned char > vec_reserved_field_I;
      std::vector< unsigned char > vec_reserved_field_II;
      std::vector< unsigned char > vec_payload_length;
      std::vector< unsigned char > vec_modulation;
      /* 
        frame type (1 Bytes)
        frame index (1 Bytes)
        Destination address (1 Bytes)
        Source address (1 Bytes)
        Reserved field 1 (1 Bytes)
        Reserved field 2 (1 Bytes)
        Payload length (1 Bytes)
	modulation type (1Bytes)
       */
      // Frame type 
      intToByte(frame_type, &vec_frame_type, 1);
      // Frame index
      intToByte(frame_index, &vec_frame_index, 1);
      // Destination address
      intToByte(destination_address, &vec_destination_address, 1);
      // Payload length
      intToByte(payload_length, &vec_payload_length, 1);
      // Source address
      intToByte(source_address, &vec_source_address, 1);
      // num_transmission 
      intToByte(num_transmission, &vec_transmission, 1);
      // Reserved field I
      intToByte(reserved_field_I, &vec_reserved_field_I, 1);
      // Reserved field II
      intToByte(reserved_field_II, &vec_reserved_field_II, 1);
      // Reserved modulation 
      intToByte(modulation, &vec_modulation, 1);

      //std::cout  << "Frame header length before frame type: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_frame_type.begin(), vec_frame_type.begin() + 1);
      //std::cout  << "Frame header length after frame type: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_frame_index.begin(), vec_frame_index.begin() + 1);
      //std::cout  << "Frame header length after frame index: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_destination_address.begin(), vec_destination_address.begin() + 1);
      //std::cout  << "Frame header length after dest: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_source_address.begin(), vec_source_address.begin() + 1);
      //std::cout  << "Frame header length after src: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_transmission.begin(), vec_transmission.begin() + d_len_num_transmission);
      //std::cout  << "Frame header length after transmission: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_reserved_field_I.begin(), vec_reserved_field_I.begin() + 1);
      //std::cout  << "Frame header length after re1: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_reserved_field_II.begin(), vec_reserved_field_II.begin() + 1);
      //std::cout  << "Frame header length after re2: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_payload_length.begin(), vec_payload_length.begin() + 1);
      //std::cout  << "Frame header length after payload_length: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_modulation.begin(), vec_modulation.begin() + 1);
      //std::cout  << "Frame header length after payload_length: " << frame_header->size() << std::endl;

      pmt::pmt_t frame_info  = pmt::make_dict();
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_type"), pmt::from_long(frame_type));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_index"), pmt::from_long(frame_index));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("destination_address"), pmt::from_long(destination_address));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("source_address"), pmt::from_long(source_address));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("num_transmission"), pmt::from_long(1));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("reserved_field_I"), pmt::from_long(reserved_field_I));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("reserved_field_II"), pmt::from_long(reserved_field_II));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("payload_length"), pmt::from_long(payload_length));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("modulation_type"), pmt::from_long(modulation));

      return frame_info;
    }

    void 
    header_framing_impl::intToByte(int i, std::vector<unsigned char> *bytes, int size)
    {
//      std::cout << "Type is about to converted" << std::endl;
      bytes->insert(bytes->end(), (unsigned char) (0xff & i));
//      std::cout << "First byte is converted" << std::endl;
      if(size > 1)
      {
        bytes->insert(bytes->end(), (unsigned char) ((0xff00 & i) >> 8));
        if(size > 2)
        {
          bytes->insert(bytes->end(), (unsigned char) ((0xff0000 & i) >> 16));
          if(size > 3)
          {
            bytes->insert(bytes->end(), (unsigned char) ((0xff000000 & i) >> 24));
          }
        }
      }
    }

    void
    header_framing_impl::Bytetoint(int *a, std::vector<unsigned char> bytes, int size)
    {
	
	*a = (int) bytes[0];
	if(size > 1)
       {
        *a |= (bytes[1] << 8) ;
        if(size > 2)
        {
          *a |= (bytes[2] << 16);
          if(size > 3)
          {
            *a |= (bytes[3] << 24);
          }
        }
      }
    }

    void 
    header_framing_impl::Bytetobits(std::vector<unsigned char> *bits, std::vector<unsigned char> bytes)
    {
	int i;
	for (i=0; i<sizeof(bytes)*8; i++)
	{
	  bits->insert(bits->end(), (unsigned char) (((1<< (i%8)) & (bytes[i/8])) >> (i%8)));	
	}
     }
    
  } /* namespace sdrcourse */
} /* namespace gr */

