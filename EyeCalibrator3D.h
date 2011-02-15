/*
 *  EyeCalibrator3D.h
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef EyeCalibrator3D_H_
#define EyeCalibrator3D_H_

#include <MWorksCore/EyeCalibrators.h>

using namespace mw;


class EyeCalibrator3D : public Calibrator {
    
protected:
    int inputIndexH, inputIndexV, inputIndexD;
    int outputIndexH, outputIndexV, outputIndexD;
    int HfunctionIndex, VfunctionIndex, DfunctionIndex;
    MWTime HsampleTime;
    PairedEyeData *pairedEyeData;
    
    Datum desiredH, desiredV, desiredD;
    Datum sampledH, sampledV, sampledD;
    Datum calibratedH, calibratedV, calibratedD;

    virtual void announceCalibrationUpdate();
    virtual void setPrivateParameters(); 
    virtual void announceCalibrationSample(int outputIndex, Datum SampledData, 
                                           Datum DesiredOutputData, Datum CalibratedOutputData, MWTime timeOfSampleUS);
    virtual void tryToUseDataToSetParameters(Datum original_data);
    
    
public:
    EyeCalibrator3D(const std::string &_tag,
                    shared_ptr<Variable> _eyeHraw,
                    shared_ptr<Variable> _eyeVraw,
                    shared_ptr<Variable> _eyeDraw,
                    shared_ptr<Variable> _eyeHcalibrated,
                    shared_ptr<Variable> _eyeVcalibrated,
                    shared_ptr<Variable> _eyeDcalibrated,
                    int order = 2);
    virtual ~EyeCalibrator3D();
    virtual void notifyRequest(const Datum& original_data, MWTime timeUS);
    virtual void notifyPrivate(const Datum& original_data, MWTime timeUS);
    
    // override base class so that we can wait for paired samples
    virtual void newDataReceived(int inputIndex, const Datum& data, 
                                 MWTime timeUS);
    
private:
    EyeCalibrator3D(const EyeCalibrator3D &other);
    
};


#endif























