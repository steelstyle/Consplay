/* 
 * File:   StreamedSound.hpp
 * Author: steel
 *
 * Created on 7 avril 2012, 
 */

#ifndef __I_STREAMEDSOUND_HPP__
#define	__I_STREAMEDSOUND_HPP__

#include <string>

#include "SongDescriptor.hpp"

using namespace std;

class IStreamedSound
{
    public :
        
        IStreamedSound(SongDescriptor* descriptor):mFinished(false)
        {
            mDescriptor = descriptor;
        }
        
        virtual ~IStreamedSound()
        {};
        
        virtual int readData(int nbSeconds) = 0;
        
        virtual bool isFinished()
        {
            return mFinished;
        }
        
        virtual void seek(int seconds) = 0;
        
        virtual void stop() = 0;
        
        virtual int getNbChannel() = 0;
        
        virtual int getSamplesFreq() = 0;
        
        virtual int getTotalFrameNbr() = 0;
        
        virtual int getTotalTime() = 0;
        
        string getAlbumName()
        {
            return mDescriptor->album;
        }
        
        string getArtistName()
        {
            return mDescriptor->artist;
        }
        
        string getGenre()
        {
            return mDescriptor->genre;
        }
        
        string getTitle()
        {
            return mDescriptor->title;
        }
        
        virtual void* getLastData() = 0;
        
    protected :
     
        bool mFinished;
        
        SongDescriptor *mDescriptor;
};


#endif	/* __I_STREAMEDSOUND_HPP__ */

