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

#ifndef INCLUDED_IEEE802154_UCLA_CC1K_CORRELATOR_CB_IMPL_H
#define INCLUDED_IEEE802154_UCLA_CC1K_CORRELATOR_CB_IMPL_H

#include <ieee802154/ucla_cc1k_correlator_cb.h>

namespace gr {
  namespace ieee802154 {

    class ucla_cc1k_correlator_cb_impl : public ucla_cc1k_correlator_cb
    {
    private:
      static const int OVERSAMPLE = 8;
      enum state_t { ST_LOOKING, ST_UNDER_THRESHOLD, ST_LOCKED };

      int	  	 d_payload_bytesize;
      unsigned char  d_sync_byte;                   // syncronisation byte
      unsigned char  d_nsync_byte;                  // indicates end of sync
      unsigned char  d_manchester;                  // 0: manchester off, 1: manchester on
      state_t	 d_state;
      unsigned int	 d_osi;				// over sample index [0,OVERSAMPLE-1]
      unsigned int	 d_transition_osi;		// first index where Hamming dist < thresh
      unsigned int	 d_center_osi;			// center of bit
      unsigned long long int d_shift_reg[OVERSAMPLE];
      int		 d_bblen;			// length of bitbuf
      float 	*d_bitbuf;			// demodulated bits
      int		 d_bbi;				// bitbuf index

      static const int AVG_PERIOD = 512;		// must be power of 2 (for freq offset correction)

      static const unsigned long long CC1K_SYNC = 0x9999999999999999ULL;

      static const int CC1K_BITS_PER_BYTE = 8;
      static const int CC1K_SYNC_OVERHEAD = sizeof(CC1K_SYNC);
      static const int CC1K_PAYLOAD_OVERHEAD = 0;		  	// 0 byte overhead
      static const int CC1K_TAIL_PAD = 1;				// one byte trailing padding
      static const int CC1K_OVERHEAD = CC1K_SYNC_OVERHEAD + CC1K_PAYLOAD_OVERHEAD + CC1K_TAIL_PAD;

      static const int THRESHOLD = 3;

      int	d_avbi;
      float	d_avgbuf[AVG_PERIOD];
      float d_avg;
      float d_accum;

    #ifdef DEBUG_UCLA_CC1K_CORRELATOR
      FILE		*d_debug_fp;			// binary log file
    #endif

      inline int slice (float x)
      {
        return x >= d_avg ? 1 : 0;
      }

      void update_avg(float x);

      void enter_locked ();
      void enter_under_threshold ();
      void enter_looking ();

      static int add_index (int a, int b)
      {
        int t = a + b;
        if (t >= OVERSAMPLE)
          t -= OVERSAMPLE;
        assert (t >= 0 && t < OVERSAMPLE);
        return t;
      }

      static int sub_index (int a, int b)
      {
        int t = a - b;
        if (t < 0)
          t += OVERSAMPLE;
        assert (t >= 0 && t < OVERSAMPLE);
        return t;
      }

     public:
      ucla_cc1k_correlator_cb_impl(
    		  int payload_bytesize,
			  unsigned char sync_byte,
			  unsigned char nsync_byte,
			  unsigned char manchester);
      ~ucla_cc1k_correlator_cb_impl();

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_CC1K_CORRELATOR_CB_IMPL_H */

