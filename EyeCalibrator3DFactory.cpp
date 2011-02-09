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
    const char* ANOTHER_ATTRIBUTE = "another_attribute";

    REQUIRE_ATTRIBUTES(parameters,
                       TAG,
                       ANOTHER_ATTRIBUTE);
    
    std::string tag(parameters[TAG]);
    
    shared_ptr<Variable> anotherAttribute(reg->getVariable(parameters[ANOTHER_ATTRIBUTE]));
    CHECK_ATTRIBUTE(anotherAttribute, parameters, ANOTHER_ATTRIBUTE);
    
    shared_ptr<EyeCalibrator3D> newComponent(new EyeCalibrator3D(tag,
                                                                                             anotherAttribute));
    
    return newComponent;
}


shared_ptr<mw::Component> EyeCalibratorFactory::createObject(std::map<std::string, std::string> parameters,
                                                             ComponentRegistry *reg) {
    REQUIRE_ATTRIBUTES(parameters, "tag", "eyeh_raw", "eyeh_calibrated", "eyev_raw", "eyev_calibrated");
    
    std::string tagname(parameters.find("tag")->second);
    shared_ptr<Variable> eyeh_raw = reg->getVariable(parameters.find("eyeh_raw")->second);    
    shared_ptr<Variable> eyev_raw = reg->getVariable(parameters.find("eyev_raw")->second);    
    shared_ptr<Variable> eyeh_calibrated = reg->getVariable(parameters.find("eyeh_calibrated")->second);    
    shared_ptr<Variable> eyev_calibrated = reg->getVariable(parameters.find("eyev_calibrated")->second);    
    
    checkAttribute(eyeh_raw, parameters["reference_id"], "eyeh_raw", parameters.find("eyeh_raw")->second);
    checkAttribute(eyev_raw, parameters["reference_id"], "eyev_raw", parameters.find("eyev_raw")->second);
    checkAttribute(eyeh_calibrated, parameters["reference_id"], "eyeh_calibrated", parameters.find("eyeh_calibrated")->second);
    checkAttribute(eyev_calibrated, parameters["reference_id"], "eyev_calibrated", parameters.find("eyev_calibrated")->second);
    
    shared_ptr <mw::Component> newEyeCalibrator = shared_ptr<mw::Component>(new EyeCalibrator(tagname, 
                                                                                              eyeh_raw, 
                                                                                              eyev_raw,
                                                                                              eyeh_calibrated,
                                                                                              eyev_calibrated,
                                                                                              2));
    return newEyeCalibrator;
}
