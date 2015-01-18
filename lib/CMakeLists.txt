# Copyright 2011,2012 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND ieee802154_sources
    ucla_cc1k_correlator_cb_impl.cc
    ucla_delay_cc_impl.cc
    ucla_interleave_impl.cc
    ucla_manchester_ff_impl.cc
    ucla_multichanneladd_cc_impl.cc
    ucla_packet_sink_f_impl.cc
    ucla_pdsu_framer_bb_impl.cc
    ucla_qpsk_modulator_cc_impl.cc
    ucla_symbols_to_chips_bi_impl.cc
)

set(ieee802154_sources "${ieee802154_sources}" PARENT_SCOPE)
if(NOT ieee802154_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT ieee802154_sources)

add_library(gnuradio-ieee802154 SHARED ${ieee802154_sources})
target_link_libraries(gnuradio-ieee802154 ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} gnuradio-blocks )
set_target_properties(gnuradio-ieee802154 PROPERTIES DEFINE_SYMBOL "gnuradio_ieee802154_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-ieee802154 PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-ieee802154
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_ieee802154_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ieee802154.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_ieee802154.cc
)

add_executable(test-ieee802154 ${test_ieee802154_sources})

target_link_libraries(
  test-ieee802154
  ${GNURADIO_RUNTIME_LIBRARIES}
  gnuradio-blocks
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-ieee802154
)

GR_ADD_TEST(test_ieee802154 test-ieee802154)
