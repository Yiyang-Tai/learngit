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

#ifndef INCLUDED_SDRCOURSE_SYMBOL_MAPPER_IMPL_H
#define INCLUDED_SDRCOURSE_SYMBOL_MAPPER_IMPL_H

#include <sdrcourse/symbol_mapper.h>
#include <gnuradio/block.h>

namespace gr {
  namespace sdrcourse {
	
    class symbol_mapper_impl : public symbol_mapper
    {
     private:
	 modtype_t d_modtype;
         std::vector<int> d_symbol_values;
         std::vector<gr_complex> d_wmaps;
     public:
      symbol_mapper_impl(modtype_t modtype, std::vector<int> symbol_values);
      ~symbol_mapper_impl();
	
        void map(const uint8_t *in, gr_complex* out, int nsymbols);
	void handle_map(pmt::pmt_t msg);
	uint16_t d_bps;
      // Where all the action really happens

    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_SYMBOL_MAPPER_IMPL_H */

