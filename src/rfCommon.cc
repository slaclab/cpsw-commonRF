//////////////////////////////////////////////////////////////////////////////
// This file is part of 'commonRF'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'commonRF', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#include "rfCommon.h"

#include <cpsw_yaml.h>
#include <yaml-cpp/yaml.h>
#include <cpsw_sval.h>
#include <cpsw_entry_adapt.h>
#include <cpsw_hub.h>
#include <fstream>
#include <sstream>

#include <string.h>
#include <math.h>

#define  MAX_CHN   12
#define  MAX_STREAM 8

#define CPSW_TRY_CATCH(X)       try {   \
        (X);                            \
    } catch (CPSWError &e) {            \
        fprintf(stderr,                 \
                "CPSW Error: %s at %s, line %d\n",     \
                e.getInfo().c_str(),    \
                __FILE__, __LINE__);    \
        throw e;                        \
    }


class CRFCommonFwAdapt;
typedef shared_ptr<CRFCommonFwAdapt> RFCommonFwAdapt;

class CRFCommonFwAdapt : public IRFCommonFw, public IEntryAdapt {
    protected:
        // path for llrfCommon
        Path _pLlrfDemod;
        Path _pLlrfPll;
        Path _pLlrfUpConverter;

        // registers for llrfDemod
        ScalVal_RO       _demod_version;
        ScalVal_RO       _phase[MAX_CHN];
        ScalVal_RO       _amp[MAX_CHN];
        ScalVal_RO       _phaseMaxHold[MAX_CHN];
        ScalVal_RO       _ampMaxHold[MAX_CHN];

        ScalVal          _setI[MAX_CHN];
        ScalVal          _setQ[MAX_CHN];

        ScalVal          _maxHoldReset;

        // registers for llrfPll
        ScalVal_RO       _pll_version;

        ScalVal_RO       _loPhase;
        ScalVal_RO       _loAmp;
        ScalVal_RO       _loLockCount;
        ScalVal_RO       _loLocked;
        ScalVal_RO       _loLoss;

        ScalVal_RO       _clockPhase;
        ScalVal_RO       _clockAmp;
        ScalVal_RO       _clockLockCount;
        ScalVal_RO       _clockLocked;
        ScalVal_RO       _clockLoss;

        ScalVal          _lockThreshold;
        ScalVal          _lossThreshold;
        Command          _resetLOLockCount;
        Command          _resetClockLockCount;
        ScalVal          _loDacControlMux;
        ScalVal          _clockDacControlMux;
        ScalVal          _userDacControlMux;
        ScalVal          _loRefSel;
        ScalVal          _clockRefSel;
        ScalVal          _loPolarity;
        ScalVal          _clockPolarity;
        ScalVal          _loDacSet;
        ScalVal          _clockDacSet;
        ScalVal          _userDacSet;
        ScalVal          _loPhaseSet;
        ScalVal          _clockPhaseSet;
        ScalVal          _loKp;
        ScalVal          _loKi;
        ScalVal          _clockKp;
        ScalVal          _clockKi;

        // registers for llrfUpConverter
        ScalVal_RO       _upCnv_version;
        ScalVal_RO       _refPhase0;
        ScalVal_RO       _refAmp0;
        ScalVal_RO       _refPhase1;
        ScalVal_RO       _refAmp1;
        ScalVal          _rfEnable;
        ScalVal          _modeSel;
        ScalVal          _loSel;
        ScalVal          _refChn0;
        ScalVal          _refChn1;
        ScalVal          _invertCorr;
        ScalVal          _basebandISet;
        ScalVal          _basebandQSet;
        ScalVal          _dacOffset;
        ScalVal          _dacMin;
        ScalVal          _dacMax;
        ScalVal          _correctionGain;

        Stream           _stream[MAX_STREAM];

    public:
        CRFCommonFwAdapt(Key &k, ConstPath p, shared_ptr<const CEntryImpl> ie);

        virtual void createStream(Path p);
        virtual uint32_t readStream(void *buf, uint64_t size, int idx);
        virtual void getDemodVersion(uint32_t *version);
        virtual void maxHoldReset(uint32_t val);
        virtual double getPhase(uint32_t *raw_phase, int idx);
        virtual double getAmp(uint32_t *raw_amp, int idx);
        virtual double getMaxPhase(uint32_t *raw_phase, int idx);
        virtual double getMaxAmp(uint32_t *raw_amp, int idx);
        virtual void getRotationIQ(uint32_t *i, uint32_t *q, int idx);
        virtual void setRotationIQ(uint32_t  i, uint32_t  q, int idx);
        virtual void setRotationPA(double phase, double amp, int idx);
        virtual void getPllVersion(uint32_t *version);
        virtual double getLOPhase(uint32_t *raw_phase);
        virtual double getLOAmp(uint32_t *raw_amp);
        virtual void getLOLockCount(uint32_t *count);
        virtual void getLOLockStatus(uint32_t *locked);
        virtual void getLOLossStatus(uint32_t *loss);
        virtual double getClockPhase(uint32_t *raw_phase);
        virtual double getClockAmp(uint32_t *raw_amp);
        virtual void getClockLockCount(uint32_t *count);
        virtual void getClockLockStatus(uint32_t *locked);
        virtual void getClockLossStatus(uint32_t *loss);
        virtual void getLockThreshold(uint32_t *val);
        virtual void setLockThreshold(uint32_t  val);
        virtual void getLossThreshold(uint32_t *val);
        virtual void setLossThreshold(uint32_t  val);
        virtual void resetLOLockCount();
        virtual void resetClockLockCount();
        virtual void loDacControlMux(uint32_t val);
        virtual void clockDacControlMux(uint32_t val);
        virtual void userDacControlMux(uint32_t val);
        virtual void loReferenceSelect(uint32_t val);
        virtual void clockReferenceSelect(uint32_t val);
        virtual void loPolarity(uint32_t val);
        virtual void clockPolarity(uint32_t val);
        virtual void setLODac(uint32_t val);
        virtual void setClockDac(uint32_t val);
        virtual void setUserDac(uint32_t val);
        virtual void setLOPhase(uint32_t val);
        virtual void setClockPhase(uint32_t val);
        virtual void setLOKp(uint32_t val);
        virtual void setLOKi(uint32_t val);
        virtual void setClockKp(uint32_t val);
        virtual void setClockKi(uint32_t val);
        virtual void getUpCnvVersion(uint32_t *version);
        virtual double getRefPhase(uint32_t *raw_phase, int idx);
        virtual double getRefAmp(uint32_t *raw_amp, int idx);
        virtual void rfEnable(uint32_t val);
        virtual void modeSelect(uint32_t val);
        virtual void loSelect(uint32_t val);
        virtual void refChnSelect(uint32_t val, int idx);
        virtual void invertCorrection(uint32_t val);
        virtual void getBasebandIQ(uint32_t *i, uint32_t *q);
        virtual void setBasebandIQ(uint32_t i, uint32_t q);
        virtual void setBasebandPA(double phase, double amp);
        virtual void setDacOffset(uint32_t val);
        virtual void setDacMin(uint32_t val);
        virtual void setDacMax(uint32_t val);
        virtual void setCorrectionGain(uint32_t val);
};

RFCommonFw IRFCommonFw::create(Path p)
{
    return IEntryAdapt::check_interface<RFCommonFwAdapt, DevImpl>(p);
}


CRFCommonFwAdapt::CRFCommonFwAdapt(Key &k, ConstPath p, shared_ptr<const CEntryImpl> ie) : 
  IEntryAdapt(k, p, ie),
    // path
    _pLlrfDemod(      p->findByName("LlrfDemod")),
    _pLlrfPll(        p->findByName("LlrfPll")),
    _pLlrfUpConverter(p->findByName("LlrfUpconvert")),
    // registers for LlrfDemod
    _demod_version(IScalVal_RO::create(_pLlrfDemod->findByName("version"))),
    _maxHoldReset( IScalVal   ::create(_pLlrfDemod->findByName("maxHoldReset"))),
    // register for LlrfPll
    _pll_version(   IScalVal_RO::create(_pLlrfPll->findByName("version"))),
    _loPhase(       IScalVal_RO::create(_pLlrfPll->findByName("loPllPhase"))),
    _loAmp(         IScalVal_RO::create(_pLlrfPll->findByName("loPllAmplitude"))),
    _loLockCount(   IScalVal_RO::create(_pLlrfPll->findByName("loPllLockCount"))),
    _loLocked(      IScalVal_RO::create(_pLlrfPll->findByName("loPllLocked"))),
    _loLoss(        IScalVal_RO::create(_pLlrfPll->findByName("loPllLossOfSignal"))),
    _clockPhase(    IScalVal_RO::create(_pLlrfPll->findByName("clockPllPhase"))),
    _clockAmp(      IScalVal_RO::create(_pLlrfPll->findByName("clockPllAmplitude"))),
    _clockLockCount(IScalVal_RO::create(_pLlrfPll->findByName("clockPllLockCount"))),
    _clockLocked(   IScalVal_RO::create(_pLlrfPll->findByName("clockPllLocked"))),
    _clockLoss(     IScalVal_RO::create(_pLlrfPll->findByName("clockPllLossOfSignal"))),
    _lockThreshold(       IScalVal::create(_pLlrfPll->findByName("lossOfLockThreshold"))),
    _lossThreshold(       IScalVal::create(_pLlrfPll->findByName("lossOfSignalThreshold"))),
    _resetLOLockCount(    ICommand::create(_pLlrfPll->findByName("ResetLOPllLockedCounter"))),
    _resetClockLockCount( ICommand::create(_pLlrfPll->findByName("ResetClockPllLockedCounter"))),
    _loDacControlMux(     IScalVal::create(_pLlrfPll->findByName("loDacControlMux"))),
    _clockDacControlMux(  IScalVal::create(_pLlrfPll->findByName("clockDacControlMux"))),
    _userDacControlMux(   IScalVal::create(_pLlrfPll->findByName("userDacControlMux"))),
    _loRefSel(            IScalVal::create(_pLlrfPll->findByName("loPllReferenceSelect"))),
    _clockRefSel(         IScalVal::create(_pLlrfPll->findByName("clockPllReferenceSelect"))),
    _loPolarity(          IScalVal::create(_pLlrfPll->findByName("loPllPolarity"))),
    _clockPolarity(       IScalVal::create(_pLlrfPll->findByName("clockPllPolarity"))),
    _loDacSet(            IScalVal::create(_pLlrfPll->findByName("loDacSet"))),
    _clockDacSet(         IScalVal::create(_pLlrfPll->findByName("clockDacSet"))),
    _userDacSet(          IScalVal::create(_pLlrfPll->findByName("userDacSet"))),
    _loPhaseSet(          IScalVal::create(_pLlrfPll->findByName("loPllPhaseSet"))),
    _clockPhaseSet(       IScalVal::create(_pLlrfPll->findByName("clockPllPhaseSet"))),
    _loKp(                IScalVal::create(_pLlrfPll->findByName("loPllKp"))),
    _loKi(                IScalVal::create(_pLlrfPll->findByName("loPllKi"))),
    _clockKp(             IScalVal::create(_pLlrfPll->findByName("clockPllKp"))),
    _clockKi(             IScalVal::create(_pLlrfPll->findByName("clockPllKi"))),
    // register for LlrfUpConverter
    _upCnv_version( IScalVal_RO::create(_pLlrfUpConverter->findByName("version"))),
    _refPhase0(     IScalVal_RO::create(_pLlrfUpConverter->findByName("referencePhase0"))),
    _refAmp0(       IScalVal_RO::create(_pLlrfUpConverter->findByName("referenceAmplitude0"))),
    _refPhase1(     IScalVal_RO::create(_pLlrfUpConverter->findByName("referencePhase1"))),
    _refAmp1(       IScalVal_RO::create(_pLlrfUpConverter->findByName("referenceAmplitude1"))),
    _rfEnable(      IScalVal::create(_pLlrfUpConverter->findByName("rfEnable"))),
    _modeSel(       IScalVal::create(_pLlrfUpConverter->findByName("modeSelect"))),
    _loSel(         IScalVal::create(_pLlrfUpConverter->findByName("loSelect"))),
    _refChn0(       IScalVal::create(_pLlrfUpConverter->findByName("referenceChannel0"))),
    _refChn1(       IScalVal::create(_pLlrfUpConverter->findByName("referenceChannel1"))),
    _invertCorr(    IScalVal::create(_pLlrfUpConverter->findByName("invertCorrection"))),
    _basebandISet(  IScalVal::create(_pLlrfUpConverter->findByName("basebandISet"))),
    _basebandQSet(  IScalVal::create(_pLlrfUpConverter->findByName("basebandQSet"))),
    _dacOffset(     IScalVal::create(_pLlrfUpConverter->findByName("dacOffset"))),
    _dacMin(        IScalVal::create(_pLlrfUpConverter->findByName("dacMin"))),
    _dacMax(        IScalVal::create(_pLlrfUpConverter->findByName("dacMax"))),
    _correctionGain(IScalVal::create(_pLlrfUpConverter->findByName("correctionGain")))


{
    for(int i = 0; i<MAX_CHN; i++) {
        char name[80];
        sprintf(name, "phase[%d]",i);  _phase[i] = IScalVal_RO::create(_pLlrfDemod->findByName(name));
        sprintf(name, "amp[%d]",  i);  _amp[i]   = IScalVal_RO::create(_pLlrfDemod->findByName(name));
        sprintf(name, "phaseMaxHold[%d]", i);
                                       _phaseMaxHold[i] = IScalVal_RO::create(_pLlrfDemod->findByName(name));
        sprintf(name, "ampMaxHold[%d]", i);
                                      _ampMaxHold[i] = IScalVal_RO::create(_pLlrfDemod->findByName(name));
        sprintf(name, "setI[%d]", i);  _setI[i]  = IScalVal::create(_pLlrfDemod->findByName(name));
        sprintf(name, "setQ[%d]", i);  _setQ[i]  = IScalVal::create(_pLlrfDemod->findByName(name));
    }

}

void CRFCommonFwAdapt::createStream(Path p)
{
    for(int i = 0; i < MAX_STREAM; i++) {
        char stream_name[80];
        sprintf(stream_name, "Stream%d", i); _stream[i] = IStream::create(p->findByName(stream_name));
    }
}

uint32_t CRFCommonFwAdapt::readStream(void *buf, uint64_t size, int idx)
{
    return _stream[idx]->read((uint8_t *) buf, size, CTimeout());
}

void CRFCommonFwAdapt::getDemodVersion(uint32_t *version)
{
    CPSW_TRY_CATCH(_demod_version->getVal(version))
}

void CRFCommonFwAdapt::maxHoldReset(uint32_t val)
{
    CPSW_TRY_CATCH(_maxHoldReset->setVal(val?1:0));
}

void CRFCommonFwAdapt::getPllVersion(uint32_t *version)
{
    CPSW_TRY_CATCH(_pll_version->getVal(version))
}

void CRFCommonFwAdapt::getUpCnvVersion(uint32_t *version)
{
    CPSW_TRY_CATCH(_upCnv_version->getVal(version))
}

double CRFCommonFwAdapt::getPhase(uint32_t *raw_phase, int idx)
{
    double phase;

    CPSW_TRY_CATCH(_phase[idx]->getVal(raw_phase))

    phase = ((double)(short)(*raw_phase))/32767. * 180.;

    return phase;
}

double CRFCommonFwAdapt::getAmp(uint32_t *raw_amp, int idx)
{
    double amp;

    CPSW_TRY_CATCH(_amp[idx]->getVal(raw_amp))
    
    amp = ((double)(short)(*raw_amp))/32767.;

    return amp;
}

double CRFCommonFwAdapt::getMaxPhase(uint32_t *raw_phase, int idx)
{
    double phase;

    CPSW_TRY_CATCH(_phaseMaxHold[idx]->getVal(raw_phase))

    phase = ((double)(short)(*raw_phase))/32767. * 180.;

    return phase;
}

double CRFCommonFwAdapt::getMaxAmp(uint32_t *raw_amp, int idx)
{
    double amp;

    CPSW_TRY_CATCH(_ampMaxHold[idx]->getVal(raw_amp))
    
    amp = ((double)(short)(*raw_amp))/32767.;

    return amp;
}

void CRFCommonFwAdapt::getRotationIQ(uint32_t *i, uint32_t *q, int idx)
{
    CPSW_TRY_CATCH(_setI[idx]->getVal(i))
    CPSW_TRY_CATCH(_setQ[idx]->getVal(q))
}

void CRFCommonFwAdapt::setRotationIQ(uint32_t i, uint32_t q, int idx)
{
    CPSW_TRY_CATCH(_setI[idx]->setVal(i))
    CPSW_TRY_CATCH(_setQ[idx]->setVal(q))
}

void CRFCommonFwAdapt::setRotationPA(double phase, double amp, int idx)
{
    
    phase *= M_PI / 180.;
    amp   *= 16383.;

    
    setRotationIQ((uint32_t) (amp * cos(phase)), (uint32_t) (amp * sin(phase)), idx);
}

double CRFCommonFwAdapt::getLOPhase(uint32_t *raw_phase)
{
    double phase;

    CPSW_TRY_CATCH(_loPhase->getVal(raw_phase))

    phase = ((double)(short)(*raw_phase))/32767. * 180.;

    return phase;
}

double CRFCommonFwAdapt::getLOAmp(uint32_t *raw_amp)
{
    double amp;

    CPSW_TRY_CATCH(_loAmp->getVal(raw_amp))

    amp = ((double)(short)(*raw_amp))/32767.;

    return amp;
}

void CRFCommonFwAdapt::getLOLockCount(uint32_t *count)
{
    CPSW_TRY_CATCH(_loLockCount->getVal(count))

}

void CRFCommonFwAdapt::getLOLockStatus(uint32_t *locked)
{
    CPSW_TRY_CATCH(_loLocked->getVal(locked))
}

void CRFCommonFwAdapt::getLOLossStatus(uint32_t *loss)
{
    CPSW_TRY_CATCH(_loLoss->getVal(loss))
}

double CRFCommonFwAdapt::getClockPhase(uint32_t *raw_phase)
{
    double phase;

    CPSW_TRY_CATCH(_clockPhase->getVal(raw_phase))

    phase = ((double)(short)(*raw_phase))/32767. * 180.;

    return phase;
}

double CRFCommonFwAdapt::getClockAmp(uint32_t *raw_amp)
{
    double amp;

    CPSW_TRY_CATCH(_clockAmp->getVal(raw_amp))

    amp = ((double)(short)(*raw_amp))/32767.;

    return amp;
}



void CRFCommonFwAdapt::getClockLockCount(uint32_t *count)
{
    CPSW_TRY_CATCH(_clockLockCount->getVal(count))
}

void CRFCommonFwAdapt::getClockLockStatus(uint32_t *locked)
{
    CPSW_TRY_CATCH(_clockLocked->getVal(locked))
}

void CRFCommonFwAdapt::getClockLossStatus(uint32_t *loss)
{
    CPSW_TRY_CATCH(_clockLoss->getVal(loss))
}


void CRFCommonFwAdapt::getLockThreshold(uint32_t *val)
{
    CPSW_TRY_CATCH(_lockThreshold->getVal(val))
}

void CRFCommonFwAdapt::setLockThreshold(uint32_t val)
{
    CPSW_TRY_CATCH(_lockThreshold->setVal(val))
}

void CRFCommonFwAdapt::getLossThreshold(uint32_t *val)
{
    CPSW_TRY_CATCH(_lossThreshold->getVal(val))
}

void CRFCommonFwAdapt::setLossThreshold(uint32_t val)
{
    CPSW_TRY_CATCH(_lossThreshold->setVal(val))
}

void CRFCommonFwAdapt::resetLOLockCount()
{
    CPSW_TRY_CATCH(_resetLOLockCount->execute())
}

void CRFCommonFwAdapt::resetClockLockCount()
{
    CPSW_TRY_CATCH(_resetClockLockCount->execute())
}


void CRFCommonFwAdapt::loDacControlMux(uint32_t val)
{
    CPSW_TRY_CATCH(_loDacControlMux->setVal(val))
}

void CRFCommonFwAdapt::clockDacControlMux(uint32_t val)
{
    CPSW_TRY_CATCH(_clockDacControlMux->setVal(val))
}

void CRFCommonFwAdapt::userDacControlMux(uint32_t val)
{
    CPSW_TRY_CATCH(_userDacControlMux->setVal(val))
}


void CRFCommonFwAdapt::loReferenceSelect(uint32_t val)
{
    CPSW_TRY_CATCH(_loRefSel->setVal(val))
}


void CRFCommonFwAdapt::clockReferenceSelect(uint32_t val)
{
    CPSW_TRY_CATCH(_clockRefSel->setVal(val))
}


void CRFCommonFwAdapt::loPolarity(uint32_t val)
{
    CPSW_TRY_CATCH(_loPolarity->setVal(val))
}


void CRFCommonFwAdapt::clockPolarity(uint32_t val)
{

    CPSW_TRY_CATCH(_clockPolarity->setVal(val))
}



void CRFCommonFwAdapt::setLODac(uint32_t val)
{
    CPSW_TRY_CATCH(_loDacSet->setVal(val))
}

void CRFCommonFwAdapt::setClockDac(uint32_t val)
{
    CPSW_TRY_CATCH(_clockDacSet->setVal(val))
}

void CRFCommonFwAdapt::setUserDac(uint32_t val)
{
    CPSW_TRY_CATCH(_userDacSet->setVal(val))
}

void CRFCommonFwAdapt::setLOPhase(uint32_t val)
{
    CPSW_TRY_CATCH(_loPhaseSet->setVal(val))
}


void CRFCommonFwAdapt::setClockPhase(uint32_t val)
{
    CPSW_TRY_CATCH(_clockPhaseSet->setVal(val))
}



void CRFCommonFwAdapt::setLOKp(uint32_t val)
{
    CPSW_TRY_CATCH(_loKp->setVal(val))
}


void CRFCommonFwAdapt::setLOKi(uint32_t val)
{
    CPSW_TRY_CATCH(_loKi->setVal(val))
}


void CRFCommonFwAdapt::setClockKp(uint32_t val)
{
    CPSW_TRY_CATCH(_clockKp->setVal(val))
}

void CRFCommonFwAdapt::setClockKi(uint32_t val)
{
    CPSW_TRY_CATCH(_clockKi->setVal(val))
}


double CRFCommonFwAdapt::getRefPhase(uint32_t *raw_phase, int idx)
{
    double phase;

    try {
        switch(idx) {
            case 0:
                _refPhase0->getVal(raw_phase);
                break;
            case 1:
                _refPhase1->getVal(raw_phase);
                break;
            default:
                *raw_phase = 0;
                return 0.;
        }
    } catch (CPSWError &e) {
        fprintf(stderr, "CPSW Error: %s at %s, line %d\n", e.getInfo().c_str(), __FILE__, __LINE__);
        throw e;
    }

    phase = ((double)(short)(*raw_phase))/32767. * 180.;


    return phase;
}

double CRFCommonFwAdapt::getRefAmp(uint32_t *raw_amp, int idx)
{

    double amp;
    try {
        switch(idx) {
            case 0:
                _refAmp0->getVal(raw_amp);
                break;
            case 1:
                _refAmp1->getVal(raw_amp);
                break;
            default:
                *raw_amp = 0;
                return 0.;
        }
    } catch (CPSWError &e) {
        fprintf(stderr, "CPSW Error: %s at %s, line %d\n", e.getInfo().c_str(), __FILE__, __LINE__);
        throw e;
    }
    
    amp = ((double)(short)(*raw_amp))/32767.;

    return amp;
}


void CRFCommonFwAdapt::rfEnable(uint32_t val)
{
    CPSW_TRY_CATCH(_rfEnable->setVal(val?1:0))
}

void CRFCommonFwAdapt::modeSelect(uint32_t val)
{
    CPSW_TRY_CATCH(_modeSel->setVal(val?1:0))
}

void CRFCommonFwAdapt::loSelect(uint32_t val)
{
    CPSW_TRY_CATCH(_loSel->setVal(val?1:0))
}

void CRFCommonFwAdapt::refChnSelect(uint32_t val, int idx)
{
    try {
        switch(idx) {
            case 0:
                _refChn0->setVal(val);
                break;
            case 1:
                _refChn1->setVal(val);
                break;
            default: break;
        }
    } catch (CPSWError &e) {
        fprintf(stderr, "CPSW Error: %s at %s, line %d\n", e.getInfo().c_str(), __FILE__, __LINE__);
        throw e;
    }
}


void CRFCommonFwAdapt::invertCorrection(uint32_t val)
{
    CPSW_TRY_CATCH(_invertCorr->setVal(val?1:0))
}


void CRFCommonFwAdapt::getBasebandIQ(uint32_t *i, uint32_t *q)
{
    CPSW_TRY_CATCH(_basebandISet->getVal(i))
    CPSW_TRY_CATCH(_basebandQSet->getVal(q))
}

void CRFCommonFwAdapt::setBasebandIQ(uint32_t i, uint32_t q)
{
    CPSW_TRY_CATCH(_basebandISet->setVal(i))
    CPSW_TRY_CATCH(_basebandQSet->setVal(q))
}

void CRFCommonFwAdapt::setBasebandPA(double phase, double amp)
{
    phase *= M_PI / 180.;
    amp   *= 16383.;

    
    setBasebandIQ((uint32_t) (amp * cos(phase)), (uint32_t) (amp * sin(phase)));
}


void CRFCommonFwAdapt::setDacOffset(uint32_t val)
{
    CPSW_TRY_CATCH(_dacOffset->setVal(val))
}

void CRFCommonFwAdapt::setDacMin(uint32_t val)
{
    CPSW_TRY_CATCH(_dacMin->setVal(val))
}

void CRFCommonFwAdapt::setDacMax(uint32_t val)
{
    CPSW_TRY_CATCH(_dacMax->setVal(val))
}

void CRFCommonFwAdapt::setCorrectionGain(uint32_t val)
{
    CPSW_TRY_CATCH(_correctionGain->setVal(val))
}


