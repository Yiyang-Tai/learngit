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

#ifndef INCLUDED_SDRCOURSE_RANDOM_SOURCECD_IMPL_H
#define INCLUDED_SDRCOURSE_RANDOM_SOURCECD_IMPL_H

#include <sdrcourse/random_sourcecd.h>

namespace gr {
  namespace sdrcourse {

    class random_sourcecd_impl : public random_sourcecd
    {
     private:
     	int d_payload_length;
	float d_bits_ratio;
	bool d_generating;
	

     public:
      random_sourcecd_impl(int payload_length, float bits_ratio);
      ~random_sourcecd_impl();
      
      void generate_bits();
      void generate_bitsvec(pmt::pmt_t msg) { generate_bits(); };
      void generate_bitsvec() { generate_bits(); };

      
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_RANDOM_SOURCECD_IMPL_H */

