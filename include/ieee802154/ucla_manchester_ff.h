/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_IEEE802154_UCLA_MANCHESTER_FF_H
#define INCLUDED_IEEE802154_UCLA_MANCHESTER_FF_H

#include <ieee802154/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace ieee802154 {

    /*!
     * \brief <+description of block+>
     * \ingroup ieee802154
     *
     */
    class IEEE802154_API ucla_manchester_ff : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<ucla_manchester_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ieee802154::ucla_manchester_ff.
       *
       * To avoid accidental use of raw pointers, ieee802154::ucla_manchester_ff's
       * constructor is in a private implementation
       * class. ieee802154::ucla_manchester_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_MANCHESTER_FF_H */

