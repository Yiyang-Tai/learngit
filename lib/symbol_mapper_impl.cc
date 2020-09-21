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
#include "symbol_mapper_impl.h"
#include <gnuradio/math.h>

#define SQRT_TWO 0.707107


float angle = M_PI / 8.0;
const float level = sqrt(float(0.1));

const gr_complex sym_bpsk[] = {gr_complex(1,0), gr_complex(-1,0)};
const gr_complex sym_bpsk_off[] = {gr_complex(0,1), gr_complex(0,-1)};
const gr_complex sym_qpsk[] = {gr_complex(-SQRT_TWO, -SQRT_TWO), gr_complex(SQRT_TWO, -SQRT_TWO), gr_complex(-SQRT_TWO, SQRT_TWO), gr_complex(SQRT_TWO, SQRT_TWO)};

const gr_complex sym_8psk[] = {gr_complex(cos(1 * angle), sin(1 * angle)), gr_complex(cos(7 * angle), sin(7 * angle)), gr_complex(cos(9 * angle), sin(9 * angle)), gr_complex(cos(15 * angle), sin(15 * angle)), gr_complex(cos(11 * angle), sin(11 * angle)), gr_complex(cos(13 * angle), sin(13 * angle)), gr_complex(cos(3 * angle), sin(3 * angle)), gr_complex(cos(5 * angle), sin(5 * angle))};
const gr_complex sym_pam4[] = {gr_complex(-3,0), gr_complex(-1,0), gr_complex(3,0), gr_complex(1,0)};

const gr_complex sym_qam16[]= {gr_complex(3 * level, -3 * level), gr_complex(-1 * level, -3 * level), gr_complex(3 * level, 1 * level), gr_complex(-1 * level, 1 * level),
        gr_complex(-3 * level, -3 * level), gr_complex(1 * level, -3 * level), gr_complex(-3 * level, 1 * level), gr_complex(1 * level, 1 * level),
        gr_complex(-1 * level, -1 * level), gr_complex(3 * level, -1 * level), gr_complex(-1 * level, 3 * level), gr_complex(3 * level, 3 * level),
        gr_complex(1 * level, -1 * level), gr_complex(-3 * level, -1 * level), gr_complex(1 * level, 3 * level), gr_complex(-3 * level, 3 * level)};


namespace gr {
  namespace sdrcourse {

    symbol_mapper::sptr
    symbol_mapper::make(modtype_t modtype, std::vector<int> symbol_values)
    {
      return gnuradio::get_initial_sptr
        (new symbol_mapper_impl(modtype, symbol_values));
    }

    /*
     * The private constructor
     */
    symbol_mapper_impl::symbol_mapper_impl(modtype_t modtype, std::vector<int> symbol_values)
      : gr::block("symbol_mapper",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
	 d_modtype(modtype),
	 d_symbol_values(symbol_values)

    {
	message_port_register_in(pmt::mp("IN"));
        message_port_register_out(pmt::mp("OUT"));
        set_msg_handler(pmt::mp("IN"), 		 
		boost::bind(&symbol_mapper_impl::handle_map, this, _1));

	switch(d_modtype){
		case BPSK:
		d_wmaps = std::vector<gr_complex>(&sym_bpsk[0], &sym_bpsk[2]);
		break;
		case P2BPSK:
		d_wmaps = std::vector<gr_complex>(&sym_bpsk_off[0], &sym_bpsk_off[2]);
		break;
                case QPSK:
		d_wmaps = std::vector<gr_complex>(&sym_qpsk[0], &sym_qpsk[4]);
		for(int i=0; i <4; i++){d_wmaps[i] *= gr_complex(M_SQRT1_2, M_SQRT1_2);}
                break;
                case PSK8:
		d_wmaps = std::vector<gr_complex>(&sym_8psk[0], &sym_8psk[8]);
                break;
                case PAM4:
		d_wmaps = std::vector<gr_complex>(&sym_pam4[0], &sym_pam4[4]);
                break;
                case QAM16:
		d_wmaps = std::vector<gr_complex>(&sym_qam16[0], &sym_qam16[16]);
                break;
			}
		uint16_t d_bps = log2(d_wmaps.size());
		

		
                   float avgp = 0;
                   for(int i=0; i<d_wmaps.size(); i++){ 
			avgp = std::sqrt((d_wmaps[i] * std::conj(d_wmaps[i])).real());
                        d_wmaps[i] = d_wmaps[i] / avgp;}
                 
		if(d_symbol_values.size() != d_wmaps.size()){
                     throw std::runtime_error("incorrect number of symbols for given constellation");
                  }

		for(int i=0; i<d_symbol_values.size(); i++){
                  if((d_symbol_values[i] < 0) || (d_symbol_values[i] >= d_wmaps.size())){
                     throw std::runtime_error((boost::format("bad symbol value recieved (%d) values must be in range [0,%d] for given constellation")%(d_symbol_values[i])%(d_wmaps.size()-1)).str());}
			  }
}
    

       void symbol_mapper_impl::map(const uint8_t *in, gr_complex* out, int nsymbols)
       {   

	std::vector<gr_complex> d_mappings (d_wmaps.size());    
	for(int j=0; j<d_wmaps.size(); j++)
	{
	int idx = d_symbol_values[j];
	gr_complex sym = d_wmaps[idx];
	d_mappings[j] = sym;
	//printf(" %d --> (%f,%f) \n", j, sym.real(), sym.imag() );
	}
        for(int i=0; i<nsymbols; i++)
       {
            uint16_t idx(0);
            for(int j=0; j<d_bps; j++)
	     {
                // sanity check - hopefully this does not slow things down too much
                if(__builtin_expect( in[i*d_bps + j] > 0x01, false))
		{
                    throw std::runtime_error((boost::format("ERROR: constellation mapper expects values of 0x00 and 0x01 in *ONLY* - received 0x%02x at bit offset %d!")% ((int)(in[i*d_bps+j]))%(d_bps*i)).str());
                }
                idx |= (in[i*d_bps + j] << (d_bps - j-1));
              }
            out[i] = d_mappings[idx];           
         }
       }
	

    /*
     * Our virtual destructor.
     */
    symbol_mapper_impl::~symbol_mapper_impl()
    {
    }

    void
    symbol_mapper_impl::handle_map(pmt::pmt_t msg)
    {
	pmt::pmt_t meta = pmt::car(msg);
        pmt::pmt_t bit_data = pmt::cdr(msg);
	size_t offset = 0;

	int len_bits = pmt::length(bit_data);
	while( len_bits % d_bps != 0){
            std::vector<uint8_t> nv = pmt::u8vector_elements(bit_data);
            nv.push_back(0);
            len_bits++;
            bit_data = pmt::init_u8vector(len_bits, nv);
            }
	int len_symbols = len_bits / d_bps;
	pmt::pmt_t cpdu_vector(pmt::make_c32vector(len_symbols, gr_complex(0,0)));
	map(pmt::u8vector_elements(bit_data, offset), 
	    pmt::c32vector_writable_elements(cpdu_vector,offset), 
	    len_symbols);

	message_port_pub(pmt::mp("OUT"), pmt::cons(meta, cpdu_vector));
     }

    

  } /* namespace sdrcourse */
} /* namespace gr */

