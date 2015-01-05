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
#include "ucla_manchester_ff_impl.h"

namespace gr {
  namespace ieee802154 {

    ucla_manchester_ff::sptr
    ucla_manchester_ff::make()
    {
      return gnuradio::get_initial_sptr
        (new ucla_manchester_ff_impl());
    }

    /*
     * The private constructor
     */
    ucla_manchester_ff_impl::ucla_manchester_ff_impl()
      : gr::sync_interpolator("ucla_manchester_ff",
        gr::io_signature::make(1, -1, sizeof(float)),
        gr::io_signature::make(1, -1, sizeof(float)),
        16)
    {}

    /*
     * Our virtual destructor.
     */
    ucla_manchester_ff_impl::~ucla_manchester_ff_impl()
    {
    }

    int
    ucla_manchester_ff_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
    	  assert (input_items.size() == output_items.size());
    	  int nstreams = input_items.size();

    	  //fprintf(stderr, "\n-- %d, %d\n", noutput_items, nstreams);

    	  for (int m=0;m<nstreams;m++) {
    	    float out1;
    	    float out2;
    	    const float *in = (float *) input_items[m];
    	    float *out = (float *) output_items[m];
    	    // per stream processing
    	    for (int i = 0; i < noutput_items; i+=16){
    	      //fprintf(stderr, "%f ", in[i/16]), fflush(stderr);

    	      if(in[i/16] > 0.0){
    		out1 = 1.0;
    		out2 = 0.0;
    	      } else {
    		out1 = 0.0;
    		out2 = 1.0;
    	      }
    	      //create manchester output and upsample by 8
    	      for (int j = 0; j<8; j++){
    		memcpy(&out[i+j], &out1, sizeof(float));
    	      }
    	      for (int j = 8; j<16; j++){
    		memcpy(&out[i+j], &out2, sizeof(float));
    	      }

    	      //for (int j = 0; j<16; j++){
    	      //fprintf(stderr, "%f", out[i/16+j]);
    	      //}
    	    }
    	    // end of per stream processing

    	  }
    	  return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

