/* 
 * File:   Application.hpp
 * Author: steel
 *
 * Created on 8 avril 2012, 
 */

#ifndef APPLICATION_HPP
#define	APPLICATION_HPP


/**
 * TODO : Modifier l'enchainement de deux musique actuellement
 * lorsque la musique change il n'y a pas dévenement qui permet
 * d'appeler onCurrentSongChange()
 */

#include <irrlicht.h>

#include "ALSoundPlayer.hpp"
#include "CGUIProgressBar.h"

using namespace irr;


using namespace gui;
using namespace io;
using namespace scene;
using namespace video;


enum E_GUI_ID
{
	BTN_PLAY = 2000,
	BTN_PREV,
	BTN_NEXT,
	BTN_OPENFILE,
	BTN_OPENDIR,
        
        CHK_BOX_LOOP,
        CHK_BOX_SHUFFLE,
	
	OPEN_FILE_DIALOG,
        
        SCROLL_VOLUME
};

enum E_TABLE_COLUMN
{
	COL_N,
	COL_ARTIST,
	COL_TITLE,
	COL_ALBUM
};


struct GUISoundDesc
{
    IGUIStaticText* title;
    IGUIStaticText* artist;
    IGUIStaticText* album;
    IGUIStaticText* other;
};

class Application : public IEventReceiver, public IPlayerEventReceiver
{
    public :
        Application();
        ~Application();
        
        bool run();
        
        virtual bool OnEvent(const SEvent& event);
        
    private :
        
        void initSkin();
        void initGui();
        void initScene();
        
        
        void onCurrentSongChange(SongDescriptor* descriptor);
        void onNewSong(SongDescriptor* descriptor);
                
        IrrlichtDevice* mDevice;
        
        IGUIEnvironment* mEnv;
        ISceneManager* mSmgr;
        IVideoDriver* mDriver;
        
        ALSoundPlayer* mPlayer;
        
        IGUITable* mTable;
        GUISoundDesc mSoundDesc;
        
        ITexture* bgImage;
        ISceneNode* mCube;
        core::vector3df mRotationSpeed;
        
        IGUIButton *mBtnPlay;
        CGUIProgressBar* mTrackBar;
        
};


#endif	/* APPLICATION_HPP */

