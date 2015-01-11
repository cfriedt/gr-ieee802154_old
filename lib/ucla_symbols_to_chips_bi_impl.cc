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
#include "ucla_symbols_to_chips_bi_impl.h"

namespace gr {
  namespace ieee802154 {

    static const unsigned int d_symbol_table[] = {
		0xd9c3522e,
		0xed9c3522,
		0x2ed9c352,
		0x22ed9c35,
		0x522ed9c3,
		0x3522ed9c,
		0xc3522ed9,
		0x9c3522ed,
		0x8c96077b,
		0xb8c96077,
		0x7b8c9607,
		0x77b8c960,
		0x077b8c96,
		0x6077b8c9,
		0x96077b8c,
		0xc96077b8,
    };

  static const int TABLE_SIZE = 16;

    ucla_symbols_to_chips_bi::sptr
    ucla_symbols_to_chips_bi::make()
    {
      return gnuradio::get_initial_sptr
        (new ucla_symbols_to_chips_bi_impl());
    }

    /*
     * The private constructor
     */
    ucla_symbols_to_chips_bi_impl::ucla_symbols_to_chips_bi_impl()
      : gr::sync_interpolator("ucla_symbols_to_chips_bi",
            gr::io_signature::make(1, 1, sizeof( uint8_t ) ),
            gr::io_signature::make(1, 1, sizeof( uint32_t ) ),
			2
        )
    {}

    /*
     * Our virtual destructor.
     */
    ucla_symbols_to_chips_bi_impl::~ucla_symbols_to_chips_bi_impl()
    {
    }

    int
    ucla_symbols_to_chips_bi_impl::work (int noutput_items,
    			gr_vector_const_void_star &input_items,
    			gr_vector_void_star &output_items)
    {
      assert (input_items.size() == output_items.size());

      const unsigned char *in = (unsigned char *) input_items[0];
      unsigned int *out = (unsigned int *) output_items[0];

      for (int i = 0; i < noutput_items; i+=2) {
        //fprintf(stderr, "%x %x, ", in[i/2]&0xF, (in[i/2]>>4)&0xF), fflush(stderr);

        // The LSBlock is sent first (802.15.4 standard)
        memcpy(&out[i+1], &d_symbol_table[(unsigned int)((in[i/2]>>4)&0xF)], sizeof(unsigned int));
        memcpy(&out[i], &d_symbol_table[(unsigned int)(in[i/2]&0xF)], sizeof(unsigned int));
      }

      return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

