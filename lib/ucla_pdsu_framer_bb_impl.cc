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
#include "ucla_pdsu_framer_bb_impl.h"

namespace gr {
  namespace ieee802154 {

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) ( sizeof( x ) / sizeof( ( x )[ 0 ] ) )
#endif

#define MAX_FRAME_LENGTH 127
    static int clamp( int noutput_items ) {
    	noutput_items = noutput_items > 127 ? 127 : noutput_items;
    	noutput_items = noutput_items < 0   ? 0   : noutput_items;
    	return noutput_items;
    }

    // this is really just an int, but at this point we should be using
    // bytes to represent bits
    static const uint8_t preamble[] = {
    	0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
    };

    // bit0..7
    static const uint8_t sfd[] = {
    	1, 1, 1, 0, 0, 1, 0, 1,
    };

    ucla_pdsu_framer_bb::sptr
    ucla_pdsu_framer_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new ucla_pdsu_framer_bb_impl());
    }

    /*
     * The private constructor
     */
    ucla_pdsu_framer_bb_impl::ucla_pdsu_framer_bb_impl()
      : gr::block("ucla_pdsu_framer_bb",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint8_t)))
    {}

    /*
     * Our virtual destructor.
     */
    ucla_pdsu_framer_bb_impl::~ucla_pdsu_framer_bb_impl()
    {
    }

    void
    ucla_pdsu_framer_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[ 0 ] =
			ARRAY_SIZE( preamble ) +
			ARRAY_SIZE( sfd ) +
			7 /* frame length */ +
			1 /* reserved */ +
			clamp( noutput_items );
    }

    int
    ucla_pdsu_framer_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const uint8_t *in = (const uint8_t *) input_items[0];
        uint8_t *out = (uint8_t *) output_items[0];

        uint8_t frame_length[ 8 ] = {
			!!( noutput_items & 0x40 ), !!( noutput_items & 0x20 ),
			!!( noutput_items & 0x10 ), !!( noutput_items & 0x08 ),
			!!( noutput_items & 0x04 ), !!( noutput_items & 0x02 ),
			!!( noutput_items & 0x01 ), /* reserved */
        };

    	noutput_items = clamp( noutput_items );

        memcpy( out, preamble, ARRAY_SIZE( preamble ) );
        out += ARRAY_SIZE( preamble );

        memcpy( out, sfd, ARRAY_SIZE( sfd ) );
        out += ARRAY_SIZE( sfd );

        memcpy( out, frame_length, ARRAY_SIZE( frame_length ) );
        out += ARRAY_SIZE( frame_length );

        memcpy( out, in, noutput_items );
        out += noutput_items;

        consume_each (noutput_items);

        return ( out - (uint8_t *) output_items[0] ) / sizeof( uint8_t * );
    }

  } /* namespace ieee802154 */
} /* namespace gr */

