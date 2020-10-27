#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# GNU Radio version: 3.7.14.0
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import gnuradio.digital
import numpy
import pmt
import sdrcourse
import sys
import time
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000
        self.samp_per_symbol = samp_per_symbol = 2

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(0, 0)
        self.uhd_usrp_sink_0.set_gain(0, 0)
        self.sdrcourse_symbol_mapper_2 = sdrcourse.symbol_mapper(sdrcourse.BPSK, ([0,1]))
        self.sdrcourse_symbol_mapper_1 = sdrcourse.symbol_mapper(sdrcourse.BPSK, ([0,1]))
        self.sdrcourse_start_pending_0 = sdrcourse.start_pending(5, 10)
        self.sdrcourse_random_sourcecd_0 = sdrcourse.random_sourcecd(100, 0.5)
        self.sdrcourse_pulse_shaper_0 = sdrcourse.pulse_shaper(1, ([0.68,0.60,0.52,0.43,0.34,0.26,0.19,0.12,0.05,0.00]))
        self.sdrcourse_preamble_insert_0 = sdrcourse.preamble_insert(([0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,1,1,1,1,0,0,0,0]))
        self.sdrcourse_mix_0 = sdrcourse.mix()
        self.sdrcourse_crc_add_0 = sdrcourse.crc_add()
        self.digital_burst_shaper_xx_0 = digital.burst_shaper_cc((gnuradio.fft.window.hanning(1024)), 0, 0, False, "packet_len")
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1)
        self.blocks_pdu_to_tagged_stream_0 = blocks.pdu_to_tagged_stream(blocks.complex_t, 'packet_len')
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.6, ))
        self.blocks_message_strobe_0 = blocks.message_strobe(pmt.intern("TEST"), 1000)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.sdrcourse_start_pending_0, 'MSG'))
        self.msg_connect((self.sdrcourse_crc_add_0, 'out'), (self.sdrcourse_symbol_mapper_2, 'IN'))
        self.msg_connect((self.sdrcourse_mix_0, 'out'), (self.sdrcourse_preamble_insert_0, 'IN'))
        self.msg_connect((self.sdrcourse_preamble_insert_0, 'OUT'), (self.blocks_pdu_to_tagged_stream_0, 'pdus'))
        self.msg_connect((self.sdrcourse_random_sourcecd_0, 'OUT'), (self.sdrcourse_crc_add_0, 'in'))
        self.msg_connect((self.sdrcourse_start_pending_0, 'OUT'), (self.sdrcourse_random_sourcecd_0, 'IN'))
        self.msg_connect((self.sdrcourse_start_pending_0, 'OUT'), (self.sdrcourse_symbol_mapper_1, 'IN'))
        self.msg_connect((self.sdrcourse_symbol_mapper_1, 'OUT'), (self.sdrcourse_mix_0, 'in_h'))
        self.msg_connect((self.sdrcourse_symbol_mapper_2, 'OUT'), (self.sdrcourse_mix_0, 'in_p'))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.digital_burst_shaper_xx_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0, 0), (self.sdrcourse_pulse_shaper_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.digital_burst_shaper_xx_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.sdrcourse_pulse_shaper_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

    def get_samp_per_symbol(self):
        return self.samp_per_symbol

    def set_samp_per_symbol(self, samp_per_symbol):
        self.samp_per_symbol = samp_per_symbol


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
