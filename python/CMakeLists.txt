# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Include python install macros
########################################################################
include(GrPython)
if(NOT PYTHONINTERP_FOUND)
    return()
endif()

########################################################################
# Install python sources
########################################################################
GR_PYTHON_INSTALL(
    FILES
    __init__.py
    DESTINATION ${GR_PYTHON_DIR}/ieee802154
)

########################################################################
# Handle the unit tests
########################################################################
include(GrTest)

set(GR_TEST_TARGET_DEPS gnuradio-ieee802154)
set(GR_TEST_PYTHON_DIRS ${CMAKE_BINARY_DIR}/swig)
GR_ADD_TEST(qa_ucla_cc1k_correlator_cb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_cc1k_correlator_cb.py)
GR_ADD_TEST(qa_ucla_delay_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_delay_cc.py)
GR_ADD_TEST(qa_ucla_demod_cf ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_demod_cf.py)
GR_ADD_TEST(qa_ucla_interleave ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_interleave.py)
GR_ADD_TEST(qa_ucla_manchester_ff ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_manchester_ff.py)
GR_ADD_TEST(qa_ucla_mod_bc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_mod_bc.py)
GR_ADD_TEST(qa_ucla_multichanneladd_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_multichanneladd_cc.py)
GR_ADD_TEST(qa_ucla_packet_sink_f ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_packet_sink_f.py)
GR_ADD_TEST(qa_ucla_pdsu_framer_bb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_pdsu_framer_bb.py)
GR_ADD_TEST(qa_ucla_qpsk_modulator_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_qpsk_modulator_cc.py)
GR_ADD_TEST(qa_ucla_symbols_to_chips_bi ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_ucla_symbols_to_chips_bi.py)
