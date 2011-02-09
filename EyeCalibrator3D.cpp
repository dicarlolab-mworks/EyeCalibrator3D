/*
 *  EyeCalibrator3D.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "EyeCalibrator3D.h"


EyeCalibrator3D::EyeCalibrator3D(const std::string &tag,
                                                             shared_ptr<Variable> anotherAttribute) :
    mw::Component(tag),
    anotherAttribute(anotherAttribute)
{ }


EyeCalibrator3D::~EyeCalibrator3D() { }
