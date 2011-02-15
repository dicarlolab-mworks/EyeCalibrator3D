/*
 *  EyeCalibrator3D.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "EyeCalibrator3D.h"

#define CALIBRATOR_PARAMS_D "params_D"
#define R_CALIBRATOR_PARAMS_D "params_D"

#define VERBOSE_EYE_CALIBRATORS 0


// here we define a particular calibrator class by 
//  1) register a particular set of input and out vars
//  2) define a fittable function that is meant to calibrate between them
//  
// in principle, one can make a class just like this one for ANY set of vars
// and ANY fitable funciton (as long as the fitable function class is also written)
EyeCalibrator3D::EyeCalibrator3D(const std::string &_tag,
                                 shared_ptr<Variable> _eyeHraw,
                                 shared_ptr<Variable> _eyeVraw,
                                 shared_ptr<Variable> _eyeDraw,
                                 shared_ptr<Variable> _eyeHcalibrated,
                                 shared_ptr<Variable> _eyeVcalibrated,
                                 shared_ptr<Variable> _eyeDcalibrated,
                                 int order) :
    Calibrator(_tag)
{
    
    if (VERBOSE_EYE_CALIBRATORS) mprintf("mEyeCalibrator3D constructor has been called.");
    
    // 1)  register inputs and outputs
    inputIndexH = (this->registerInput(_eyeHraw));
    inputIndexV = (this->registerInput(_eyeVraw));
    inputIndexD = (this->registerInput(_eyeDraw));
    outputIndexH = (this->registerOutput(_eyeHcalibrated));
    outputIndexV = (this->registerOutput(_eyeVcalibrated));
    outputIndexD = (this->registerOutput(_eyeDcalibrated));
    this->initialize();     // create memory for the expected data, averagers, and fitable functions
    
    // 2) create and assign a specific fitableFunction for each output 
    shared_ptr<FitableFunction> fitableFunctionH;
    shared_ptr<FitableFunction> fitableFunctionV;
    shared_ptr<FitableFunction> fitableFunctionD;
    //  In this case, they are identical, but they do not need to be the same. (any fitable function object will work here)
    // default values for parameters are established in here.
    switch(order) {
        case 1:
            fitableFunctionH = shared_ptr<FitableFunction>(new FirstOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexH)); 
            fitableFunctionV = shared_ptr<FitableFunction>(new FirstOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexV)); 
            fitableFunctionD = shared_ptr<FitableFunction>(new FirstOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexD)); 
            break;
        case 2:
            fitableFunctionH = shared_ptr<FitableFunction>(new SecondOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexH)); 
            fitableFunctionV = shared_ptr<FitableFunction>(new SecondOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexV)); 
            fitableFunctionD = shared_ptr<FitableFunction>(new SecondOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexD)); 
            break;
        default:
            merror(M_PARSER_MESSAGE_DOMAIN,"%s has an unsupported order (%d)", _tag.c_str(), order);
            break;
    }
    
    HfunctionIndex = (fitableFunctions->addReference(outputIndexH, fitableFunctionH))-1;
    VfunctionIndex = (fitableFunctions->addReference(outputIndexV, fitableFunctionV))-1;
    DfunctionIndex = (fitableFunctions->addReference(outputIndexD, fitableFunctionD))-1;
    
    //     // 2) create and assign a specific fitableFunction for each output 
    //    //  In this case, they are identical, but they do not need to be the same. (any fitable function object will work here)
    //    // default values for parameters are established in here.
    //    // first output (h)
    //    shared_ptr<FitableFunction> fitableFunctionH(new SecondOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexH)); 
    //    HfunctionIndex = (fitableFunctions->addReference(outputIndexH, fitableFunctionH))-1;
    //
    //    // second output (v)
    //    shared_ptr<FitableFunction> fitableFunctionV(new SecondOrderPolynomialFitableFunction(this->getNumInputs(),true,inputIndexV)); 
    //    VfunctionIndex = (fitableFunctions->addReference(outputIndexV, fitableFunctionV))-1;
    
    // make sure fitable functions parameters are set to defaults (they will be, but nice form)           
    this->setParametersToDefaults();            
    
    // 3) if the private variable was found, then we should use its values to set the parameters (i.e. replace defaults)
    if(privateVariableNameAlreadyInstantiated)    {
        tryToUseDataToSetParameters(privateVariable->getValue()); 
    } 
    
    // 4) announce our current paramters  (JJD added Sept 13, 2006)
    announceCalibrationUpdate();
    
    
    // the eye calibrator uses this object to buffer and pair eye samples together.
    int buffer_size = M_ASSUMED_EYE_SAMPLES_PER_MS * M_MAX_EYE_BUFFER_SIZE_MS; 
    pairedEyeData = new PairedEyeData(buffer_size);
    
    
}




EyeCalibrator3D::~EyeCalibrator3D() {
    delete fitableFunctions;
    delete pairedEyeData;
}


// JJD overrode the base class function on Nov 2, 2006, so that the eye calibrator 
// will wait for paired input from BOTH channels before posting
void EyeCalibrator3D::newDataReceived(int inputIndex, const Datum& data, 
                                    MWTime timeUS)
{
    Locker lock(*this);
    
    if (!initialized){
        return;
    }
    
    
    //pUncalibratedData->setElement(inputIndex,data);   // this is used for filtering
    pSampledData->setElement(inputIndex,data);        // this is what is used for calibration samples
    timeOfMostRecentUncalibratedDataUS = timeUS;
    
    // if the averager is running, take the sample  
    // (this happens in the background -- the averager is instatiated when this object is instantiated)
    
    
    // check to see if we have a correct pair of values that we should compute calibration on
    // process and post all such pairs now
    
    if (inputIndex==inputIndexH) {
        pairedEyeData->putDataH(data, timeUS);
    }
    else if (inputIndex==inputIndexV) {
        pairedEyeData->putDataV(data, timeUS);
    }
    else {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 " **** EyeStatusMonitor::newDataReceived  Unknown input index");
        return;
    }
    
    
    
    // if a pair is ready, pull it out and process it
    MWTime eyeTimeUS;
    double eyeH, eyeV;
    bool noErr = true;
    Datum calibData;
    
    while (pairedEyeData->getAvailable(&eyeH, &eyeV, &eyeTimeUS)) {
        
        // put the paired values in the input vector for the calibration function
        pUncalibratedData->setElement(inputIndexH,(Datum)eyeH);
        pUncalibratedData->setElement(inputIndexV,(Datum)eyeV);
        
        
        for (int outputIndex=0;outputIndex<this->getNumOutputs();outputIndex++) {
            noErr = (fitableFunctions->getElement(outputIndex))->
            applyTheFunction(*pUncalibratedData, &calibData);
            if (noErr) {
                postResults(outputIndex, calibData, timeUS);               
                pCalibratedData->setElement(outputIndex,calibData);   // keep a copy of the last outs
            }
        }
    }
}




// method to announce details about each sample that is acquired for calibration  
void EyeCalibrator3D::announceCalibrationSample(int outputIndex, Datum SampledData, 
                                              Datum DesiredOutputData, Datum CalibratedOutputData, MWTime timeOfSampleUS) {
    
    // this method expects the H sample to arrive first and then the V sample
    if (outputIndex == outputIndexH) {  // store data and wait for v (announce as pair)
        desiredH = DesiredOutputData;
        calibratedH = CalibratedOutputData;
        sampledH = (&SampledData)->getElement(inputIndexH);
        HsampleTime = timeOfSampleUS;
        return;
    }
    if (outputIndex == outputIndexV) {  // store data and wait for v (announce as pair)
        desiredV = DesiredOutputData;
        calibratedV = CalibratedOutputData;
        sampledV = (&SampledData)->getElement(inputIndexV);
        if ( (abs(timeOfSampleUS-HsampleTime)) > 10000) {
            mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                     "Calibrator sample announce detected large time differential between h and v samples.  Values likely inaccurate.");
        }
    }
    
    
    Datum announceData(M_DICTIONARY, 5);
    announceData.addElement(CALIBRATOR_NAME,uniqueCalibratorName);    // char
    announceData.addElement(CALIBRATOR_ACTION,CALIBRATOR_ACTION_SAMPLE);
    
    Datum temp(M_LIST, 3);
    temp.setElement(0,sampledH);
    temp.setElement(1,sampledV);
    temp.setElement(2,sampledD);
    announceData.addElement(CALIBRATOR_SAMPLE_SAMPLED_HV,temp);    // input values
    
    temp.setElement(0,desiredH);
    temp.setElement(1,desiredV);
    temp.setElement(2,desiredD);
    announceData.addElement(CALIBRATOR_SAMPLE_DESIRED_HV,temp);    // gold standard values
    
    temp.setElement(0,calibratedH);
    temp.setElement(1,calibratedV);
    temp.setElement(2,calibratedD);
    announceData.addElement(CALIBRATOR_SAMPLE_CALIBRATED_HV,temp); // values produced from input values using current calibration
    
    //announceData.addElement("JJDtest",desiredH); // values produced from input values using current calibration
    
    if (VERBOSE_EYE_CALIBRATORS) mprintf("mCalibrator::announceCalibrationSample  Announcing now");
    announce(announceData);    // announce things here using method from Announcable
    
}


void EyeCalibrator3D::announceCalibrationUpdate() {
    
    Datum announceData(M_DICTIONARY, 5);
    announceData.addElement(CALIBRATOR_NAME,uniqueCalibratorName);
    announceData.addElement(CALIBRATOR_ACTION,CALIBRATOR_ACTION_UPDATE_PARAMS);
    
    Datum paramsH = (fitableFunctions->getElement(HfunctionIndex))->getParameters();
    announceData.addElement(CALIBRATOR_PARAMS_H,paramsH);   // M_LIST
    
    Datum paramsV = (fitableFunctions->getElement(VfunctionIndex))->getParameters();
    announceData.addElement(CALIBRATOR_PARAMS_V,paramsV);   // M_LIST
    
    Datum paramsD = (fitableFunctions->getElement(DfunctionIndex))->getParameters();
    announceData.addElement(CALIBRATOR_PARAMS_D,paramsD);   // M_LIST
    
    announce(announceData);    // announce things here using method from Announcable (will set values in announce variable)
    
}

void EyeCalibrator3D::setPrivateParameters() { 
    
    Datum privateData(M_DICTIONARY, 3);
    
    Datum paramsH = (fitableFunctions->getElement(HfunctionIndex))->getParameters();
    privateData.addElement(R_CALIBRATOR_PARAMS_H,paramsH);   // M_LIST
    
    Datum paramsV = (fitableFunctions->getElement(VfunctionIndex))->getParameters();
    privateData.addElement(R_CALIBRATOR_PARAMS_V,paramsV);   // M_LIST
    
    Datum paramsD = (fitableFunctions->getElement(DfunctionIndex))->getParameters();
    privateData.addElement(R_CALIBRATOR_PARAMS_D,paramsD);   // M_LIST
    
    storePrivateData(privateData);   // base class method 
    
    // updated Feb 2007  -- DDC may suggest a better way, but works for now
    // Two potential problems that are delt with by the PrivateStorable base class:
    // 1) if we load saved data (in a private variable), then this causes us to come in on a notification
    //      in that case, we will NOT update the private variable (because the VARIABLE is locked and we don't need to update it!)
    //      (but, no matter what, we will keep track of the private variable values (in the PrivateStorable class) to deal with the following problem:
    // 2) if we change the value of the eye calibrator data by any other method, we will update the private variable
    //      and that will trigger a notification back to us, but we will IGNORE that notification (because the CALIBRATOR is locked and we don't need it!)
    //
    
}

// PUBLIC METHOD
// triggered by any change to the calibrator request variable  
void EyeCalibrator3D::notifyRequest(const Datum& dictionaryData, MWTime timeUS) {
    
    Locker lock(*this);
    
    // check if this calibrator should respond
    bool validRequest = checkRequest(dictionaryData);     // base class method (minimal checking)
    if (!validRequest){
        return;                          // not meant for this calibrator or improperly formatted
    }
    
    // determine the requested action
    
    CalibratorRequestedAction requestedAction = getRequestedAction(dictionaryData);  // overridable base class method
    
    switch (requestedAction) {
            
        case CALIBRATOR_NO_ACTION:
            mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                     "Request sent to calibrator %s resulted in no action.", uniqueCalibratorName.c_str());
            return;
            break;
            
        case CALIBRATOR_ACTION_SET_PARAMS_TO_DEFAULTS:
            //mwarning(M_SYSTEM_MESSAGE_DOMAIN,
            //    "Request to update params sent to calibrator %s, BUT METHOD NOT YET WRITTEN. Request ignored.", uniqueCalibratorName);
            if (VERBOSE_EYE_CALIBRATORS) mprintf("An eye calibrator is processing a request to set its parameters to defaults."); 
            setParametersToDefaults(); 
            break;
            break;
            
        case CALIBRATOR_ACTION_SET_PARAMS_TO_CONTAINED:
            if (VERBOSE_EYE_CALIBRATORS) mprintf("An eye calibrator is processing a request to set its parameters to specific values."); 
            tryToUseDataToSetParameters(dictionaryData);      
            break;
            
    }
    
}

// PUBLIC METHOD -- this means the private variable is locked -- DO NOT UPDATE IT!
void EyeCalibrator3D::notifyPrivate(const Datum& dictionaryData, MWTime timeUS) {
    Locker lock(*this);
    tryToUseDataToSetParameters(dictionaryData); 
}



// this routine handles both "requests" and loading of private data (stored params)
// if a request, then priuvate values are probably in need of update
// if a load of private, then private values are OK, but I can check this.
void EyeCalibrator3D::tryToUseDataToSetParameters(Datum dictionaryData) {
    
    // check if this is a dictionary
    if (!(dictionaryData.getDataType() == M_DICTIONARY)) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed by calibrator %s that was not expected dictionary type was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    
    // try to perform the requested action
    bool paramsChanged = false; 
    Datum paramData;
    
    // if appropriate param fields are present and have expected length, then use the data 
    //     to try to update the parameters
    
    //  H params ================================================
    if (!(dictionaryData.hasKey(R_CALIBRATOR_PARAMS_H))) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s without proper params filed was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    paramData = dictionaryData.getElement(R_CALIBRATOR_PARAMS_H);
    
    // check if vector and correct length
    if  (paramData.getDataType() != M_LIST) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s that did not contain vector in params field was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    Datum paramsH = paramData;
    if (paramsH.getNElements() != (fitableFunctions->getElement(HfunctionIndex))->getNumParameters() ) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s that did not contain expected number of params was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    bool noErr = (fitableFunctions->getElement(HfunctionIndex))->setParameters(paramsH); 
    if (noErr) paramsChanged = true;     // params have been updated
    
    
    //  V params ================================================
    if (!(dictionaryData.hasKey(R_CALIBRATOR_PARAMS_V))) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s without proper params filed was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    paramData = dictionaryData.getElement(R_CALIBRATOR_PARAMS_V);
    
    // check if vector and correct length
    if  (paramData.getDataType() != M_LIST) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s that did not contain vector in params field was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    Datum paramsV = paramData;
    if (paramsV.getNElements() != (fitableFunctions->getElement(VfunctionIndex))->getNumParameters() ) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s that did not contain expected number of params was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    noErr = (fitableFunctions->getElement(VfunctionIndex))->setParameters(paramsV); 
    if (noErr) paramsChanged = true;     // params have been updated
    
    
    //  D params ================================================
    if (!(dictionaryData.hasKey(R_CALIBRATOR_PARAMS_D))) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s without proper params filed was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    paramData = dictionaryData.getElement(R_CALIBRATOR_PARAMS_D);
    
    // check if vector and correct length
    if  (paramData.getDataType() != M_LIST) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s that did not contain vector in params field was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    Datum paramsD = paramData;
    if (paramsD.getNElements() != (fitableFunctions->getElement(DfunctionIndex))->getNumParameters() ) {
        mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                 "Data processed to update params of calibrator %s that did not contain expected number of params was ignored.", uniqueCalibratorName.c_str());
        return;
    }
    noErr = (fitableFunctions->getElement(DfunctionIndex))->setParameters(paramsD); 
    if (noErr) paramsChanged = true;     // params have been updated
    
    // if any params were updated, announce the full set of current parameters
    // if this change was triggered by an external change to the private variable, then we do not need to update that private variable (and we cannot anyway --  it is locked)
    if (paramsChanged) reportParameterUpdate();
    
}






















