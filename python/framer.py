#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2020 gnuradio.org.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

# this block does a few things
#
#  1. add a thing header that looks like
#   [ 0x1337:16 | data_len_bits:16 | header_crc:32 ]
#  2. it then inserts the payload bits
#  3. a crc 32 is placed after the payload
#  4. the burst bits are then padded out to a multiple of block size
#

import pmt, sys, pprint, bitarray, array, struct, binascii, math
from gnuradio import gr



class framer(gr.basic_block):
    """
    docstring for block framer
    """
    def __init__(self, blocksize=1024):
        gr.basic_block.__init__(self, "framer",[], [])
        self.message_port_register_out(pmt.intern("OUT"))
        self.message_port_register_in(pmt.intern("IN"))
        self.set_msg_handler(pmt.intern("IN"), self.handler)
        self.blocksize = blocksize;

    def handler(self, msg):
        ba = bitarray.bitarray();
        meta = pmt.car(msg);
        packed_data = pmt.cdr(msg);
        
        # convert pmt -> int list (of packed bytes)
        data = array.array('B', pmt.u8vector_elements(packed_data))

        # add header on front
        header = struct.pack('hh', 0x1337, 8*len(data));
        ba.frombytes(header);

        # compute header crc
        c2 = binascii.crc32(ba.tobytes());
        hcrc = struct.pack('i', c2);
        ba.frombytes(hcrc);

        # add the data payload
        ba.frombytes(data.tostring())
        
        # compute payload crc
        c2 = binascii.crc32(ba.tobytes());
        pcrc = struct.pack('i', c2);
        ba.frombytes(pcrc);
        
        # convert the unpacked bits to a list and back into a pmt u8vector
        burst_bits = pmt.init_u8vector(len(ba), ba.tolist());
        print "Tx Packet: " + ":".join("{:02x}".format(ord(c)) for c in ba.tobytes()[0:8])
        
        # make the new pdu
        pdu = pmt.cons(meta, burst_bits);

        # send it on its way
        self.message_port_pub(pmt.intern("OUT"), pdu);

