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

#include <gnuradio/io_signature.h>
#include "ucla_demod_cf_impl.h"

namespace gr {
  namespace ieee802154 {

    ucla_demod_cf::sptr
	ucla_demod_cf::make()
    {
      return gnuradio::get_initial_sptr
        (new ucla_demod_cf_impl());
    }

    /*
     * The private constructor
     */
    ucla_demod_cf_impl::ucla_demod_cf_impl()
      : gr::hier_block2("ucla_demod_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float))),
		d_sps( 2 ),
	    d_fmdemod( gr::analog::quadrature_demod_cf::make( 1 ) ),
		d_alpha( 0.0008 / d_sps ),
    	d_freq_offset( gr::filter::single_pole_iir_filter_ff::make( d_alpha ) ),
		d_sub( gr::blocks::sub_ff::make() ),
		d_omega( d_sps ),
		d_gain_mu( 0.03 ),
		d_mu( 0.5 ),
		d_omega_relative_limit( 0.0002 ),
		d_freq_error( 0.0 ),
		d_gain_omega( 0.25 * d_gain_mu * d_gain_mu ), // critically damped
		d_clock_recovery( gr::digital::clock_recovery_mm_ff::make( d_omega, d_gain_omega, d_mu, d_gain_mu, d_omega_relative_limit  ) )
    {
        connect( self(), 0, d_fmdemod, 0 );
        connect( d_fmdemod, 0, d_sub, 0 );
        connect( d_fmdemod, 0, d_freq_offset, 0 );
        connect( d_freq_offset, 0, d_sub, 1 );
        connect( d_sub, 0, d_clock_recovery, 0 );
        connect( d_clock_recovery, 0, self(), 0 );
    }

    /*
     * Our virtual destructor.
     */
    ucla_demod_cf_impl::~ucla_demod_cf_impl()
    {
    }


  } /* namespace ieee802154 */
} /* namespace gr */

