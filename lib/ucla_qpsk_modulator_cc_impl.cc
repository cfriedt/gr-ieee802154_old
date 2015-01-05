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
#include "ucla_qpsk_modulator_cc_impl.h"

namespace gr {
  namespace ieee802154 {

  	static const int SAMPLES_PER_SYMBOL = 4;

    ucla_qpsk_modulator_cc::sptr
    ucla_qpsk_modulator_cc::make()
    {
      return gnuradio::get_initial_sptr
        (new ucla_qpsk_modulator_cc_impl());
    }

    /*
     * The private constructor
     */
    ucla_qpsk_modulator_cc_impl::ucla_qpsk_modulator_cc_impl()
      : gr::sync_interpolator("ucla_qpsk_modulator_cc",
            gr::io_signature::make(1, 1, sizeof (gr_complex)),
            gr::io_signature::make(1, 1, sizeof (gr_complex)),
            SAMPLES_PER_SYMBOL
		)
    {}

    /*
     * Our virtual destructor.
     */
    ucla_qpsk_modulator_cc_impl::~ucla_qpsk_modulator_cc_impl()
    {
    }

    /**
     * Generate a QPSK signal from a +/- 1 float stream. For each
     * two input symbols we output 4 complex symbols with a half-sine
     * pulse shape.
     */
    int
    ucla_qpsk_modulator_cc_impl::work (int noutput_items,
    			gr_vector_const_void_star &input_items,
    			gr_vector_void_star &output_items)
    {
      const gr_complex *in = (gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      assert (noutput_items % SAMPLES_PER_SYMBOL == 0);

      for (int i = 0; i < noutput_items / SAMPLES_PER_SYMBOL; i++){
        float iphase = real(in[i]);
        float qphase = imag(in[i]);
        //fprintf(stderr, "%.0f %.0f ", iphase, qphase), fflush(stderr);

        *out++ = gr_complex(0.0, 0.0);
        *out++ = gr_complex(iphase * 0.70710678, qphase * 0.70710678);
        *out++ = gr_complex(iphase, qphase);
        *out++ = gr_complex(iphase * 0.70710678, qphase * 0.70710678);
      }

      return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

