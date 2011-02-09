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


#endif
