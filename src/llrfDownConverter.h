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
