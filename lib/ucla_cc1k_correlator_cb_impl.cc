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
#include <gnuradio/blocks/count_bits.h>
#include "ucla_cc1k_correlator_cb_impl.h"

namespace gr {
  namespace ieee802154 {

    ucla_cc1k_correlator_cb::sptr
    ucla_cc1k_correlator_cb::make( int payload_bytesize,
              unsigned char sync_byte,
              unsigned char nsync_byte,
              unsigned char manchester )
    {
      return gnuradio::get_initial_sptr
        (new ucla_cc1k_correlator_cb_impl(
                payload_bytesize,
                sync_byte,
                nsync_byte,
                manchester ));
    }

    /*
     * The private constructor
     */
    ucla_cc1k_correlator_cb_impl::ucla_cc1k_correlator_cb_impl(
        int payload_bytesize,
        unsigned char sync_byte,
        unsigned char nsync_byte,
        unsigned char manchester )
      : gr::block("ucla_cc1k_correlator_cb",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))),
        d_payload_bytesize (payload_bytesize),
        d_sync_byte (sync_byte),
        d_nsync_byte (nsync_byte),
        d_manchester (manchester),
        d_state (ST_LOOKING),
        d_osi (0),
        d_bblen ((payload_bytesize) * CC1K_BITS_PER_BYTE),
        d_bitbuf (new float [2*d_bblen]),
        d_bbi (0),
        d_avbi(0),
        d_accum(0.0),
        d_avg(0)
    {
        for (int i = 0; i < AVG_PERIOD; i++)
            d_avgbuf[i] = 0.0;

		#ifdef DEBUG_UCLA_CC1K_CORRELATOR
		  d_debug_fp = fopen("corr.log", "w");
		#endif
		  enter_looking ();
    }

    /*
     * Our virtual destructor.
     */
    ucla_cc1k_correlator_cb_impl::~ucla_cc1k_correlator_cb_impl()
    {
		#ifdef DEBUG_UCLA_CC1K_CORRELATOR
		  fclose(d_debug_fp);
		#endif
		  delete [] d_bitbuf;
    }

    void
    ucla_cc1k_correlator_cb_impl::enter_looking ()
    {
      fflush (stdout);
    #ifdef DEBUG_UCLA_CC1K_CORRELATOR
      fprintf (stderr, ">>> enter_looking\n");
    #endif
      d_state = ST_LOOKING;
      for (int i = 0; i < OVERSAMPLE; i++)
        d_shift_reg[i] = 0;
      d_osi = 0;

      d_avbi = 0;
      d_avg = d_avg * 0.5;
      d_accum = 0;
      for (int i = 0; i < AVG_PERIOD; i++)
        d_avgbuf[i] = 0.0;
    }

    void
    ucla_cc1k_correlator_cb_impl::enter_under_threshold ()
    {
      fflush (stdout);
    #ifdef DEBUG_UCLA_CC1K_CORRELATOR
      fprintf (stderr, ">>> enter_under_threshold\n");
    #endif
      d_state = ST_UNDER_THRESHOLD;
      d_transition_osi = d_osi;
    }

    void
    ucla_cc1k_correlator_cb_impl::enter_locked ()
    {
      d_state = ST_LOCKED;
      int delta = sub_index (d_osi, d_transition_osi);
      d_center_osi = add_index (d_transition_osi, delta/2);
      d_center_osi = add_index (d_center_osi, 3);   // FIXME
      d_bbi = 0;
      fflush (stdout);
    #ifdef DEBUG_UCLA_CC1K_CORRELATOR
      fprintf (stderr, ">>> enter_locked  d_center_osi = %d\n", d_center_osi);
    #endif
      d_avg = std::max(-1.0, std::min(1.0, d_accum * (1.0/AVG_PERIOD)));
    #ifdef DEBUG_UCLA_CC1K_CORRELATOR
      fprintf(stderr, ">>> enter_locked  d_avg = %g\n", d_avg);
    #endif
    }

    /*static int
    packit (unsigned char *pktbuf, const unsigned char *bitbuf, int bitcount)
    {
      int t, t1, t2, errors=0;
    //
    //  for (int i = 0; i < bitcount; i += 8){
    //    t = bitbuf[i+0] & 0x1;
    //    t = (t << 1) | (bitbuf[i+1] & 0x1);
    //    t = (t << 1) | (bitbuf[i+2] & 0x1);
    //    t = (t << 1) | (bitbuf[i+3] & 0x1);
    //    t = (t << 1) | (bitbuf[i+4] & 0x1);
    //    t = (t << 1) | (bitbuf[i+5] & 0x1);
    //    t = (t << 1) | (bitbuf[i+6] & 0x1);
    //    t = (t << 1) | (bitbuf[i+7] & 0x1);
    //
      for (int i = 0; i < bitcount; i += 16){
        t = 0;
        for (int j = i; j < i+16; j += 2){
          t1 = bitbuf[j] & 0x1;
          t2 = bitbuf[j+1] & 0x1;
          if ( t1 == 0 && t2 == 1 ){
    	t = (t << 1) | 0;
          } else if ( t1 == 1 && t2 == 0 ){
    	t = (t << 1) | 1;
          } else {
    	// mistake in manchester encoding encode as 0
    	t = (t << 1) | 0;
    	errors += 1;
          }
        }

        *pktbuf++ = t;
      }
      return errors;
    }
    */

    void
    ucla_cc1k_correlator_cb_impl::update_avg(float x)
    {
      d_accum -= d_avgbuf[d_avbi];
      d_avgbuf[d_avbi] = x;
      d_accum += x;
      d_avbi = (d_avbi + 1) & (AVG_PERIOD-1);
    }

    int
    ucla_cc1k_correlator_cb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
		const float *in = (const float *) input_items[0];
		float *out = (float *) output_items[0];


		int n = 0;
		int nin = ninput_items[0];
		int decision;
		int hamming_dist;

		struct debug_data {
		float	raw_data;
		float	sampled;
		float 	enter_locked;
		} debug_data;

		while (n < nin){

		#ifdef DEBUG_UCLA_CC1K_CORRELATOR
		debug_data.raw_data = in[n];
		debug_data.sampled = 0.0;
		debug_data.enter_locked = 0.0;
		#endif

		switch (d_state){

		case ST_LOCKED:
		  if (d_osi == d_center_osi){

		#ifdef DEBUG_UCLA_CC1K_CORRELATOR
		debug_data.sampled = 1.0;
		#endif
		decision = slice (in[n]);
		assert(d_bbi < 2*d_bblen);
		d_bitbuf[d_bbi] = decision;
		d_bbi++;
		if (d_bbi >= 2*d_bblen){
		  int errors;
		  printf ("got whole packet\n");
		  /*
		  for(int i = 0; i < d_bbi/CC1K_BITS_PER_BYTE/2; i++){
			printf ("%2x ", pktbuf[i]);
		  }
		  printf ("\n");
		  */
		  memcpy (out, &d_bitbuf[0], 2*d_bblen);
		  enter_looking ();
		  consume_each (n + 1);
		  return d_bblen*2;
		}
		  }
		  break;

		case ST_LOOKING:
		case ST_UNDER_THRESHOLD:
		  update_avg(in[n]);
		  decision = slice (in[n]);
		  assert(d_osi < OVERSAMPLE);
		  d_shift_reg[d_osi] = (d_shift_reg[d_osi] << 1) | decision;

		  hamming_dist = gr::blocks::count_bits64 (d_shift_reg[d_osi] ^ CC1K_SYNC);
		  //printf ("%2d  %d\n", hamming_dist, d_osi);

		  if (d_state == ST_LOOKING && hamming_dist <= THRESHOLD){
		// We're seeing a good PN code, remember location
		enter_under_threshold ();
		  }
		  else if (d_state == ST_UNDER_THRESHOLD && hamming_dist > THRESHOLD){
		// no longer seeing good PN code, compute center of goodness
		enter_locked ();
		debug_data.enter_locked = 1.0;
		  }
		  break;

		default:
		  assert (0);
		}

		#ifdef DEBUG_UCLA_CC1K_CORRELATOR
		//    if(d_debug_fp)
		//  fwrite(&debug_data, sizeof (debug_data), 1, d_debug_fp);
		#endif

		d_osi = add_index (d_osi, 1);
		n++;
		}

		consume_each (n);
		return 0;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

