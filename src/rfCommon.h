#ifndef _RF_COMMON_FW_H
#define _RF_COMMON_FW_H

#include <cpsw_api_user.h>
#include <cpsw_api_builder.h>


class IRFCommonFw;
typedef shared_ptr<IRFCommonFw> RFCommonFw;

class IRFCommonFw : public virtual IEntry {
public:
    static RFCommonFw create(Path p);
    virtual void createStream(Path p)                                  = 0;
    virtual uint32_t readStream(void *buf, uint64_t size, int idx)     = 0;
    virtual void getDemodVersion(uint32_t *version)                    = 0;
    virtual void maxHoldReset(uint32_t val)                            = 0;
    virtual double getPhase(uint32_t *raw_phase, int idx)              = 0;
    virtual double getAmp(uint32_t *raw_amp, int idx)                  = 0;
    virtual double getMaxPhase(uint32_t *raw_phase, int idx)           = 0;
    virtual double getMaxAmp(uint32_t *raw_amp, int idx)               = 0;
    virtual void getRotationIQ(uint32_t *i, uint32_t *q, int idx)      = 0;
    virtual void setRotationIQ(uint32_t i,  uint32_t  q, int idx)      = 0;
    virtual void setRotationPA(double phase, double amp, int idx)      = 0;
    virtual void getPllVersion(uint32_t *version)                      = 0;
    virtual double getLOPhase(uint32_t *raw_phase)                     = 0;
    virtual double getLOAmp(uint32_t *raw_amp)                         = 0;
    virtual void getLOLockCount(uint32_t *count)                       = 0;
    virtual void getLOLockStatus(uint32_t *locked)                     = 0;
    virtual void getLOLossStatus(uint32_t *loss)                       = 0;
    virtual double getClockPhase(uint32_t *raw_phase)                  = 0;
    virtual double getClockAmp(uint32_t *raw_amp)                      = 0;
    virtual void getClockLockCount(uint32_t *count)                    = 0;
    virtual void getClockLockStatus(uint32_t *locked)                  = 0;
    virtual void getClockLossStatus(uint32_t *loss)                    = 0;
    virtual void getLockThreshold(uint32_t *val)                       = 0;
    virtual void setLockThreshold(uint32_t  val)                       = 0;
    virtual void getLossThreshold(uint32_t *val)                       = 0;
    virtual void setLossThreshold(uint32_t  val)                       = 0;
    virtual void loLockCountReset(uint32_t  val)                       = 0;
    virtual void clockLockCountReset(uint32_t val)                     = 0;
    virtual void loDacControlMux(uint32_t val)                         = 0;
    virtual void clockDacControlMux(uint32_t val)                      = 0;
    virtual void userDacControlMux(uint32_t val)                       = 0;
    virtual void loReferenceSelect(uint32_t val)                       = 0;
    virtual void clockReferenceSelect(uint32_t val)                    = 0;
    virtual void loPolarity(uint32_t val)                              = 0;
    virtual void clockPolarity(uint32_t val)                           = 0;
    virtual void setLODac(uint32_t val)                                = 0;
    virtual void setClockDac(uint32_t val)                             = 0;
    virtual void setUserDac(uint32_t val)                              = 0;
    virtual void setLOPhase(uint32_t val)                              = 0;
    virtual void setClockPhase(uint32_t val)                           = 0;
    virtual void setLOKp(uint32_t val)                                 = 0;
    virtual void setLOKi(uint32_t val)                                 = 0;
    virtual void setClockKp(uint32_t val)                              = 0;
    virtual void setClockKi(uint32_t val)                              = 0;
    virtual void getUpCnvVersion(uint32_t *version)                    = 0;
    virtual double getRefPhase(uint32_t *raw_phase, int idx)           = 0;
    virtual double getRefAmp(uint32_t *raw_amp, int idx)               = 0;
    virtual void rfEnable(uint32_t val)                                = 0;
    virtual void modeSelect(uint32_t val)                              = 0;
    virtual void loSelect(uint32_t val)                                = 0;
    virtual void refChnSelect(uint32_t val, int idx)                   = 0;
    virtual void invertCorrection(uint32_t val)                        = 0;
    virtual void getBasebandIQ(uint32_t *i, uint32_t *q)               = 0;
    virtual void setBasebandIQ(uint32_t i, uint32_t q)                 = 0;
    virtual void setBasebandPA(double pahse, double amplitude)         = 0;
    virtual void setDacOffset(uint32_t val)                            = 0;
    virtual void setDacMin(uint32_t val)                               = 0;
    virtual void setDacMax(uint32_t val)                               = 0;

};

#endif  /* _RF_COMMON_FW_H */
