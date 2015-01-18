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

#ifndef INCLUDED_IEEE802154_UCLA_DEMOD_CF_IMPL_H
#define INCLUDED_IEEE802154_UCLA_DEMOD_CF_IMPL_H

#include <gnuradio/analog/quadrature_demod_cf.h>
#include <gnuradio/filter/single_pole_iir_filter_ff.h>
#include <gnuradio/blocks/sub_ff.h>
#include <gnuradio/digital/clock_recovery_mm_ff.h>

#include <ieee802154/ucla_demod_cf.h>

namespace gr {
  namespace ieee802154 {

    class ucla_demod_cf_impl : public ucla_demod_cf
    {
     private:
    	uint32_t d_sps;
    	gr::analog::quadrature_demod_cf::sptr d_fmdemod;

    	uint32_t d_alpha;
    	gr::filter::single_pole_iir_filter_ff::sptr d_freq_offset;

    	gr::blocks::sub_ff::sptr d_sub;

		uint32_t d_omega;
		float d_gain_mu;
		float d_mu;
		float d_omega_relative_limit;
		float d_freq_error;
		float d_gain_omega;

		gr::digital::clock_recovery_mm_ff::sptr d_clock_recovery;


     public:
      ucla_demod_cf_impl();
      ~ucla_demod_cf_impl();

      // Where all the action really happens
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_DEMOD_CF_IMPL_H */

