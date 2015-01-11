/* -*- c++ -*- */

#define IEEE802154_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ieee802154_swig_doc.i"

%{
#include "ieee802154/ucla_cc1k_correlator_cb.h"
#include "ieee802154/ucla_delay_cc.h"
#include "ieee802154/ucla_interleave.h"
#include "ieee802154/ucla_manchester_ff.h"
#include "ieee802154/ucla_multichanneladd_cc.h"
#include "ieee802154/ucla_qpsk_modulator_cc.h"
#include "ieee802154/ucla_symbols_to_chips_bi.h"
#include "ieee802154/ucla_packet_sink_f.h"
%}


%include "ieee802154/ucla_cc1k_correlator_cb.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_cc1k_correlator_cb);
%include "ieee802154/ucla_delay_cc.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_delay_cc);
%include "ieee802154/ucla_interleave.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_interleave);
%include "ieee802154/ucla_manchester_ff.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_manchester_ff);
%include "ieee802154/ucla_multichanneladd_cc.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_multichanneladd_cc);
%include "ieee802154/ucla_packet_sink_f.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_packet_sink_f);
%include "ieee802154/ucla_qpsk_modulator_cc.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_qpsk_modulator_cc);
%include "ieee802154/ucla_symbols_to_chips_bi.h"
GR_SWIG_BLOCK_MAGIC2(ieee802154, ucla_symbols_to_chips_bi);
