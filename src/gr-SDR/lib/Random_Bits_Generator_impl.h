/* -*- c++ -*- */
/* 
 * Copyright 2020 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_SDR_RANDOM_BITS_GENERATOR_IMPL_H
#define INCLUDED_SDR_RANDOM_BITS_GENERATOR_IMPL_H

#include <SDR/Random_Bits_Generator.h>


namespace gr {
  namespace SDR {

    class Random_Bits_Generator_impl : public Random_Bits_Generator
    {
     private:
      // Nothing to declare in this block.
      int d_length;
      float d_ratio;

     public:
      Random_Bits_Generator_impl(int length, float ratio);
      bool start();
      void generate_bits();
      void generate_bitsvec(pmt::pmt_t msg) { generate_bits(); };
      void generate_bitsvec() { generate_bits(); };

    };

  } // namespace SDR
} // namespace gr

#endif /* INCLUDED_SDR_RANDOM_BITS_GENERATOR_IMPL_H */

