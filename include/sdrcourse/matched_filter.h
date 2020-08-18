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


#ifndef INCLUDED_SDRCOURSE_MATCHED_FILTER_H
#define INCLUDED_SDRCOURSE_MATCHED_FILTER_H

#include <sdrcourse/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace sdrcourse {

    /*!
     * \brief <+description of block+>
     * \ingroup sdrcourse
     *
     */
    class SDRCOURSE_API matched_filter : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<matched_filter> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sdrcourse::matched_filter.
       *
       * To avoid accidental use of raw pointers, sdrcourse::matched_filter's
       * constructor is in a private implementation
       * class. sdrcourse::matched_filter::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::vector<gr_complex> &taps);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_MATCHED_FILTER_H */

