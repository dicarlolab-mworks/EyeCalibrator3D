/*
 *  EyeCalibrator3DFactory.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include <MWorksCore/ComponentRegistry.h>

#include "EyeCalibrator3DFactory.h"
#include "EyeCalibrator3D.h"

using namespace mw;


shared_ptr<mw::Component> EyeCalibrator3DFactory::createObject(std::map<std::string, std::string> parameters,
                                                               ComponentRegistry *reg)
{
    const char* TAG = "tag";
    const char* EYEH_RAW = "eyeh_raw";
    const char* EYEV_RAW = "eyev_raw";
    const char* EYED_RAW = "eyed_raw";
    const char* EYEH_CALIBRATED = "eyeh_calibrated";
    const char* EYEV_CALIBRATED = "eyev_calibrated";
    const char* EYED_CALIBRATED = "eyed_calibrated";

    REQUIRE_ATTRIBUTES(parameters,
                       TAG,
                       EYEH_RAW,
                       EYEV_RAW,
                       EYED_RAW,
                       EYEH_CALIBRATED,
                       EYEV_CALIBRATED,
                       EYED_CALIBRATED);
    
    std::string tag(parameters[TAG]);

    shared_ptr<Variable> eyeh_raw(reg->getVariable(parameters[EYEH_RAW]));
    shared_ptr<Variable> eyev_raw(reg->getVariable(parameters[EYEV_RAW]));
    shared_ptr<Variable> eyed_raw(reg->getVariable(parameters[EYED_RAW]));
    shared_ptr<Variable> eyeh_calibrated(reg->getVariable(parameters[EYEH_CALIBRATED]));
    shared_ptr<Variable> eyev_calibrated(reg->getVariable(parameters[EYEV_CALIBRATED]));
    shared_ptr<Variable> eyed_calibrated(reg->getVariable(parameters[EYED_CALIBRATED]));
    
    CHECK_ATTRIBUTE(eyeh_raw, parameters, EYEH_RAW);
    CHECK_ATTRIBUTE(eyev_raw, parameters, EYEV_RAW);
    CHECK_ATTRIBUTE(eyed_raw, parameters, EYED_RAW);
    CHECK_ATTRIBUTE(eyeh_calibrated, parameters, EYEH_CALIBRATED);
    CHECK_ATTRIBUTE(eyev_calibrated, parameters, EYEV_CALIBRATED);
    CHECK_ATTRIBUTE(eyed_calibrated, parameters, EYED_CALIBRATED);
    
    shared_ptr<EyeCalibrator3D> newEyeCalibrator3D(new EyeCalibrator3D(tag,
                                                                       eyeh_raw,
                                                                       eyev_raw,
                                                                       eyed_raw,
                                                                       eyeh_calibrated,
                                                                       eyev_calibrated,
                                                                       eyed_calibrated,
                                                                       order));

    return newEyeCalibrator3D;
}






















