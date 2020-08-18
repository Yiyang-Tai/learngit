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

#ifndef INCLUDED_SDRCOURSE_MIX_IMPL_H
#define INCLUDED_SDRCOURSE_MIX_IMPL_H

#include <sdrcourse/mix.h>

namespace gr {
  namespace sdrcourse {

    class mix_impl : public mix
    {
     private:
      std::vector<unsigned char> data2_payload;

     public:
      mix_impl();
      ~mix_impl();
	void mix(pmt::pmt_t data1_in);
	void save(pmt::pmt_t data2_in);
	
      
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_MIX_IMPL_H */

