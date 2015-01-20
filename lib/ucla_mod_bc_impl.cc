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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <complex.h>

#include <gnuradio/io_signature.h>
#include <gnuradio/endianness.h>
#include "ucla_mod_bc_impl.h"

namespace gr {
  namespace ieee802154 {

    ucla_mod_bc::sptr
    ucla_mod_bc::make()
    {
      return gnuradio::get_initial_sptr
        (new ucla_mod_bc_impl());
    }

    /*
     * The private constructor
     */
    ucla_mod_bc_impl::ucla_mod_bc_impl()
      : gr::hier_block2("ucla_mod_bc",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_spb( 2 ),
        d_symbols_to_chips( gr::ieee802154::ucla_symbols_to_chips_bi::make() ),
        d_chips_to_symbols( gr::blocks::packed_to_unpacked_ii::make( 2, gr::GR_MSB_FIRST ) ),
		d_pskmod( gr::ieee802154::ucla_qpsk_modulator_cc::make() ),
		d_delay( gr::ieee802154::ucla_delay_cc::make( d_spb + 1 ) )
    {
#undef j
#define j _Complex_I
    	static const gr_complex d_constellation_arr[] = { -1-1j, -1+1j, 1-1j, 1+1j };
    	static const std::vector<gr_complex> d_constellation = std::vector<gr_complex>( d_constellation_arr, d_constellation_arr + sizeof( d_constellation_arr ) / sizeof( d_constellation_arr[0] ) );

        d_symbols_to_constellation = gr::digital::chunks_to_symbols_ic::make( d_constellation );

        connect( self(), 0, d_symbols_to_chips, 0 );
        connect( d_symbols_to_chips, 0, d_chips_to_symbols, 0 );
        connect( d_chips_to_symbols, 0, d_symbols_to_constellation, 0 );
        connect( d_symbols_to_constellation, 0, d_pskmod, 0 );
        connect( d_pskmod, 0, d_delay, 0 );
        connect( d_delay, 0, self(), 0 );
    }

    /*
     * Our virtual destructor.
     */
    ucla_mod_bc_impl::~ucla_mod_bc_impl()
    {
    }


  } /* namespace ieee802154 */
} /* namespace gr */

