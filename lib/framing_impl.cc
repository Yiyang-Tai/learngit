/* -*- c++ -*- */
/* 
 * Copyright 2020 n.
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
#include "framing_impl.h"
#include <gnuradio/digital/crc32.h> 
#include <volk/volk.h>
#include <boost/crc.hpp>

namespace gr {
  namespace sdrcourse {

    framing::sptr
    framing::make(int destination_address, int source_address, int frame_type, int frame_index, int reserved_field_I,  int reserved_field_II, int modulation, int len_payload_length)
    {
      return gnuradio::get_initial_sptr
        (new framing_impl(destination_address, source_address, frame_type, frame_index, reserved_field_I, reserved_field_II, modulation, len_payload_length));
    }

    /*
     * The private constructor
     */
    framing_impl::framing_impl(int destination_address, int source_address, int frame_type, int frame_index, int reserved_field_I,  int reserved_field_II, int modulation, int len_payload_length)
      : gr::block("framing",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	d_frame_type(frame_type),
	d_frame_index(frame_index),
	d_destination_address(destination_address),
	d_source_address(source_address),
	d_reserved_field_I(reserved_field_I),
	d_reserved_field_II(reserved_field_II),
	d_len_payload_length(len_payload_length), // Bytes
	d_modulation(modulation) 
    {
	message_port_register_in(pmt::mp("IN"));
        set_msg_handler(pmt::mp("IN"), boost::bind(&framing_impl::catagorization, this, _1 ));
	message_port_register_out(pmt::mp("OUT"));
	}

    /*
     * Our virtual destructor.
     */
    framing_impl::~framing_impl()
    {
    }
    void
    framing_impl::catagorization(pmt::pmt_t data_in)
    {
      pmt::pmt_t generated_frame;
      if(pmt::is_dict(data_in))
      {
        if(d_frame_type == 1)
        {
          generated_frame = data_frame_formation(data_in);
        }
        else if(d_frame_type == 2)
        {
          generated_frame = ack_frame_formation(data_in);
        }
        if(pmt::dict_has_key(generated_frame, pmt::string_to_symbol("frame_pmt")))
        {
          message_port_pub(pmt::mp("OUT"), generated_frame);
        }
      }
      else if(pmt::is_integer(data_in) && d_frame_type == 1)
      {
        generated_frame = data_frame_formation(data_in);
        if(pmt::dict_has_key(generated_frame, pmt::string_to_symbol("frame_pmt")))
        {
          message_port_pub(pmt::mp("OUT"), generated_frame);
        }
      } 
    }

    pmt::pmt_t
    framing_impl::data_frame_formation(pmt::pmt_t rx_payload)
    {
      /*
       * Generate a data frame
       */
      pmt::pmt_t frame_info;
      pmt::pmt_t frame_after_crc;
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
          frame.insert(frame.end(), frame_header.begin(), frame_header.end());
          frame.insert(frame.end(), payload_array.begin(), payload_array.end());
     
          // crc
          pmt::pmt_t frame_before_crc_u8vector = pmt::init_u8vector(frame.size(), frame);
          pmt::pmt_t frame_before_crc = pmt::cons(meta, frame_before_crc_u8vector); 
          frame_after_crc = crc32_bb_calc(frame_before_crc);
          frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_pmt"), frame_after_crc);
          
        }
        else
          std::cout << "pmt is not a u8vector" << std::endl;
      }
      else 
        std::cout << "pmt is not a pair" << std::endl;
      return frame_info;
    }

    pmt::pmt_t
    framing_impl::ack_frame_formation(pmt::pmt_t rx_data)
    {
      /*
       * Generate an ack frame
       */
      pmt::pmt_t frame_info;
      pmt::pmt_t frame_after_crc;
      if(pmt::is_dict(rx_data))
      {
        pmt::pmt_t meta = pmt::make_dict();
        pmt::pmt_t not_found;
        // generate an ack frame
        int ack_address = pmt::to_long(pmt::dict_ref(rx_data, pmt::string_to_symbol("source_address"), not_found));
        int ack_index = pmt::to_long(pmt::dict_ref(rx_data, pmt::string_to_symbol("frame_index"), not_found));
        int ack_num_transmission = pmt::to_long(pmt::dict_ref(rx_data, pmt::string_to_symbol("num_transmission"), not_found));
        
	std::vector<unsigned char> frame_header;
        frame_info = frame_header_formation(&frame_header, 2, ack_index, ack_address, d_source_address, d_reserved_field_I, d_reserved_field_II, 0, ack_num_transmission, d_modulation);
        std::vector<unsigned char> frame;
        frame.insert(frame.end(), frame_header.begin(), frame_header.end());
        // crc
        // crc32_bb_calc(&frame);
        // change frame to pmt::pmt_t
        pmt::pmt_t frame_before_crc_u8vector = pmt::init_u8vector(frame.size(), frame);
        pmt::pmt_t frame_before_crc = pmt::cons(meta, frame_before_crc_u8vector); 
        pmt::pmt_t frame_after_crc = crc32_bb_calc(frame_before_crc);
        frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_pmt"), frame_after_crc);
      }
      else 
        std::cout << "Error: pmt is not a dict, cannot generate an ack frame. please check your connections." << std::endl;
      return frame_info;
    }


    pmt::pmt_t
    framing_impl::frame_header_formation(std::vector<unsigned char> *frame_header, int frame_type, int frame_index, int destination_address, int source_address, int reserved_field_I, int reserved_field_II, int payload_length, int num_transmission, int modulation)
    {
      std::vector< unsigned char > vec_frame_header;
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
      frame_header->insert(frame_header->end(), vec_transmission.begin(), vec_transmission.begin() + 1);
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
    framing_impl::intToByte(int i, std::vector<unsigned char> *bytes, int size)
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

    pmt::pmt_t
    framing_impl::crc32_bb_calc(pmt::pmt_t msg)
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

      pmt::pmt_t output = pmt::init_u8vector(pkt_len+4, bytes_out); // this copies the values from bytes_out into the u8vector
      return pmt::cons(meta, output);
    } 


  } /* namespace sdrcourse */
} /* namespace gr */

