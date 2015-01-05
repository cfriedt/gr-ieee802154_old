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
#include "ucla_multichanneladd_cc_impl.h"

namespace gr {
  namespace ieee802154 {

    ucla_multichanneladd_cc::sptr
    ucla_multichanneladd_cc::make( size_t itemsize )
    {
      return gnuradio::get_initial_sptr
        (new ucla_multichanneladd_cc_impl( itemsize ));
    }

    /*
     * The private constructor
     */
    ucla_multichanneladd_cc_impl::ucla_multichanneladd_cc_impl( size_t itemsize )
      : gr::block("ucla_multichanneladd_cc",
            gr::io_signature::make(1, gr::io_signature::IO_INFINITE, itemsize),
            gr::io_signature::make(1, 1, itemsize)
	    ),
		d_itemsize (itemsize)
    {}

    /*
     * Our virtual destructor.
     */
    ucla_multichanneladd_cc_impl::~ucla_multichanneladd_cc_impl()
    {
    }

    void
    ucla_multichanneladd_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
		unsigned ninputs = ninput_items_required.size();
		for (unsigned i = 0; i < ninputs; i++) {
			ninput_items_required[i] = 0;
		}
    }

    int
    ucla_multichanneladd_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
    	  size_t nchan = input_items.size ();
    	  int	r = work (noutput_items, input_items, output_items);
    	  if (r > 0) {
    	    consume_each (r);
    	  }
    	  return r;
    }

    int
    ucla_multichanneladd_cc_impl::work (int noutput_items,
    		     gr_vector_const_void_star &input_items,
    		     gr_vector_void_star &output_items)
    {
      size_t nchan = input_items.size ();
      size_t itemsize = d_itemsize;
      const char **in = (const char **) &input_items[0];
      char *out = (char *) output_items[0];

      for (int i = 0; i < noutput_items; i += nchan){
        for (unsigned int n = 0; n < nchan; n++){
          memcpy (out, in[n], itemsize);
          out += itemsize;
          in[n] += itemsize;
        }
      }
      return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

