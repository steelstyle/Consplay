/* 
 * File:   SndStreamedSound.hpp
 * Author: steel
 *
 * Created on 7 avril 2012, 
 */

#ifndef __SND_STREAMEDSOUND_HPP__
#define	__SND_STREAMEDSOUND_HPP__

#include <string>
#include <sndfile.h>

#ifdef __unix__
#include <AL/al.h>
#else
#include <al.h>
#endif


#include "IStreamedSound.hpp"

using namespace std;

class SndStreamedSound : public IStreamedSound
{
    public :
        
        static SongDescriptor* getDescriptor(string filename);
        
        SndStreamedSound(SongDescriptor* descriptor);
        ~SndStreamedSound();
        
        int readData(int nbSeconds);
        
        void seek(int seconds)
        {
            sf_seek(mSndFile,seconds*getSamplesFreq(),SEEK_SET);
        }
        
        void stop()
        {
            mFinished = false;
            seek(0);
        }
        
        int getTotalTime()
        {
            return mTotalTime;
        }
        
        int getNbChannel()
        {
            return mFileInfo.channels;
        }
        
        int getSamplesFreq()
        {
            return mFileInfo.samplerate;
        }
        
        int getTotalFrameNbr()
        {
            return mFileInfo.frames;
        }
        
        void* getLastData()
        {
            return data;
        }
        
    private :
        int mTotalSize;
        int mReadSize;
        
        SNDFILE* mSndFile;
        SF_INFO mFileInfo;
        
        ALshort *data;
        
        int mTotalTime;
};


#endif	/* SND_STREAMEDSOUND_HPP */

