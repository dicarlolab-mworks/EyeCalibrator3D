/*
 *  EyeCalibrator3DFactory.h
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef EyeCalibrator3D_FACTORY_H
#define EyeCalibrator3D_FACTORY_H

#include <MWorksCore/ComponentFactory.h>

using namespace mw;


class EyeCalibrator3DFactory : public ComponentFactory {

private:
    int order;
    
public:
    EyeCalibrator3DFactory(int order) : order(order) { }
    virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   ComponentRegistry *reg);

};


#endif
