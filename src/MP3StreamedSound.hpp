/* 
 * File:   MP3StreamedSound.hpp
 * Author: steel
 *
 * Created on 9 avril 2012, 
 */

#ifndef __MP3_STREAMSOUND_HPP__
#define	__MP3_STREAMSOUND_HPP__

#include "IStreamedSound.hpp"
#include "libmpg123/mpg123.h"

struct MP3StreamedSound : public IStreamedSound
{
    public :
        static SongDescriptor* getDescriptor(string filename);
        
        MP3StreamedSound(SongDescriptor* descriptor);
        ~MP3StreamedSound();
        
        int readData(int nbSeconds);
        
        void seek(int nbSeconds)
        {
            int frames = mpg123_timeframe(mMpgHandle,nbSeconds);
            mpg123_seek_frame(mMpgHandle,frames,SEEK_SET);
        }
        
        void stop()
        {
            
        }
        
        int getTotalTime()
        {
            return mTotalTime;
        }
        
        int getNbChannel()
        {
            return mNbChannel;
        }
        
        int getSamplesFreq()
        {
            return mMpgInfo->rate;
        }
        
        int getTotalFrameNbr()
        {
            return mTotalFrameNbr;
        }
        
        void* getLastData()
        {
            return buffer;
        }
        
    private :
        mpg123_handle* mMpgHandle;
        mpg123_frameinfo* mMpgInfo;
        
        unsigned char* buffer;
        unsigned int bufferSize;
        
        int mTotalTime;
        int mTotalFrameNbr;
        
        int mNbChannel;
};

#endif	/* MP3STREAMEDSOUND_HPP */

