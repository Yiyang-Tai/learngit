# Copyright 2011 Free Software Foundation, Inc.
#
# This file was generated by gr_modtool, a tool from the GNU Radio framework
# This file is a part of gr-sdrcourse
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
install(FILES
    sdrcourse_start_pending.xml
    sdrcourse_random_sourcecd.xml
    sdrcourse_framer.xml
    sdrcourse_preamble_insert.xml
    sdrcourse_symbol_mapper.xml
    sdrcourse_pulse_shaper.xml
    sdrcourse_matched_filter.xml
    sdrcourse_symbol_sync_max.xml
    sdrcourse_framing.xml
    sdrcourse_header_framing.xml
    sdrcourse_crc_add.xml
    sdrcourse_mix.xml
    sdrcourse_rotator.xml
    sdrcourse_baseband_derotation.xml
    sdrcourse_demod.xml
    sdrcourse_header_payload_demux.xml DESTINATION share/gnuradio/grc/blocks
)
