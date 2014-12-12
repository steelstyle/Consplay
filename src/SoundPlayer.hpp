/* 
 * File:   SoundPlayer.hpp
 * Author: steel
 *
 * Created on 5 avril 2012, 
 */

#ifndef SOUNDPLAYER_HPP
#define	SOUNDPLAYER_HPP

#include <deque>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "IPlayerEventReceiver.hpp"
#include "SongDescriptor.hpp"

#define PREVIOUS_LIMIT 500

class ISoundPlayer
{
    public :
        ISoundPlayer():mCurrentSoundId(0),mRepeat(false),mShuffle(false)
        {
            mReceiver = 0;
            
            srand(time(NULL));
        }
         
        static void getListOfDevices();
        
        bool addFolder(string folder);
        bool addSong(string filename);
        bool removeSong(string filename);
        
        // Controls functions
        void goTo(float percent);
        
        void pause();
        void play();
        void stop();
        
        void next()
        {
            int next = getNextSongId();
            if (next != -1)
            {
                mCurrentSoundId = next;
                mPrevious.push_back(mCurrentSoundId);
                
                if (mPrevious.size() > PREVIOUS_LIMIT)
                    mPrevious.pop_front();
            }
        }
        
        void prev(){
            if (!mPrevious.empty())
            {
                mPrevious.pop_back();
            }
            mCurrentSoundId = getPrevSongId();
        }
        
        void setRandomize(bool playRandom)
        {
            mShuffle = playRandom;
            if (mShuffle && !mRepeat)
            {
                random_shuffle(mPlaylist.begin(),mPlaylist.end());
            }
        }
        
        void setRepeat(bool repeat)
        {
            mRepeat = repeat;
        }
        
        void setVolume(float volume);
        
        // Status function
        
        bool isPlaying();
        
        int getNextSongId()
        {
            int listSize = mPlaylist.size();
            int nextId;
            
            if (mShuffle && mRepeat)
            {
                int nextId;
                while(nextId == mCurrentSoundId)
                {
                    nextId = rand()%listSize;
                }
            }
            else
            {
                nextId = mCurrentSoundId +1;
                if (nextId == listSize)
                {
                    if (mRepeat) nextId = 0;
                    else nextId = -1;
                }
            }
            
            return nextId;
        }
        
        int getPrevSongId()
        {
          if (mShuffle)
          {
              if (mPrevious.empty())
                  return -1;
              
              return mPrevious.back();
          }
          else {
              return mCurrentSoundId - 1;
          }
        }
        
        void setEventReceiver(IPlayerEventReceiver* receiver)
        {
            mReceiver = receiver;
        }
        
        int getTime();
        
        int getTotalTime();
        
    protected :
        bool mRepeat;
        bool mShuffle;
        float mVolume;
        
        int mCurrentSoundId;
        
        deque<SongDescriptor*> mSongsNames;
        deque<int> mPlaylist;
        deque<int> mPrevious;
        
        //Notre event receveiver
        IPlayerEventReceiver* mReceiver;
};

#endif	/* SOUNDPLAYER_HPP */

