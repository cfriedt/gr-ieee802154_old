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


#ifndef INCLUDED_IEEE802154_UCLA_CC1K_CORRELATOR_CB_H
#define INCLUDED_IEEE802154_UCLA_CC1K_CORRELATOR_CB_H

#include <ieee802154/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ieee802154 {

    /*!
     * \brief <+description of block+>
     * \ingroup ieee802154
     *
     */
    class IEEE802154_API ucla_cc1k_correlator_cb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<ucla_cc1k_correlator_cb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ieee802154::ucla_cc1k_correlator_cb.
       *
       * To avoid accidental use of raw pointers, ieee802154::ucla_cc1k_correlator_cb's
       * constructor is in a private implementation
       * class. ieee802154::ucla_cc1k_correlator_cb::make is the public interface for
       * creating new instances.
       */
      static sptr make( int payload_bytesize,
              unsigned char sync_byte,
              unsigned char nsync_byte,
              unsigned char manchester );
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_CC1K_CORRELATOR_CB_H */

