/* 
 * File:   IPlayerInterface.hpp
 * Author: steel
 *
 * Created on 13 avril 2012, 
 */

#ifndef IPLAYERINTERFACE_HPP
#define	IPLAYERINTERFACE_HPP

#include "SongDescriptor.hpp"

class IPlayerEventReceiver
{
    public :
        virtual void onNewSong(SongDescriptor* descriptor) = 0;
        virtual void onCurrentSongChange(SongDescriptor* descriptor) = 0;
};


#endif	/* IPLAYERINTERFACE_HPP */

