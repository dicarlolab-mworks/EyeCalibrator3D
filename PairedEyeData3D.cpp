/*
 *  PairedEyeData3D.cpp
 *  EyeCalibrator3D
 *
 *  Created by Christopher Stawarz on 2/15/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "PairedEyeData3D.h"


PairedEyeData3D::PairedEyeData3D(int buffer_size) {
    // setup buffers to hold eye data -- we assume that these are collected at the
    //  same rate, and we can check this occasionally    
    
    eyeH_buffer = new FloatDataTimeStampedRingBuffer(buffer_size); 
    eyeV_buffer = new FloatDataTimeStampedRingBuffer(buffer_size);
    eyeH_buffer_reader = new FloatDataTimeStampedBufferReader(eyeH_buffer);       
    eyeV_buffer_reader = new FloatDataTimeStampedBufferReader(eyeV_buffer);         
}

PairedEyeData3D::~PairedEyeData3D() {
    delete eyeH_buffer;
    delete eyeV_buffer;
    delete eyeH_buffer_reader;
    delete eyeV_buffer_reader;
}

void PairedEyeData3D::reset() {
    eyeH_buffer->reset();
    eyeV_buffer->reset();
    eyeH_buffer_reader->seek(0);
    eyeV_buffer_reader->seek(0);
}


void PairedEyeData3D::putDataH(double eyeH, MWTime timeUS) {
    eyeH_buffer->putData(eyeH, timeUS);
}
void PairedEyeData3D::putDataV(double eyeV, MWTime timeUS) {
    eyeV_buffer->putData(eyeV, timeUS);
}

// need to be sophisticated about waiting for pairs of H and V before transforming to final output
// ideally, all data should be paired (ITC does this)
// here, we just insure that the pairs that are sent for processing have stamped times within M_WARN_EYE_PAIR_SEPARATION_US of each other.

bool PairedEyeData3D::getAvailable(double *pEyeH, double *pEyeV, MWTime *pEyeTimeUS) {
    
    if ( (eyeH_buffer_reader->getNItemsUnserviced()==0) || 
        (eyeV_buffer_reader->getNItemsUnserviced()==0) ) {
        return false;        //  no data in one or more buffers
    }
    
    bool diffWarned = false;
    
    // keep trying to find paired data
    long diff;
    MWTime eyeVtimeUS, eyeHtimeUS;
    while ( (eyeH_buffer_reader->getNItemsUnserviced()>0) && 
           (eyeV_buffer_reader->getNItemsUnserviced()>0) ) {
        
        *pEyeV = (double)(eyeV_buffer_reader->getData());
        eyeVtimeUS = eyeV_buffer_reader->getTime();
        
        *pEyeH = (double)(eyeH_buffer_reader->getData());
        eyeHtimeUS = eyeH_buffer_reader->getTime();
        
        diff = (long)(eyeHtimeUS-eyeVtimeUS);
        
        if (abs(diff) <= M_MAXIMAL_ALLOWED_EYE_PAIR_SEPARATION_US) {   // data alignment acceptable to use 
            *pEyeTimeUS = eyeHtimeUS;
            eyeH_buffer_reader->advance();
            eyeV_buffer_reader->advance();
            return true;  // data should be used by object
        }
        
        if (!diffWarned) {
            mwarning(M_SYSTEM_MESSAGE_DOMAIN,
                     "mPairedEyeData3D:  Eye H and V data are not paired in time within required limit of %d us. They are separated by %d us  (+ means old V data has no paired H). Correcting by discarding unpaired data.", 
                     M_MAXIMAL_ALLOWED_EYE_PAIR_SEPARATION_US, diff);
            diffWarned = true;
        }
        
        // try to align data by discarding old data that has no possibility of being paired
        //  (note:  this assumes that the data arrive to the object in chronological order !)
        
        if (eyeHtimeUS>eyeVtimeUS) {  // eyeV is too old -- kill it
            //mprintf("Ignoring one element of eye V data");
            eyeV_buffer_reader->advance();
        }
        else {
            //mprintf("Ignoring one element of eye H data");
            eyeH_buffer_reader->advance();
        }
    }
    
    // if we exit the while loop, one or more buffers is empty               
    return false;
    
}
