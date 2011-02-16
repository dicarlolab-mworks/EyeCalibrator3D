/*
 *  FakeCalibratableObject3DFactory.h
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/16/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef FakeCalibratableObject3D_FACTORY_H
#define FakeCalibratableObject3D_FACTORY_H

#include <MWorksCore/ComponentFactory.h>

using namespace mw;


class FakeCalibratableObject3DFactory : public ComponentFactory {
	virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   ComponentRegistry *reg);
};


#endif
