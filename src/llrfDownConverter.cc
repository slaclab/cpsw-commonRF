//////////////////////////////////////////////////////////////////////////////
// This file is part of 'commonRF'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'commonRF', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#include "llrfDownConverter.h"


#include <cpsw_yaml.h>
#include <yaml-cpp/yaml.h>
#include <cpsw_sval.h>
#include <cpsw_entry_adapt.h>
#include <cpsw_hub.h>
#include <fstream>
#include <sstream>

#include <string.h>
#include <math.h>

#define MAX_ATTEN_CHN      6
#define MAX_TEMP_CHN       4
#define ATTEN_PATH_STR     "AttHMC624[%d]/SetValue"
#define TEMPCFG_PATH_STR   "Adt7420[%d]/Config"
#define TEMPMSB_PATH_STR   "Adt7420[%d]/TempMSByte"
#define TEMPLSB_PATH_STR   "Adt7420[%d]/TempLSByte"


#define CPSW_TRY_CATCH(X)       try {   \
        (X);                            \
    } catch (CPSWError &e) {            \
        fprintf(stderr,                 \
                "CPSW Error: %s at %s, line %d\n",     \
                e.getInfo().c_str(),    \
                __FILE__, __LINE__);    \
        throw e;                        \
    }


class CLlrfDownConverterFwAdapt;
typedef shared_ptr <CLlrfDownConverterFwAdapt> LlrfDownConverterFwAdapt;

class CLlrfDownConverterFwAdapt : public ILlrfDownConverterFw, public IEntryAdapt {
    protected:
        ScalVal     _tempConfig[MAX_TEMP_CHN];
        ScalVal_RO  _tempMSB[MAX_TEMP_CHN];
        ScalVal_RO  _tempLSB[MAX_TEMP_CHN];
        ScalVal     _atten[MAX_ATTEN_CHN];
    public:
        CLlrfDownConverterFwAdapt(Key &k, ConstPath p, shared_ptr<const CEntryImpl> ie);
        virtual double getTemp(int idx);
        virtual void   acqTemp(void);
        virtual void   getTemp(double temp[]);
        virtual void   setAtten(uint32_t val, int idx);
};

LlrfDownConverterFw ILlrfDownConverterFw::create(Path p)
{
    return IEntryAdapt::check_interface<LlrfDownConverterFwAdapt, DevImpl>(p);
}

CLlrfDownConverterFwAdapt::CLlrfDownConverterFwAdapt(Key &k, ConstPath p, shared_ptr<const CEntryImpl> ie) : 
  IEntryAdapt(k, p, ie)
{

    for(int i = 0; i<MAX_ATTEN_CHN; i++) {
        char name[80];
        sprintf(name, ATTEN_PATH_STR, i);  _atten[i] = IScalVal::create(p->findByName(name));
    }


    for(int i = 0; i<MAX_TEMP_CHN; i++) {
        char name[80];
        if(i==0) {sprintf(name, TEMPCFG_PATH_STR, i); _tempConfig[i] = IScalVal::create(p->findByName(name));} 
        sprintf(name, TEMPMSB_PATH_STR, i); _tempMSB[i] = IScalVal_RO::create(p->findByName(name));
        sprintf(name, TEMPLSB_PATH_STR, i); _tempLSB[i] = IScalVal_RO::create(p->findByName(name));
    }


    acqTemp();
}


double CLlrfDownConverterFwAdapt::getTemp(int idx)
{
    double   temp;
    uint8_t  msb, lsb;

    CPSW_TRY_CATCH(_tempMSB[idx]->getVal(&msb))
    CPSW_TRY_CATCH(_tempLSB[idx]->getVal(&lsb))

    if(msb & 0x80) {
        temp = ((msb<<8) + lsb - 0x10000) / 128.;
    }
    else {
        temp = ((msb<<8) + lsb) / 128.;
    }

    return temp;
}

void CLlrfDownConverterFwAdapt::acqTemp(void)
{
    uint8_t v = (0x01 << 7) | (0x01 << 5);

    CPSW_TRY_CATCH(_tempConfig[0]->setVal(v))
}


void CLlrfDownConverterFwAdapt::getTemp(double temp[])
{
    for(int i = 0; i < MAX_TEMP_CHN; i++) temp[i] = getTemp(i);

    acqTemp();
}

void CLlrfDownConverterFwAdapt::setAtten(uint32_t val, int idx)
{
    CPSW_TRY_CATCH(_atten[idx]->setVal(val))
}
