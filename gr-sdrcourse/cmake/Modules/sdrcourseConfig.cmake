INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SDRCOURSE sdrcourse)

FIND_PATH(
    SDRCOURSE_INCLUDE_DIRS
    NAMES sdrcourse/api.h
    HINTS $ENV{SDRCOURSE_DIR}/include
        ${PC_SDRCOURSE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SDRCOURSE_LIBRARIES
    NAMES gnuradio-sdrcourse
    HINTS $ENV{SDRCOURSE_DIR}/lib
        ${PC_SDRCOURSE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDRCOURSE DEFAULT_MSG SDRCOURSE_LIBRARIES SDRCOURSE_INCLUDE_DIRS)
MARK_AS_ADVANCED(SDRCOURSE_LIBRARIES SDRCOURSE_INCLUDE_DIRS)

