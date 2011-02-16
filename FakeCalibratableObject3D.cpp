/*
 *  FakeCalibratableObject3D.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/16/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "FakeCalibratableObject3D.h"


FakeCalibratableObject3D::FakeCalibratableObject3D(const std::string &tag,
                                                   shared_ptr<Variable> xValue,
                                                   shared_ptr<Variable> yValue,
                                                   shared_ptr<Variable> zValue) :
    Component(tag),
    xValue(xValue),
    yValue(yValue),
    zValue(zValue)
{ }


ExpandableList<Datum>* FakeCalibratableObject3D::getGoldStandardValues() {
    goldStandardValues->clear();

    goldStandardValues->addElement(xValue->getValue());
    goldStandardValues->addElement(yValue->getValue());
    goldStandardValues->addElement(zValue->getValue());

    return goldStandardValues;
}
