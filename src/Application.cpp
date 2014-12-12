#include "ALSoundPlayer.hpp"
#include "Application.hpp"
#include "SkinLoader.h"

#include "SkinLoader.h"
#include "CImageGUISkin.h"

#include <iomanip>


core::stringw convertSecondToString(int seconds)
{
	int t_sec = seconds%60;
	int t_min = seconds/60;
	
	core::stringw text = "";
	text += t_min;
	text += ":";
	if (t_sec < 10) text += 0;
	text += t_sec;
	
	return text;
}

core::stringw getTimeString(int current, int total)
{
	core::stringw timeString = convertSecondToString(current);
	timeString += "/";
	timeString += convertSecondToString(total);
	
	return timeString;
}

Application::Application()
{
	mDevice = createDevice(video::EDT_OPENGL,core::dimension2du(900,400),32,false,false,true);
	mDevice->setWindowCaption(L"ConsPlay by Abou and Romeo");
	mDevice->setResizable(false);
    
    mDriver = mDevice->getVideoDriver();
    mSmgr = mDevice->getSceneManager();
    mEnv = mDevice->getGUIEnvironment();
	
	mDevice->setEventReceiver(this);
	
	mPlayer = new ALSoundPlayer();
	mPlayer->setEventReceiver(this);
	
	initSkin();
	initGui();
	initScene();
}

Application::~Application()
{
	mPlayer->stop();
	delete mPlayer;
	
	mDevice->drop();
}

bool Application::run()
{
	if ( mDevice->run() )
    {
		float time = mPlayer->getTime();
		float totalTime = mPlayer->getTotalTime();
		
		mTrackBar->setText(getTimeString(time,totalTime).c_str());
		mTrackBar->setProgress(float(time)/float(totalTime));
		
		core::vector3df rotation = mCube->getRotation();
		if (mPlayer->isPlaying())
		{
			if (mRotationSpeed.X <= 1.2)
			{
				mRotationSpeed.X += 0.01;
				mRotationSpeed.Y += 0.003;
			}
		}
		else // On ralenti le cube legerement
		{
			mRotationSpeed.X *= 0.99;
			mRotationSpeed.Y *= 0.99;
		}
		
		rotation.X += mRotationSpeed.X;
		rotation.Y += mRotationSpeed.Y;
		mCube->setRotation(rotation);
		
        mDriver->beginScene(true, true, video::SColor(0,5,5,5));
		
		mDriver->draw2DImage(bgImage,core::vector2di(0,0));
		
		mSmgr->drawAll();
        mEnv->drawAll();
		
        mDriver->endScene();
		
		mPlayer->process();
		mDevice->yield();
		
		return true;
    }
	
	return false;
}

void Application::initSkin()
{
	io:IFileSystem* filesystem = mDevice->getFileSystem();
	
	SImageGUISkinConfig guicfg = LoadGUISkinFromFile(filesystem, mDriver, "ui/guiskin.cfg");
    CImageGUISkin* skin = new CImageGUISkin(mDriver, mEnv->getSkin());
    skin->loadConfig(guicfg);

    IGUIFont* font = mEnv->getFont("ui/fontlucida.png");

	if (font != 0)
	{
		skin->setFont(font, EGDF_DEFAULT);
		skin->setFont(font, EGDF_WINDOW);
	}

	s32 j = 0;
	video::SColor newCol = video::SColor();
	video::SColor oldCol = newCol;
	IGUIScrollBar* bar;

	for(j = 0; j < EGDC_COUNT; ++j)
	{
		oldCol = skin->getColor((EGUI_DEFAULT_COLOR)j);
		
		newCol = oldCol;
		newCol.setAlpha(255);

		skin->setColor((EGUI_DEFAULT_COLOR)j, newCol);
	}

	skin->setColor(EGDC_ACTIVE_CAPTION, video::SColor(255,255,255,255));
	skin->setColor(EGDC_BUTTON_TEXT, video::SColor(255,255,255,255));
	skin->setColor(EGDC_HIGH_LIGHT_TEXT, video::SColor(255,255,255,255));
	skin->setColor(EGDC_GRAY_TEXT, video::SColor(150,255,255,255));
	skin->setColor(EGDC_INACTIVE_CAPTION, video::SColor(150,255,255,255));
	skin->setColor(EGDC_WINDOW_SYMBOL,video::SColor(255,255,205,25));
	skin->setColor(EGDC_SCROLLBAR,video::SColor(80,200,200,255));
	
	skin->setSize(EGDS_WINDOW_BUTTON_WIDTH, 22);
	skin->setSize(EGDS_TITLEBARTEXT_DISTANCE_X, 2);
	skin->setSize(EGDS_TITLEBARTEXT_DISTANCE_Y, 9);

    mEnv->setSkin(skin);

    skin->drop();
}

void Application::initGui()
{
	bgImage = mDriver->getTexture("ui/background.png");
	
	IGUIWindow* win = mEnv->addWindow(core::rect<s32>(10,10,890,390), false, L"ConsPlay - v0.01");
	win->setDraggable(false);
	
	int top = 30;
	
	mEnv->addStaticText(L"Title",core::rect<s32>(40,80,90,100),false,false,win);
	mEnv->addStaticText(L"Artiste",core::rect<s32>(40,110,90,130),false,false,win);
	mEnv->addStaticText(L"Album",core::rect<s32>(40,140,90,160),false,false,win);
	//mEnv->addStaticText(L"Artiste",core::rect<s32>(40,200,90,220),false,false,win);
	
	mSoundDesc.title = mEnv->addStaticText(L"",core::rect<s32>(110,80,240,100),false,false,win);
	mSoundDesc.artist = mEnv->addStaticText(L"",core::rect<s32>(110,110,240,130),false,false,win);
	mSoundDesc.album = mEnv->addStaticText(L"",core::rect<s32>(110,140,240,160),false,false,win);
	
	mSoundDesc.title->setOverrideColor(video::SColor(255,255,205,25));
	mSoundDesc.artist->setOverrideColor(video::SColor(255,255,205,25));
	mSoundDesc.album->setOverrideColor(video::SColor(255,255,205,25));
	
	top = 260;
	mEnv->addCheckBox(false,core::rect<s32>(40,top,120,top+20),win,CHK_BOX_LOOP,L"Loop");
	mEnv->addCheckBox(false,core::rect<s32>(140,top,220,top+20),win,CHK_BOX_SHUFFLE,L"Shuffle");
	
	top += 40;
	
	mTrackBar = new CGUIProgressBar(mEnv->getRootGUIElement(), mEnv, core::rect<s32>(40,top,850,top+20));
	mTrackBar->setText(L"0:00 / 0:00");
	mTrackBar->setFillColor(video::SColor(255,255,255,255));
	mTrackBar->setEmptyColor(video::SColor(180,255,255,255));
	win->addChild(mTrackBar);
    mTrackBar->setProgress(0.35f);
    mTrackBar->drop();
	
	IGUIScrollBar* volumeBar = mEnv->addScrollBar(false,core::rect<s32>(835,50,850,280),win,SCROLL_VOLUME);
	// Le Maximum est de 150%
	volumeBar->setMax(150);
	
	mTable = mEnv->addTable(core::rect<s32>(250,50,820,280),win,22,false);
	mTable->addColumn(L"N°",0);
	mTable->addColumn(L"Artiste",1);
	mTable->addColumn(L"Titre",2);
	mTable->addColumn(L"Album",3);
	
	for (int i=1;i<4; i++)
	{
		mTable->setColumnWidth(i,170);
	}
	
	top = 340;
	int bottom = top + 25;
	
	
	IGUIButton *btn3 = mEnv->addButton(core::rect<s32>(25,top,95,bottom),win,BTN_PREV,L"Prev");
	mBtnPlay = mEnv->addButton(core::rect<s32>(100,top-5,170,bottom),win,BTN_PLAY,L"Play");
	IGUIButton *btn4 = mEnv->addButton(core::rect<s32>(175,top,245,bottom),win,BTN_NEXT,L"Next");
	
	
	IGUIButton *btn5 = mEnv->addButton(core::rect<s32>(520,top,590,bottom),win,BTN_OPENFILE,L"Open File");
	IGUIButton *btn6 = mEnv->addButton(core::rect<s32>(610,top,680,bottom),win,BTN_OPENDIR,L"Open Dir");
	
	mEnv->saveGUI("file.xml",mEnv->getRootGUIElement());
	
	
}

void Application::initScene()
{
	mSmgr->addLightSceneNode(0,core::vector3df(20,20,20),video::SColorf(0.8f,0.5f,0.3,1.));
	mSmgr->addLightSceneNode(0,core::vector3df(-15,15,-15),video::SColorf(0.6f,0.02f,0.02,1.0f));
	
	mCube = mSmgr->addCubeSceneNode(10,0,0);
	mCube->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	mCube->getMaterial(0).DiffuseColor.set(255,170,80,255);
	mCube->getMaterial(0).AmbientColor.set(255,20,20,20);
	mCube->getMaterial(0).SpecularColor.set(255,80,2,255);
	
	mSmgr->setAmbientLight(video::SColorf(0.01f,0.01,0.01f,1.0f));
	
	irr::scene::ICameraSceneNode* cam = mSmgr->addCameraSceneNode(0,core::vector3df(30,15,30),core::vector3df(-45,-20,32));
	mSmgr->setActiveCamera(cam);
}

bool Application::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		core::position2d<s32> mousePos(event.MouseInput.X,event.MouseInput.Y);
		switch(event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN :
				core::rect<s32> rect = mTrackBar->getAbsolutePosition();
				if (rect.isPointInside(mousePos))
				{
					float percent = (mousePos.X - rect.UpperLeftCorner.X);
					percent /= rect.getWidth();
					mPlayer->goTo(percent);
				}
				break;
		}
	}
	if (event.EventType == EET_GUI_EVENT)
	{
		IGUICheckBox* chkBox;
		switch(event.GUIEvent.EventType)
		{
			case EGET_BUTTON_CLICKED :
				switch (event.GUIEvent.Caller->getID())
				{
					case BTN_PLAY :
						if (mPlayer->isPlaying())
						{
							mPlayer->pause();
							mBtnPlay->setText(L"Play");
						}
						else
						{
							mPlayer->play();
							mBtnPlay->setText(L"Pause");
						}
						break;
					case BTN_PREV:
							mPlayer->prev();
						break;
					case BTN_NEXT:
							mPlayer->next();
						break;
					case BTN_OPENFILE:
						mEnv->addFileOpenDialog(L"Open Sound File",true,0,OPEN_FILE_DIALOG);
						break;
					case BTN_OPENDIR:
						break;
					case CHK_BOX_LOOP:
						chkBox = (IGUICheckBox*)event.GUIEvent.Caller;
						mPlayer->setRepeat(chkBox->isChecked());
						break;
					case CHK_BOX_SHUFFLE:
						chkBox = (IGUICheckBox*)event.GUIEvent.Caller;
						mPlayer->setRandomize(chkBox->isChecked());
						break;
				}
			break;
			case EGET_FILE_SELECTED :
			{
				IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
				char dest[400];
				const wchar_t *filenameW = dialog->getFileName();

				if (filenameW != 0)
				{
					wcstombs(dest,filenameW,400);
					mPlayer->addSong(dest);
				}
			}
			break;
			case EGET_SCROLL_BAR_CHANGED :
				if (event.GUIEvent.Caller->getID() == SCROLL_VOLUME)
				{
					IGUIScrollBar* bar = (IGUIScrollBar*)event.GUIEvent.Caller;
					float volumeScale = bar->getPos();
					volumeScale /= 100;
					mPlayer->setVolume(1.5f - volumeScale);
				}
			break;
		}
	}
	return false;
}

void Application::onNewSong(SongDescriptor* descriptor)
{
	int row = mTable->getRowCount();
	mTable->addRow(row);
	
	mTable->setCellText(row,COL_N,core::stringw(descriptor->trackNumber.c_str()));
	mTable->setCellText(row,COL_ARTIST,core::stringw(descriptor->artist.c_str()));
	mTable->setCellText(row,COL_ALBUM,core::stringw(descriptor->album.c_str()));
	mTable->setCellText(row,COL_TITLE,core::stringw(descriptor->title.c_str()));
}

void Application::onCurrentSongChange(SongDescriptor* descriptor)
{
	mSoundDesc.title->setText(core::stringw(descriptor->title.c_str()).c_str());
	mSoundDesc.album->setText(core::stringw(descriptor->album.c_str()).c_str());
	mSoundDesc.artist->setText(core::stringw(descriptor->artist.c_str()).c_str());
}