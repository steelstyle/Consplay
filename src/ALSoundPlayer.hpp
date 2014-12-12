/* 
 * File:   ALSoundPlayer.hpp
 * Author: steel
 *
 * Created on 7 avril 2012, 
 */

#ifndef ALSOUNDPLAYER_HPP
#define	ALSOUNDPLAYER_HPP

#include <AL/al.h>
#include <AL/alc.h>

// Pour la lecture en streaming on prend la taille minimale de buffers
// c'est a dire 2
#define PLAYER_BUFFER_NBR 3
// Le nombre de seconde lue par frame
#define SECOND_READ_BYFRAME 3

#include "SoundPlayer.hpp"
#include "SoundFactory.hpp"

#include <iostream>

class ALSoundPlayer : public ISoundPlayer
{
    public :
        ALSoundPlayer();
        ~ALSoundPlayer();
        
        static void getListOfDevices();
        
        bool addSong(string filename);
        
        void goTo(float percent)
        {
            bool playingSong = isPlaying();
            
            alSourceStop(mSource);
            alSourcei(mSource,AL_BUFFER,0);
            mSecondRead = percent * getTotalTime();
            mCurrentSound->seek(mSecondRead);
            fillBuffers();
            
            if (playingSong)
            {
                alSourcePlay(mSource);
            }
            
        }
        
        void play()
        {
            if (!isPlaying())
                alSourcePlay(mSource);
        }
        
        void pause()
        {
            alSourcePause(mSource);
        }
        
        void playStream();
        
        void process()
        {
            if (isPlaying())
            {
                playStream();
            }
        }
        
        void stop()
        {
            alSourceStop(mSource);
            mCurrentSound->stop();
            
            //Les buffers sont maintenant obselete;
            alSourcei(mSource,AL_BUFFER,0);
            fillBuffers();
            alSourceRewind(mSource);
        }
        
        void next();
        
        void prev();
        
        void setVolume(float volume)
        {
            alSourcef(mSource,AL_GAIN,volume);
        }
        
        bool isPlaying()
        {
            int playing;
            alGetSourcei(mSource,AL_SOURCE_STATE,&playing);
            return (playing == AL_PLAYING);
        }
        
        float getTime()
        {
            if (mCurrentSound != 0)
            {
                float secondOffset;
                alGetSourcef(mSource,AL_SEC_OFFSET,&secondOffset);
                return mSecondRead + secondOffset;
            }
            return 0;
        }
        
        float getTotalTime()
        {
            if (mCurrentSound != 0)
            {
                return mCurrentSound->getTotalTime();
            }
            return 0;
        }
        
    private :
        
        void resetBuffers()
        {
            
        }
        void fillBuffers();
        
        void loadSound(SongDescriptor* descriptor)
        {
            alSourceStop(mSource);
            alSourcei(mSource,AL_BUFFER,0);
            
            mCurrentSound = mSndFactory->getSound(descriptor);
            fillBuffers();
            mSecondRead = 0;
            
            mReceiver->onCurrentSongChange(descriptor);
        }
        
        void readData();
        
        ALCdevice *mDevice;
        ALCcontext *mContext;
        
        // On ne joue qu'un son a la fois on a donc besoin
        // que d'une source
        ALuint mSource;
        ALuint mBuffers[PLAYER_BUFFER_NBR];
        int mCurrentBuffer;
        
        // nbSecondRead
        int mSecondRead;
        
        SoundFactory* mSndFactory;
        IStreamedSound* mCurrentSound;
};

#endif	/* ALSOUNDPLAYER_HPP */

