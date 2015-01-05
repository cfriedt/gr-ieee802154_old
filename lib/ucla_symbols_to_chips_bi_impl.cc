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
		3653456430,
		3986437410,
		786023250,
		585997365,
		1378802115,
		891481500,
		3276943065,
		2620728045,
		2358642555,
		3100205175,
		2072811015,
		2008598880,
		125537430,
		1618458825,
		2517072780,
		3378542520
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
            gr::io_signature::make(1, -1, sizeof (unsigned char)),
            gr::io_signature::make(1, -1, sizeof (unsigned int)),
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
      int nstreams = input_items.size();

      for (int m=0;m<nstreams;m++) {
        const unsigned char *in = (unsigned char *) input_items[m];
        unsigned int *out = (unsigned int *) output_items[m];

        // per stream processing
        for (int i = 0; i < noutput_items; i+=2){
          //fprintf(stderr, "%x %x, ", in[i/2]&0xF, (in[i/2]>>4)&0xF), fflush(stderr);

          // The LSBlock is sent first (802.15.4 standard)
          memcpy(&out[i+1], &d_symbol_table[(unsigned int)((in[i/2]>>4)&0xF)], sizeof(unsigned int));
          memcpy(&out[i], &d_symbol_table[(unsigned int)(in[i/2]&0xF)], sizeof(unsigned int));
        }
        // end of per stream processing

      }
      return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

