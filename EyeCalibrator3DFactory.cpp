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
