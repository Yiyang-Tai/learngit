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

#ifndef INCLUDED_SDRCOURSE_DEMOD_IMPL_H
#define INCLUDED_SDRCOURSE_DEMOD_IMPL_H

#include <sdrcourse/demod.h>

namespace gr {
  namespace sdrcourse {

    class demod_impl : public demod
    {
     private:
       modtype_t d_modtype;
	std::vector<int> d_symbol_values;

     public:
      demod_impl(modtype_t modtype, std::vector<int> symbol_values);
      ~demod_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

	void handle_pdu(pmt::pmt_t pdu);

	unsigned int decision_maker(const gr_complex* sample, modtype_t modtype);
	unsigned int decision_maker_bpsk(const gr_complex* sample);
	unsigned int decision_maker_bpsk_off(const gr_complex* sample);
	unsigned int decision_maker_qpsk(const gr_complex* sample);
	unsigned int decision_maker_8psk(const gr_complex* sample);
	unsigned int decision_maker_16psk(const gr_complex* sample);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_DEMOD_IMPL_H */

