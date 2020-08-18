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

#ifndef INCLUDED_SDRCOURSE_PULSE_SHAPER_IMPL_H
#define INCLUDED_SDRCOURSE_PULSE_SHAPER_IMPL_H

#include <sdrcourse/pulse_shaper.h>

namespace gr {
  namespace sdrcourse {

    class pulse_shaper_impl : public pulse_shaper
    {
     private:
      unsigned int d_rate;
      std::vector<gr_complex> d_new_taps;
      std::vector<gr_complex> outbuffer;
      std::vector<gr_complex> outbuffer2;
      std::vector<gr_complex> inbuffer;

     public:
      pulse_shaper_impl(unsigned interpolation, const std::vector<gr_complex> &taps);
      ~pulse_shaper_impl();
      void set_taps(const std::vector<gr_complex> &taps);
      void convolution(std::vector<gr_complex> input1, std::vector<gr_complex> input2, std::vector<gr_complex> out);
      int min(int a, int b);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_PULSE_SHAPER_IMPL_H */

