CPSW_DIR=/afs/slac/g/lcls/package/cpsw/framework/R4.3.1/src

CCACHE_DISABLE=1

SRCDIR=.
-include $(SRCDIR)/release.mak

include $(CPSW_DIR)/defs.mak

INSTALL_DIR=../

HEADERS += rfCommon.h 
HEADERS += llrfDownConverter.h 
HEADERS += llrfUpConverter.h

commonRF_SRCS += rfCommon.cc 
commonRF_SRCS += llrfDownConverter.cc
commonRF_SRCS += llrfUpConverter.cc
commonRF_LIBS = $(CPSW_LIBS)



SHARED_LIBRARIES_YES+=commonRF
STATIC_LIBRARIES_YES+=commonRF


# PROGRAMS=

include $(CPSW_DIR)/rules.mak
