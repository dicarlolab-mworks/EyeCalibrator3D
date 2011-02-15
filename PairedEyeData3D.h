/*
 *  PairedEyeData3D.h
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/15/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef PairedEyeData3D_H_
#define PairedEyeData3D_H_

#include <MWorksCore/EyeMonitors.h>

using namespace mw;


class PairedEyeData3D {
    
private:
    // buffers to hold eye data
    FloatDataTimeStampedRingBuffer *eyeH_buffer;
    FloatDataTimeStampedRingBuffer *eyeV_buffer;
    FloatDataTimeStampedBufferReader *eyeH_buffer_reader;
    FloatDataTimeStampedBufferReader *eyeV_buffer_reader;
    
public:
    PairedEyeData3D(int buffer_size);
    virtual ~PairedEyeData3D();
    virtual void putDataH(double eyeH, MWTime timeUS);
    virtual void putDataV(double eyeV, MWTime timeUS);
    virtual bool getAvailable(double *pEyeH, double *pEyeV, MWTime *pEyeTimeUS);
    virtual void reset();
    
};


#endif
