/* -*- c++ -*- */
/* 
 * Copyright 2020 n.
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


#ifndef INCLUDED_SDRCOURSE_FRAMING_H
#define INCLUDED_SDRCOURSE_FRAMING_H

#include <sdrcourse/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace sdrcourse {

    /*!
     * \brief <+description of block+>
     * \ingroup sdrcourse
     *
     */
    class SDRCOURSE_API framing : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<framing> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sdrcourse::framing.
       *
       * To avoid accidental use of raw pointers, sdrcourse::framing's
       * constructor is in a private implementation
       * class. sdrcourse::framing::make is the public interface for
       * creating new instances.
       */
      static sptr make(int destination_address, int source_address, int frame_type, int frame_index, int reserved_field_I,  int reserved_field_II, int modulation, int len_payload_length);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_FRAMING_H */

