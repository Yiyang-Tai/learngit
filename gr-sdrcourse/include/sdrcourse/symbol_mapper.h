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


#ifndef INCLUDED_SDRCOURSE_SYMBOL_MAPPER_H
#define INCLUDED_SDRCOURSE_SYMBOL_MAPPER_H

#include <sdrcourse/api.h>
#include <gnuradio/block.h>


namespace gr {
  namespace sdrcourse {

        typedef enum {
        BPSK=0,
        P2BPSK=1,
        QPSK=2,
        PSK8=3,
        PAM4=4,
        QAM16=5,
        } modtype_t;

    class SDRCOURSE_API symbol_mapper : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<symbol_mapper> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sdrcourse::symbol_mapper.
       *
       * To avoid accidental use of raw pointers, sdrcourse::symbol_mapper's
       * constructor is in a private implementation
       * class. sdrcourse::symbol_mapper::make is the public interface for
       * creating new instances.
       */
      static sptr make(modtype_t modtype, std::vector<int> symbol_values);
    };

  } // namespace sdrcourse
} // namespace gr

#endif /* INCLUDED_SDRCOURSE_SYMBOL_MAPPER_H */

