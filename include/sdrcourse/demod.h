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


#ifndef INCLUDED_SDRCOURSE_DEMOD_H
#define INCLUDED_SDRCOURSE_DEMOD_H

#include <sdrcourse/api.h>
#include <gnuradio/block.h>
#include <sdrcourse/symbol_mapper.h>


namespace gr {
  namespace sdrcourse {

    
    class SDRCOURSE_API demod : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<demod> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sdrcourse::demod.
       *
       * To avoid accidental use of raw pointers, sdrcourse::demod's
       * constructor is in a private implementation
       * class. sdrcourse::demod::make is the public interface for
       * creating new instances.
       */
      static sptr make(modtype_t modtype, std::vector<int> symbol_values);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_DEMOD_H */

