/*
 *  EyeCalibrator3DPlugin.h
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/9/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef EyeCalibrator3D_PLUGIN_H_
#define EyeCalibrator3D_PLUGIN_H_

#include <MWorksCore/Plugin.h>

using namespace mw;


extern "C" {
    Plugin* getPlugin();
}


class EyeCalibrator3DPlugin : public Plugin {
    virtual void registerComponents(shared_ptr<ComponentRegistry> registry);
};


#endif
