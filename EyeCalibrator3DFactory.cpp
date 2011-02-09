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
    REQUIRE_ATTRIBUTES(parameters, "tag", "eyeh_raw", "eyeh_calibrated", "eyev_raw", "eyev_calibrated");
    
    std::string tagname(parameters["tag"]);

    shared_ptr<Variable> eyeh_raw(reg->getVariable(parameters["eyeh_raw"]));
    shared_ptr<Variable> eyev_raw(reg->getVariable(parameters["eyev_raw"]));
    shared_ptr<Variable> eyeh_calibrated(reg->getVariable(parameters["eyeh_calibrated"]));
    shared_ptr<Variable> eyev_calibrated(reg->getVariable(parameters["eyev_calibrated"]));
    
    CHECK_ATTRIBUTE(eyeh_raw, parameters, "eyeh_raw");
    CHECK_ATTRIBUTE(eyev_raw, parameters, "eyev_raw");
    CHECK_ATTRIBUTE(eyeh_calibrated, parameters, "eyeh_calibrated");
    CHECK_ATTRIBUTE(eyev_calibrated, parameters, "eyev_calibrated");
    
    shared_ptr<EyeCalibrator3D> newEyeCalibrator3D(new EyeCalibrator3D(tagname,
                                                                       eyeh_raw,
                                                                       eyev_raw,
                                                                       eyeh_calibrated,
                                                                       eyev_calibrated,
                                                                       2));

    return newEyeCalibrator3D;
}






















