/* 
 * File:   SoundFactory.hpp
 * Author: steel
 *
 * Created on 4 mai 2012, 
 */

#ifndef SOUNDFACTORY_HPP
#define	SOUNDFACTORY_HPP

#include <string>
#include "MP3StreamedSound.hpp"
#include "SndStreamedSound.hpp"

using namespace std;

class SoundFactory
{
    public :
        static IStreamedSound* getSound(SongDescriptor* descriptor)
         {
            IStreamedSound* result;
            if (descriptor->filename.find(".mp3") != string::npos)
            {
#ifdef __COMPILE_WITH_MP3__
                result = new MP3StreamedSound(descriptor);
#endif
            }
            else
            {
                result = new SndStreamedSound(descriptor);
            }
            return result;
        }
        
        
        static SongDescriptor* getDescriptor(string filename)
        {
            SongDescriptor* result;
            if (filename.find(".mp3") != string::npos)
            {
#ifdef __COMPILE_WITH_MP3__
                result = MP3StreamedSound::getDescriptor(filename);
#endif
            }
            else
            {
                result = SndStreamedSound::getDescriptor(filename);
            }
            return result;
        }

};


#endif	/* SOUNDFACTORY_HPP */

