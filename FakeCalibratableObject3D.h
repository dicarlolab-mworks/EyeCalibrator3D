/*
 *  FakeCalibratableObject3D.h
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/16/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef FakeCalibratableObject3D_H_
#define FakeCalibratableObject3D_H_

#include <MWorksCore/EyeCalibrators.h>

using namespace mw;


class FakeCalibratableObject3D : public mw::Component, public GoldStandard {
    
public:
    FakeCalibratableObject3D(const std::string &tag,
                             shared_ptr<Variable> xValue,
                             shared_ptr<Variable> yValue,
                             shared_ptr<Variable> zValue);
    virtual ~FakeCalibratableObject3D() { }
    virtual ExpandableList<Datum>* getGoldStandardValues();
    
private:
    shared_ptr<Variable> xValue;
    shared_ptr<Variable> yValue;
    shared_ptr<Variable> zValue;
    
};


#endif
