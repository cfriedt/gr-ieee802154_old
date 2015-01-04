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

#ifndef INCLUDED_IEEE802154_UCLA_QPSK_MODULATOR_CC_IMPL_H
#define INCLUDED_IEEE802154_UCLA_QPSK_MODULATOR_CC_IMPL_H

#include <ieee802154/ucla_qpsk_modulator_cc.h>

namespace gr {
  namespace ieee802154 {

    class ucla_qpsk_modulator_cc_impl : public ucla_qpsk_modulator_cc
    {
     private:
      // Nothing to declare in this block.

     public:
      ucla_qpsk_modulator_cc_impl();
      ~ucla_qpsk_modulator_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace ieee802154
} // namespace gr

#endif /* INCLUDED_IEEE802154_UCLA_QPSK_MODULATOR_CC_IMPL_H */

