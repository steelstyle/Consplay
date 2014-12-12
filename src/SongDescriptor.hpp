/* 
 * File:   SongDescriptor.hpp
 * Author: steel
 *
 * Created on 7 avril 2012, 
 */

#ifndef SONGDESCRIPTOR_HPP
#define	SONGDESCRIPTOR_HPP

#include <string>

using namespace std;

struct SongDescriptor
{
    public :
        string filename;
        string album;
        string artist;
        string genre;
        string title;
        string trackNumber;
        
        int type;
        
        enum SONG_MANAGER_TYPE
        {
            SMT_MP3 = 1,
            SMT_LIBSND,
            SMT_COUNT
        };
};


#endif	/* SONGDESCRIPTOR_HPP */

