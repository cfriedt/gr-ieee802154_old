#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Sun Jan 11 23:18:45 2015
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import scopesink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import ieee802154
import numpy
import wx

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 62500
        self.samp_rate = samp_rate = 32*symbol_rate
        self.gain = gain = 30
        self.freq = freq = 2420e6
        self.data_rate = data_rate = 4*symbol_rate

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_scopesink2_0 = scopesink2.scope_sink_f(
        	self.GetWin(),
        	title="Scope Plot",
        	sample_rate=samp_rate,
        	v_scale=0,
        	v_offset=0,
        	t_scale=0,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=2,
        	trig_mode=wxgui.TRIG_MODE_AUTO,
        	y_axis_label="Counts",
        )
        self.Add(self.wxgui_scopesink2_0.win)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=512,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.ieee802154_ucla_symbols_to_chips_bi_0 = ieee802154.ucla_symbols_to_chips_bi()
        self.ieee802154_ucla_qpsk_modulator_cc_0 = ieee802154.ucla_qpsk_modulator_cc()
        self.ieee802154_ucla_pdsu_framer_bb_0 = ieee802154.ucla_pdsu_framer_bb()
        self.ieee802154_ucla_manchester_ff_0_0 = ieee802154.ucla_manchester_ff()
        self.ieee802154_ucla_manchester_ff_0 = ieee802154.ucla_manchester_ff()
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, data_rate,True)
        self.blocks_sub_xx_1_0 = blocks.sub_ff(1)
        self.blocks_sub_xx_1 = blocks.sub_ff(1)
        self.blocks_packed_to_unpacked_xx_0 = blocks.packed_to_unpacked_ii(1, gr.GR_LSB_FIRST)
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(4)
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vff((2.0, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((2.0, ))
        self.blocks_int_to_float_0_0 = blocks.int_to_float(1, 1)
        self.blocks_int_to_float_0 = blocks.int_to_float(1, 1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_deinterleave_0 = blocks.deinterleave(gr.sizeof_int*1, 1)
        self.blocks_complex_to_float_0 = blocks.complex_to_float(1)
        self.analog_random_source_x_0 = blocks.vector_source_b(map(int, numpy.random.randint(0, 2, 10000)), True)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 1.0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_float_to_complex_0, 0), (self.ieee802154_ucla_qpsk_modulator_cc_0, 0))
        self.connect((self.blocks_deinterleave_0, 1), (self.blocks_int_to_float_0_0, 0))
        self.connect((self.blocks_deinterleave_0, 0), (self.blocks_int_to_float_0, 0))
        self.connect((self.ieee802154_ucla_symbols_to_chips_bi_0, 0), (self.blocks_packed_to_unpacked_xx_0, 0))
        self.connect((self.blocks_packed_to_unpacked_xx_0, 0), (self.blocks_deinterleave_0, 0))
        self.connect((self.analog_random_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_complex_to_float_0, 0), (self.wxgui_scopesink2_0, 0))
        self.connect((self.blocks_complex_to_float_0, 1), (self.wxgui_scopesink2_0, 1))
        self.connect((self.ieee802154_ucla_qpsk_modulator_cc_0, 0), (self.blocks_complex_to_float_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.ieee802154_ucla_pdsu_framer_bb_0, 0))
        self.connect((self.ieee802154_ucla_pdsu_framer_bb_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.ieee802154_ucla_symbols_to_chips_bi_0, 0))
        self.connect((self.blocks_int_to_float_0, 0), (self.ieee802154_ucla_manchester_ff_0, 0))
        self.connect((self.blocks_int_to_float_0_0, 0), (self.ieee802154_ucla_manchester_ff_0_0, 0))
        self.connect((self.ieee802154_ucla_qpsk_modulator_cc_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.ieee802154_ucla_manchester_ff_0_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.ieee802154_ucla_manchester_ff_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_sub_xx_1, 1))
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_sub_xx_1_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.blocks_sub_xx_1_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_sub_xx_1, 0))
        self.connect((self.blocks_sub_xx_1, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_sub_xx_1_0, 0), (self.blocks_float_to_complex_0, 1))



    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_samp_rate(32*self.symbol_rate)
        self.set_data_rate(4*self.symbol_rate)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.wxgui_scopesink2_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq

    def get_data_rate(self):
        return self.data_rate

    def set_data_rate(self, data_rate):
        self.data_rate = data_rate
        self.blocks_throttle_0.set_sample_rate(self.data_rate)

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()
