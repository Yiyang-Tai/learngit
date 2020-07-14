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


#ifndef INCLUDED_SDRCOURSE_PULSE_SHAPER_H
#define INCLUDED_SDRCOURSE_PULSE_SHAPER_H

#include <sdrcourse/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace sdrcourse {

    /*!
     * \brief <+description of block+>
     * \ingroup sdrcourse
     *
     */
    class SDRCOURSE_API pulse_shaper : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<pulse_shaper> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sdrcourse::pulse_shaper.
       *
       * To avoid accidental use of raw pointers, sdrcourse::pulse_shaper's
       * constructor is in a private implementation
       * class. sdrcourse::pulse_shaper::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned interpolation, const std::vector<gr_complex> &taps);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_PULSE_SHAPER_H */

