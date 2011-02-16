/*
 *  EyeCalibrator3DPlugin.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include <MWorksCore/ComponentFactory.h>

#include "EyeCalibrator3DPlugin.h"
#include "EyeCalibrator3DFactory.h"
#include "FakeCalibratableObject3DFactory.h"

using namespace mw;


Plugin* getPlugin() {
    return new EyeCalibrator3DPlugin();
}


void EyeCalibrator3DPlugin::registerComponents(shared_ptr<ComponentRegistry> registry) {
    registry->registerFactory(std::string("calibrator/standard_eye_calibrator_3d"),
                              (ComponentFactory *)(new EyeCalibrator3DFactory()));
    registry->registerFactory(std::string("calibrator/fake_calibratable_object_3d"),
                              (ComponentFactory *)(new FakeCalibratableObject3DFactory()));
}
