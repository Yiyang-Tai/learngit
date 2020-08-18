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

#ifndef INCLUDED_SDRCOURSE_FRAME_SYNC_IMPL_H
#define INCLUDED_SDRCOURSE_FRAME_SYNC_IMPL_H

#include <sdrcourse/frame_sync.h>
#include <volk/volk.h>

namespace gr {
  namespace sdrcourse {

    class frame_sync_impl : public frame_sync
    {
     private:
	float d_threshold;
	std::vector<int> d_preamble;
	int _state;
	float _d_f;
        std::complex<float> _d_phi;
	int d_len_preamble;
	std::vector<gr_complex> _mod_preamble;

	static const int STATE_DETECT = 0;
        static const int STATE_PREAMBLE = 1;
        static const int STATE_PAYLOAD = 3;
        static const int STATE_PROCESS_PREAMBLE = 2;
        static const int STATE_SET_TRIGGER = 4;

	float calculate_fd(const gr_complex* z, const gr_complex* x, const gr_complex* c, int N, int L0);
        std::complex<double> calculate_R(int m, const gr_complex* z, int L0);
        float wrap_phase(float phi);
	void modulate_preamble(); 

	uint64_t rx_time_full_sec;
        double rx_time_frac_sec;

     public:
      frame_sync_impl(const std::vector<int> &preamble, float threshold);
      ~frame_sync_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_FRAME_SYNC_IMPL_H */

