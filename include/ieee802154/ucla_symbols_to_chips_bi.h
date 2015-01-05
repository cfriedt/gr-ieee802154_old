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


#ifndef INCLUDED_IEEE802154_UCLA_SYMBOLS_TO_CHIPS_BI_H
#define INCLUDED_IEEE802154_UCLA_SYMBOLS_TO_CHIPS_BI_H

#include <ieee802154/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace ieee802154 {

    /*!
     * \brief <+description of block+>
     * \ingroup ieee802154
     *
     */
    class IEEE802154_API ucla_symbols_to_chips_bi : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<ucla_symbols_to_chips_bi> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ieee802154::ucla_symbols_to_chips_bi.
       *
       * To avoid accidental use of raw pointers, ieee802154::ucla_symbols_to_chips_bi's
       * constructor is in a private implementation
       * class. ieee802154::ucla_symbols_to_chips_bi::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_SYMBOLS_TO_CHIPS_BI_H */

