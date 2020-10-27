SDR course Lab
Building and Installing the USRP Open-Source Toolchain (UHD and GNU Radio) on Linux

Install Linux:
If you already have a recent version of Linux installed, then you may be able to skip this section and we suggest that you use Ubuntu 18.04.
Download and install Ubuntu from the links below. Download the appropriate ISO image, and write it to a USB flash drive.
Ubuntu download page

Update and Install dependencies:
Before building UHD and GNU Radio, you need to make sure that all the dependencies are first installed. But firstly you should make sure that all the packages that are already installed on your system are up-to-date.
On Ubuntu systems, run:
sudo apt-get update

Once the system has been updated, then install the required dependencies for UHD and GNU Radio:
On Ubuntu 18.04 systems, run:
sudo apt-get -y install git swig cmake doxygen build-essential libboost-all-dev libtool libusb-1.0-0 libusb-1.0-0-dev libudev-dev libncurses5-dev libfftw3-bin libfftw3-dev libfftw3-doc libcppunit-1.14-0 libcppunit-dev libcppunit-doc ncurses-bin cpufrequtils python-numpy python-numpy-doc python-numpy-dbg python-scipy python-docutils qt4-bin-dbg qt4-default qt4-doc libqt4-dev libqt4-dev-bin python-qt4 python-qt4-dbg python-qt4-dev python-qt4-doc python-qt4-doc libqwt6abi1 libfftw3-bin libfftw3-dev libfftw3-doc ncurses-bin libncurses5 libncurses5-dev libncurses5-dbg libfontconfig1-dev libxrender-dev libpulse-dev swig g++ automake autoconf libtool python-dev libfftw3-dev libcppunit-dev libboost-all-dev libusb-dev libusb-1.0-0-dev fort77 libsdl1.2-dev python-wxgtk3.0 git libqt4-dev python-numpy ccache python-opengl libgsl-dev python-cheetah python-mako python-lxml doxygen qt4-default qt4-dev-tools libusb-1.0-0-dev libqwtplot3d-qt5-dev pyqt4-dev-tools python-qwt5-qt4 cmake git wget libxi-dev gtk2-engines-pixbuf r-base-dev python-tk liborc-0.4-0 liborc-0.4-dev libasound2-dev python-gtk2 libzmq3-dev libzmq5 python-requests python-sphinx libcomedi-dev python-zmq libqwt-dev libqwt6abi1 python-six libgps-dev libgps23 gpsd gpsd-clients python-gps python-setuptools
After installing the dependencies, you should reboot the system.

Building and installing UHD from source code:
UHD is open-source, and is hosted on GitHub. You can browse the code online at the link below, which points to version 3.14.0.0, which is the the latest release at the time of this writing.
UHD repository on GitHub
First, make a folder to hold the repository.
cd $HOME
mkdir sdrcourse
cd sdrcourse
Next, clone the repository and change into the cloned directory(Example: For UHD 3.14.0.0):
git clone https://github.com/EttusResearch/uhd
cd uhd
git checkout v3.14.0.0
Next, create a build folder within the repository, invoke CMake and build UHD:
cd host
mkdir build
cd build
cmake ../
make
make test
sudo make install
sudo ldconfig

Downloading the UHD FPGA Images:
You can now download the UHD FPGA Images for this installation. This can be done by running the command
cd
sudo uhd_images_downloader

Building and installing GNU Radio from source code:
As with UHD, GNU Radio is open-source and is hosted on GitHub. You can browse the code online at the link below. 
GNU Radio repository on GitHub
It is generally recommend at this time to use either the v3.7.13.4 or maint-3.7 branch of GNU Radio. The master branch includes many major changes such as converting to use Python 3 and may be unstable.
First, make a folder to hold the repository:
cd $HOME
cd sdrcourse
Next, clone the repository:
git clone --recursive https://github.com/gnuradio/gnuradio
Next, go into the repository and check out the desired GNU Radio version(Example: maint-3.7):
cd gnuradio
git checkout maint-3.7
Next, update the submodules:
git submodule update --init –recursive
Next, create a build folder within the repository, invoke CMake, and build GNU Radio:
mkdir build
cd build
cmake ../
make
make test
sudo make install
sudo ldconfig

Reference:
https://kb.ettus.com/Building_and_Installing_the_USRP_Open-Source_Toolchain_(UHD_and_GNU_Radio)_on_Linux
