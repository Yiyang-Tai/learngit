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
#include "demod_impl.h"
#include <pmt/pmt.h>

namespace gr {
  namespace sdrcourse {

    demod::sptr
    demod::make(modtype_t modtype, std::vector<int> symbol_values)
    {
      return gnuradio::get_initial_sptr
        (new demod_impl(modtype, symbol_values));
    }

    /*
     * The private constructor
     */
    demod_impl::demod_impl(modtype_t modtype, std::vector<int> symbol_values)
      : gr::block("demod",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	d_modtype(modtype),
	d_symbol_values(symbol_values)
    {
	message_port_register_in(pmt::mp("IN"));
        message_port_register_out(pmt::mp("OUT"));
        set_msg_handler(pmt::mp("OUT"), boost::bind(&demod_impl::handle_pdu, this, _1) );
	}

    /*
     * Our virtual destructor.
     */
    demod_impl::~demod_impl()
    {
    }


 void demod_impl::handle_pdu(pmt::pmt_t pdu){
        pmt::pmt_t meta = pmt::car(pdu);
        pmt::pmt_t sym_data = pmt::cdr(pdu);

        // start with default modulation
        modtype_t modtype = d_modtype;

        // check for dynamic modtype in dictionary
        if(!pmt::eqv(meta, pmt::PMT_NIL)){
            pmt::pmt_t dynamic_modtype = pmt::dict_ref(meta, pmt::mp("modulation"), pmt::PMT_NIL);
            if(!pmt::eqv(dynamic_modtype, pmt::PMT_NIL)){
                int mt = pmt::to_long(dynamic_modtype);
                modtype = (modtype_t) mt;
            }
        }

        // check/compute parameters
        int bps = d_symbol_values.size();
        int len_syms = pmt::length(sym_data);
        int len_bits = len_syms * bps;
        size_t offset = 0;

        // perform the mapping into a new f32 vector
        pmt::pmt_t fpdu_vector(pmt::make_f32vector( len_bits,  0.0f ) );
	const gr_complex* data = (const gr_complex*)pmt::c32vector_elements(sym_data, offset);
	std::vector<unsigned char> data_demod;

	for(int i =0; i< len_syms; i++){
	  unsigned int data_point = decision_maker(&data[i], modtype);
	  data_demod.push_back(data_point);
	}
	

        // send out the cpdu
        message_port_pub(pmt::mp("fpdus"), pmt::cons(meta, pmt::init_u8vector(data_demod.size(), data_demod)));
    }



unsigned int demod_impl::decision_maker(const gr_complex* sample, modtype_t modtype)
{
		switch(modtype){
		case BPSK:
		  return (real(*sample) > 0);
		break;
		case P2BPSK:
		  return (imag(*sample) > 0);
		break;
                case QPSK:
		  return 2 * (imag(*sample) > 0) + (real(*sample) > 0);
                break;
                case PSK8:
		{unsigned int ret = 0;
    		float re = sample->real();
   		float im = sample->imag();
   		 if (fabsf(re) <= fabsf(im))
      		  ret = 4;
   		 else if (re <= 0)
      		  ret |= 1;
   		 else if (im <= 0)
     		  ret |= 2;
		 return ret;}
                 break;
    		case QAM16:
    		{unsigned int ret = 0;
    		const float level = sqrt(float(0.1));
    		float re = sample->real();
    		float im = sample->imag();

    if (im <= 0 && im >= -2 * level && re >= 0 && re <= 2 * level)
        ret = 0;
    else if (im <= 0 && im >= -2 * level && re <= 0 && re >= -2 * level)
        ret = 1;
    else if (im <= -2 * level && re >= 2 * level)
        ret = 2;
    else if (im <= -2 * level && re <= -2 * level)
        ret = 3;
    else if (im <= 0 && im >= -2 * level && re <= -2 * level)
        ret = 4;
    else if (im <= 0 && im >= -2 * level && re >= 2 * level)
        ret = 5;
    else if (im <= -2 * level && re <= 0 && re >= -2 * level)
        ret = 6;
    else if (im <= -2 * level && re >= 0 && re <= 2 * level)
        ret = 7;
    else if (im >= 2 * level && re <= -2 * level)
        ret = 8;
    else if (im >= 2 * level && re >= 2 * level)
        ret = 9;
    else if (im >= 0 && im <= 2 * level && re <= 0 && re >= -2 * level)
        ret = 10;
    else if (im >= 0 && im <= 2 * level && re >= 0 && re <= 2 * level)
        ret = 11;
    else if (im >= 2 * level && re >= 0 && re <= 2 * level)
        ret = 12;
    else if (im >= 2 * level && re <= 0 && re >= -2 * level)
        ret = 13;
    else if (im >= 0 && im <= 2 * level && re >= 2 * level)
        ret = 14;
    else if (im >= 0 && im <= 2 * level && re <= -2 * level)
        ret = 15;

    return ret;}
                break;
			}
}



unsigned int demod_impl::decision_maker_bpsk(const gr_complex* sample)
{
    return (real(*sample) > 0);
}

unsigned int demod_impl::decision_maker_bpsk_off(const gr_complex* sample)
{
    return (imag(*sample) > 0);
}

unsigned int demod_impl::decision_maker_qpsk(const gr_complex* sample)
{
    // Real component determines small bit.
    // Imag component determines big bit.
    return 2 * (imag(*sample) > 0) + (real(*sample) > 0);

    /*
      bool a = real(*sample) > 0;
      bool b = imag(*sample) > 0;
      if(a) {
      if(b)
      return 0x0;
      else
      return 0x1;
      }
      else {
      if(b)
      return 0x2;
      else
      return 0x3;
      }
    */
}



unsigned int demod_impl::decision_maker_8psk(const gr_complex* sample)
{
    unsigned int ret = 0;

    float re = sample->real();
    float im = sample->imag();

    if (fabsf(re) <= fabsf(im))
        ret = 4;
    if (re <= 0)
        ret |= 1;
    if (im <= 0)
        ret |= 2;

    return ret;
}


unsigned int demod_impl::decision_maker_16psk(const gr_complex* sample)
{
    unsigned int ret = 0;
    const float level = sqrt(float(0.1));
    float re = sample->real();
    float im = sample->imag();

    if (im <= 0 && im >= -2 * level && re >= 0 && re <= 2 * level)
        ret = 0;
    else if (im <= 0 && im >= -2 * level && re <= 0 && re >= -2 * level)
        ret = 1;
    else if (im <= -2 * level && re >= 2 * level)
        ret = 2;
    else if (im <= -2 * level && re <= -2 * level)
        ret = 3;
    else if (im <= 0 && im >= -2 * level && re <= -2 * level)
        ret = 4;
    else if (im <= 0 && im >= -2 * level && re >= 2 * level)
        ret = 5;
    else if (im <= -2 * level && re <= 0 && re >= -2 * level)
        ret = 6;
    else if (im <= -2 * level && re >= 0 && re <= 2 * level)
        ret = 7;
    else if (im >= 2 * level && re <= -2 * level)
        ret = 8;
    else if (im >= 2 * level && re >= 2 * level)
        ret = 9;
    else if (im >= 0 && im <= 2 * level && re <= 0 && re >= -2 * level)
        ret = 10;
    else if (im >= 0 && im <= 2 * level && re >= 0 && re <= 2 * level)
        ret = 11;
    else if (im >= 2 * level && re >= 0 && re <= 2 * level)
        ret = 12;
    else if (im >= 2 * level && re <= 0 && re >= -2 * level)
        ret = 13;
    else if (im >= 0 && im <= 2 * level && re >= 2 * level)
        ret = 14;
    else if (im >= 0 && im <= 2 * level && re <= -2 * level)
        ret = 15;

    return ret;
}

  } /* namespace sdrcourse */
} /* namespace gr */

