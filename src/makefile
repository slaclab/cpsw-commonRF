ifeq ($(PACKAGE_TOP),)
ifneq ($(EPICS_PACKAGE_TOP),)
	PACKAGE_TOP=$(EPICS_PACKAGE_TOP)
else
	$(error PACKAGE_TOP or EPICS_PACKAGE_TOP must be provided in the environment or on the command line)
endif
endif

CPSW_DIR=$(PACKAGE_TOP)/cpsw/framework/R4.4.1/src

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
