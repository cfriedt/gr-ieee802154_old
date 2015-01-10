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

#include <string.h>

#include <gnuradio/io_signature.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/message.h>
#include <gnuradio/blocks/count_bits.h>
#include "ucla_packet_sink_f_impl.h"

namespace gr {
  namespace ieee802154 {

	// very verbose output for almost each sample
	#define VERBOSE 0
	// less verbose output for higher level debugging
	#define VERBOSE2 0

	#define V_( x, fmt, args...) do { if ( x ) { fprintf( stderr, fmt "\n", ##args ); fflush(stderr); } } while(0)
	#define V(fmt, args...) V_( VERBOSE, fmt, ##args )
	#define V2(fmt, args...) V_( VERBOSE2, fmt, ##args )

	static const int DEFAULT_THRESHOLD = 10;  // detect access code with up to DEFAULT_THRESHOLD bits wrong

	// this is the mapping between chips and symbols if we do
	// a fm demodulation of the O-QPSK signal. Note that this
	// is different than the O-QPSK chip sequence from the
	// 802.15.4 standard since there there is a translation
	// happening.
	// See "CMOS RFIC Architectures for IEEE 802.15.4 Networks",
	// John Notor, Anthony Caviglia, Gary Levy, for more details.
	static const unsigned int CHIP_MAPPING[] = {
		1618456172,
		1309113062,
		1826650030,
		1724778362,
		778887287,
		2061946375,
		2007919840,
		125494990,
		529027475,
		838370585,
		320833617,
		422705285,
		1368596360,
		85537272,
		139563807,
		2021988657,
	};

    ucla_packet_sink_f::sptr
    ucla_packet_sink_f::make( boost::shared_ptr<gr::messages::msg_queue> target_queue, int threshold )
    {
      return gnuradio::get_initial_sptr
        (new ucla_packet_sink_f_impl( target_queue, threshold ));
    }

    /*
     * The private constructor
     */
    ucla_packet_sink_f_impl::ucla_packet_sink_f_impl( boost::shared_ptr<gr::messages::msg_queue> target_queue, int threshold )
      : gr::sync_block(
            "ucla_packet_sink_f",
			gr::io_signature::make(1, 1, sizeof(float)),
			gr::io_signature::make(0, 0, 0)
        ),
	    d_target_queue(target_queue),
	    d_threshold(threshold == -1 ? DEFAULT_THRESHOLD : threshold),
		d_sync_vector(0xA7),
		d_processed(0),
		d_lqi(0),
		d_lqi_sample_count(0)
    {
		V("syncvec: %x, threshold: %d", d_sync_vector, d_threshold);
		enter_search();
    }

    /*
     * Our virtual destructor.
     */
    ucla_packet_sink_f_impl::~ucla_packet_sink_f_impl()
    {
    }

    inline void
    ucla_packet_sink_f_impl::enter_search()
    {
      if (VERBOSE)
        fprintf(stderr, "@ enter_search\n");

      d_state = STATE_SYNC_SEARCH;
      d_shift_reg = 0;
      d_preamble_cnt = 0;
      d_chip_cnt = 0;
      d_packet_byte = 0;
    }

    inline void
    ucla_packet_sink_f_impl::enter_have_sync()
    {
		V("@ enter_have_sync");

		d_state = STATE_HAVE_SYNC;
		d_packetlen_cnt = 0;
		d_packet_byte = 0;
		d_packet_byte_index = 0;

		// Link Quality Information
		d_lqi = 0;
		d_lqi_sample_count = 0;
    }

    inline void
    ucla_packet_sink_f_impl::enter_have_header(int payload_len)
    {
		V("@ enter_have_header (payload_len = %d)", payload_len);

		d_state = STATE_HAVE_HEADER;
		d_packetlen  = payload_len;
		d_payload_cnt = 0;
		d_packet_byte = 0;
		d_packet_byte_index = 0;
    }

    inline unsigned char
    ucla_packet_sink_f_impl::decode_chips(unsigned int chips) {
		int i;
		int best_match = 0xFF;
		int min_threshold = 33; // Matching to 32 chips, could never have a error of 33 chips

		for(i=0; i<16; i++) {
			// FIXME: we can store the last chip
			// ignore the first and last chip since it depends on the last chip.
			unsigned int threshold = gr::blocks::count_bits32((chips&0x7FFFFFFE) ^ (CHIP_MAPPING[i]&0x7FFFFFFE));

			if (threshold < min_threshold) {
				best_match = i;
				min_threshold = threshold;
			}
		}

		if (min_threshold < d_threshold) {
			V("Found sequence with %d errors at 0x%x", min_threshold, (chips&0x7FFFFFFE) ^ (CHIP_MAPPING[best_match]&0x7FFFFFFE));
			// LQI: Average number of chips correct * MAX_LQI_SAMPLES
			//
			if (d_lqi_sample_count < MAX_LQI_SAMPLES) {
				d_lqi += 32 - min_threshold;
				d_lqi_sample_count++;
			}

			return (char)best_match&0xF;
		}

		return 0xFF;
    }

    boost::shared_ptr<ucla_packet_sink_f>
    make( boost::shared_ptr<gr::messages::msg_queue> target_queue, int threshold )
    {
      return boost::shared_ptr<ucla_packet_sink_f>(new ucla_packet_sink_f_impl(target_queue, threshold));
    }

    int
    ucla_packet_sink_f_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
		float *inbuf = (float *) input_items[0];
		int count=0;

		V(">>> Entering state machine");
		d_processed += noutput_items;

		while (count<noutput_items) {

			switch(d_state) {

				case STATE_SYNC_SEARCH:    // Look for sync vector

				V("SYNC Search, noutput=%d syncvec=%x",noutput_items, d_sync_vector);

				while (count < noutput_items) {

					//if(inbuf[count++] == 0.0)
					//  continue;
					if(slice(inbuf[count++])) {
						d_shift_reg = (d_shift_reg << 1) | 1;
					} else {
						d_shift_reg = d_shift_reg << 1;
					}

					if(d_preamble_cnt > 0){
						d_chip_cnt = d_chip_cnt+1;
					}

					// The first if block syncronizes to chip sequences.
					if(d_preamble_cnt == 0) {
						unsigned int threshold;
						threshold = gr::blocks::count_bits32((d_shift_reg&0x7FFFFFFE) ^ (CHIP_MAPPING[0]&0x7FFFFFFE));
						if(threshold < d_threshold) {
							//  fprintf(stderr, "Threshold %d d_preamble_cnt: %d\n", threshold, d_preamble_cnt);
							//if ((d_shift_reg&0xFFFFFE) == (CHIP_MAPPING[0]&0xFFFFFE)) {
							V2("Found 0 in chip sequence");
							// we found a 0 in the chip sequence
							d_preamble_cnt+=1;
							//fprintf(stderr, "Threshold %d d_preamble_cnt: %d\n", threshold, d_preamble_cnt);
						}
					} else {
						// we found the first 0, thus we only have to do the calculation every 32 chips
						if(d_chip_cnt == 32) {
							d_chip_cnt = 0;
							if(d_packet_byte == 0) {
								if (gr::blocks::count_bits32((d_shift_reg&0x7FFFFFFE) ^ (CHIP_MAPPING[0]&0xFFFFFFFE)) <= d_threshold) {
									V2("Found %d 0 in chip sequence", d_preamble_cnt);
									// we found an other 0 in the chip sequence
									d_packet_byte = 0;
									d_preamble_cnt++;
								} else if (gr::blocks::count_bits32((d_shift_reg&0x7FFFFFFE) ^ (CHIP_MAPPING[7]&0xFFFFFFFE)) <= d_threshold) {
									V2("Found first SFD");
									d_packet_byte = 7<<4;
								} else {
									// we are not in the synchronization header
									V2("Wrong first byte of SFD. %u", d_shift_reg);
									enter_search();
									break;
								}
							} else {
								if (gr::blocks::count_bits32((d_shift_reg&0x7FFFFFFE) ^ (CHIP_MAPPING[10]&0xFFFFFFFE)) <= d_threshold) {
									d_packet_byte |= 0xA;
									V2("Found sync, 0x%x", d_packet_byte);
									// found SDF
									// setup for header decode
									enter_have_sync();
									break;
								} else {
									V("Wrong second byte of SFD. %u", d_shift_reg);
									enter_search();
									break;
								}
							}
						}
					}
				}
				break;

				case STATE_HAVE_SYNC:

					V2("Header Search bitcnt=%d, header=0x%08x", d_headerbitlen_cnt, d_header);

					while (count < noutput_items) {		// Decode the bytes one after another.
						if(slice(inbuf[count++])) {
							d_shift_reg = (d_shift_reg << 1) | 1;
						} else {
							d_shift_reg = d_shift_reg << 1;
						}

						d_chip_cnt = d_chip_cnt+1;

						if(d_chip_cnt == 32) {
							d_chip_cnt = 0;
							unsigned char c = decode_chips(d_shift_reg);
							if(c == 0xFF){
								// something is wrong. restart the search for a sync
								V2("Found a not valid chip sequence! %u", d_shift_reg);

								enter_search();
								break;
							}

							if(d_packet_byte_index == 0) {
								d_packet_byte = c;
							} else {
								// c is always < 15
								d_packet_byte |= c << 4;
							}
							d_packet_byte_index = d_packet_byte_index + 1;
							if(d_packet_byte_index%2 == 0) {
								// we have a complete byte which represents the frame length.
								int frame_len = d_packet_byte;
								if(frame_len <= MAX_PKT_LEN) {
									enter_have_header(frame_len);
								} else {
									enter_search();
								}
								break;
							}
						}
					}
					break;

				case STATE_HAVE_HEADER:
					V2("Packet Build count=%d, noutput_items=%d, packet_len=%d", count, noutput_items, d_packetlen);

					while (count < noutput_items) {   // shift bits into bytes of packet one at a time
						if(slice(inbuf[count++])) {
							d_shift_reg = (d_shift_reg << 1) | 1;
						} else {
							d_shift_reg = d_shift_reg << 1;
						}

						d_chip_cnt = (d_chip_cnt+1)%32;

						if(d_chip_cnt == 0) {
							unsigned char c = decode_chips(d_shift_reg);
							if(c == 0xff) {
								// something is wrong. restart the search for a sync
								V2("Found a not valid chip sequence! %u", d_shift_reg);

								enter_search();
								break;
							}
							// the first symbol represents the first part of the byte.
							if(d_packet_byte_index == 0) {
								d_packet_byte = c;
							} else {
								// c is always < 15
								d_packet_byte |= c << 4;
							}
							//fprintf(stderr, "%d: 0x%x\n", d_packet_byte_index, c);
							d_packet_byte_index = d_packet_byte_index + 1;
							if(d_packet_byte_index%2 == 0) {
								// we have a complete byte
								V2("packetcnt: %d, payloadcnt: %d, payload 0x%x, d_packet_byte_index: %d", d_packetlen_cnt, d_payload_cnt, d_packet_byte, d_packet_byte_index);

								d_packet[d_packetlen_cnt++] = d_packet_byte;
								d_payload_cnt++;
								d_packet_byte_index = 0;

								if (d_payload_cnt >= d_packetlen) {	// packet is filled, including CRC. might do check later in here
									unsigned int scaled_lqi = (d_lqi / MAX_LQI_SAMPLES) << 3;
									unsigned char lqi = (scaled_lqi >= 256? 255 : scaled_lqi);

									// build a message, leave space for LQI
									// Message will be:
									// |-----------------|--------------------------|
									// |    LQI (1 Byte) |   MPDU (variable length) |
									// |-----------------|--------------------------|
									//
									boost::shared_ptr<gr::message> msg = gr::message::make(0, 0, 0, d_packetlen_cnt + sizeof(unsigned char));

									memcpy(msg->msg(), &lqi, sizeof(unsigned char));
									memcpy(msg->msg() + sizeof(unsigned char), d_packet, d_packetlen_cnt);

									pmt::pmt_t intrusive_msg = pmt::make_any( msg.get() );

									d_target_queue->insert_tail( intrusive_msg );		// send it
									msg.reset();  				// free it up
									V2("Adding message of size %d to queue", d_packetlen_cnt);
									enter_search();
									break;
								}
							}
						}
					}
					break;
			default:
				assert(0);
			}
		}

		V2("Samples Processed: %d", d_processed);

		return noutput_items;
    }

  } /* namespace ieee802154 */
} /* namespace gr */

