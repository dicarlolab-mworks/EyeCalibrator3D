/*
 *  FakeCalibratableObject3DFactory.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/16/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include <MWorksCore/ComponentRegistry.h>

#include "FakeCalibratableObject3DFactory.h"
#include "FakeCalibratableObject3D.h"

using namespace mw;


shared_ptr<mw::Component> FakeCalibratableObject3DFactory::createObject(std::map<std::string, std::string> parameters,
                                                                        ComponentRegistry *reg)
{
    const char* TAG = "tag";
    const char* X_VALUE = "x_value";
    const char* Y_VALUE = "y_value";
    const char* Z_VALUE = "z_value";
    
    REQUIRE_ATTRIBUTES(parameters,
                       TAG,
                       X_VALUE,
                       Y_VALUE,
                       Z_VALUE);
    
    std::string tag(parameters[TAG]);
    
    shared_ptr<Variable> xValue(reg->getVariable(parameters[X_VALUE]));
    shared_ptr<Variable> yValue(reg->getVariable(parameters[Y_VALUE]));
    shared_ptr<Variable> zValue(reg->getVariable(parameters[Z_VALUE]));
    
    CHECK_ATTRIBUTE(xValue, parameters, X_VALUE);
    CHECK_ATTRIBUTE(yValue, parameters, Y_VALUE);
    CHECK_ATTRIBUTE(zValue, parameters, Z_VALUE);
    
    return shared_ptr<FakeCalibratableObject3D>(new FakeCalibratableObject3D(tag, xValue, yValue, zValue));
}






















