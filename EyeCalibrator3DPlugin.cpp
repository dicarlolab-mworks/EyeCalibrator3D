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

using namespace mw;


Plugin* getPlugin() {
    return new EyeCalibrator3DPlugin();
}


void EyeCalibrator3DPlugin::registerComponents(shared_ptr<ComponentRegistry> registry) {
    // TODO: you need to customize the "signature" of the object your plugin will create
    //       The signature is of the form component/type Ð(e.g. stimulus/circle, or iodevice/NIDAQ)
    registry->registerFactory(std::string("stimulus/EyeCalibrator3D"),
                              (ComponentFactory *)(new EyeCalibrator3DFactory()));
}
