#ifndef _LLRF_UPCONVERTER_FW_H
#define _LLRF_UPCONVERTER_FW_H

#include <cpsw_api_user.h>
#include <cpsw_api_builder.h>

class ILlrfUpConverterFw;
typedef shared_ptr <ILlrfUpConverterFw> LlrfUpConverterFw;

class ILlrfUpConverterFw : public virtual IEntry {
public:
    static LlrfUpConverterFw create(Path p);
    virtual double getTemp(int idx)                         = 0;
    virtual void   acqTemp(void)                            = 0;
    virtual void   getTemp(double temp[])                   = 0;
    virtual void   setAtten(uint32_t val, int idx)          = 0;
};

#endif /* _LLRF_UPCONVERTER_FW_H */
