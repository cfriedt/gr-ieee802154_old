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
#include "ucla_delay_cc_impl.h"

namespace gr {
  namespace ieee802154 {

    ucla_delay_cc::sptr
    ucla_delay_cc::make( const int delay )
    {
      return gnuradio::get_initial_sptr
        (new ucla_delay_cc_impl( delay ));
    }

    /*
     * The private constructor
     */
    ucla_delay_cc_impl::ucla_delay_cc_impl( const int delay )
      : gr::sync_block("ucla_delay_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		d_delay = delay-1;
		set_history (delay);
    }

    /*
     * Our virtual destructor.
     */
    ucla_delay_cc_impl::~ucla_delay_cc_impl()
    {
    }

    int
    ucla_delay_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
        for (int j = 0; j < noutput_items; j++)
            out[j] = gr_complex (real(in[j+d_delay]), imag(in[j]));

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

