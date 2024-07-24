//////////////////////////////////////////////////////////////////////////////
// This file is part of 'commonRF'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'commonRF', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#ifndef _LLRF_DOWNCONVERTER_FW_H
#define _LLRF_DOWNCONVERTER_FW_H

#include <cpsw_api_user.h>
#include <cpsw_api_builder.h>

class ILlrfDownConverterFw;
typedef shared_ptr <ILlrfDownConverterFw> LlrfDownConverterFw;

class ILlrfDownConverterFw : public virtual IEntry {
public:
    static LlrfDownConverterFw create(Path p);
    virtual double getTemp(int idx)                       = 0;
    virtual void   acqTemp(void)                          = 0;
    virtual void   getTemp(double temp[])                 = 0;
    virtual void   setAtten(uint32_t atten, int idx)      = 0;
};

#endif /* _LLRF_DOWNCONVERTER_FW_H */
