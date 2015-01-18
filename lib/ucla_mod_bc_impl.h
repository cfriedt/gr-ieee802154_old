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

#ifndef INCLUDED_IEEE802154_UCLA_MOD_BC_IMPL_H
#define INCLUDED_IEEE802154_UCLA_MOD_BC_IMPL_H

#include <gnuradio/blocks/packed_to_unpacked_ii.h>
#include <gnuradio/digital/chunks_to_symbols_ic.h>

#include <ieee802154/ucla_symbols_to_chips_bi.h>
#include <ieee802154/ucla_mod_bc.h>
#include <ieee802154/ucla_qpsk_modulator_cc.h>
#include <ieee802154/ucla_delay_cc.h>

namespace gr {
  namespace ieee802154 {

    class ucla_mod_bc_impl : public ucla_mod_bc
    {
     private:
    	uint32_t d_spb; // samples per baud >= 2
    	gr::ieee802154::ucla_symbols_to_chips_bi::sptr d_symbols_to_chips;
    	gr::blocks::packed_to_unpacked_ii::sptr d_chips_to_symbols;
    	gr::digital::chunks_to_symbols_ic::sptr d_symbols_to_constellation;
    	gr::ieee802154::ucla_qpsk_modulator_cc::sptr d_pskmod;
    	gr::ieee802154::ucla_delay_cc::sptr d_delay;

     public:
      ucla_mod_bc_impl();
      ~ucla_mod_bc_impl();

      // Where all the action really happens
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_MOD_BC_IMPL_H */

