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

#include <MWorksCore/Component.h>
#include <MWorksCore/GenericVariable.h>

using namespace mw;


class EyeCalibrator3D : public mw::Component {

public:
    EyeCalibrator3D(const std::string &tag, shared_ptr<Variable> anotherAttribute);

    virtual ~EyeCalibrator3D();
    
private:
    EyeCalibrator3D(const EyeCalibrator3D &other);
    
    shared_ptr<Variable> anotherAttribute;

};


class EyeCalibrator : public Calibrator {
    
protected:
    int inputIndexH, inputIndexV, outputIndexH, outputIndexV;
    int HfunctionIndex, VfunctionIndex;
    MWTime HsampleTime;
    MWTime lastHtimeUS;
    MWTime nextTimeToWarnUS;
    PairedEyeData *pairedEyeData;
    
    Datum desiredH, desiredV;
    Datum sampledH, sampledV;
    Datum calibratedH, calibratedV;
    virtual void announceCalibrationUpdate();
    virtual void setPrivateParameters(); 
    virtual void announceCalibrationSample(int outputIndex, Datum SampledData, 
                                           Datum DesiredOutputData, Datum CalibratedOutputData, MWTime timeOfSampleUS);
    virtual void tryToUseDataToSetParameters(Datum original_data);
    
    
public:
    EyeCalibrator(std::string _tag, shared_ptr<Variable> _eyeHraw, shared_ptr<Variable> _eyeVraw,
                  shared_ptr<Variable> _eyeHcalibrated, shared_ptr<Variable> _eyeVcalibrated, const int order=2);
    virtual ~EyeCalibrator();
    virtual void notifyRequest(const Datum& original_data, MWTime timeUS);
    virtual void notifyPrivate(const Datum& original_data, MWTime timeUS);
    
    // override base class so that we can wait for paired samples
    virtual void newDataReceived(int inputIndex, const Datum& data, 
                                 MWTime timeUS);
    
};


#endif
