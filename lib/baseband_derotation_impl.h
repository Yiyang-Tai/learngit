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

#ifndef INCLUDED_SDRCOURSE_BASEBAND_DEROTATION_IMPL_H
#define INCLUDED_SDRCOURSE_BASEBAND_DEROTATION_IMPL_H

#include <sdrcourse/baseband_derotation.h>

namespace gr {
  namespace sdrcourse {

    class baseband_derotation_impl : public baseband_derotation
    {
     private:
      float d_mu;
      float d_error;
      std::vector<int> d_symbol_values;

     public:
      baseband_derotation_impl(float mu, std::vector<int> symbol_values);
      ~baseband_derotation_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_BASEBAND_DEROTATION_IMPL_H */

