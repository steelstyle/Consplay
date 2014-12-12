#include <iostream>
#include "ALSoundPlayer.hpp"

ALSoundPlayer::ALSoundPlayer():ISoundPlayer(),mCurrentBuffer(0),mCurrentSound(0)
{
	mDevice = alcOpenDevice(NULL);
	
	if (mDevice)
	{
		const ALCchar* name = alcGetString(mDevice,ALC_DEVICE_SPECIFIER);
		cout << "Device name : " << name << endl;
		// Création du contexte OpenAL
		mContext = alcCreateContext(mDevice,NULL);
		alcMakeContextCurrent(mContext);
	}
	else
	{
		cout << "Probleme" << endl;
	}
	alGetError();
	
	alGenSources(1,&mSource);
	
	alGetError();
	
	alGenBuffers(PLAYER_BUFFER_NBR,mBuffers);
	
	mSndFactory = new SoundFactory();
}

ALSoundPlayer::~ALSoundPlayer()
{
	stop();
	
	alSourcei(mSource,AL_BUFFER,0);
	
	alDeleteBuffers(PLAYER_BUFFER_NBR,mBuffers);
	alDeleteSources(1,&mSource);
	
	alcMakeContextCurrent(NULL);
	alcDestroyContext(mContext);	
	alcCloseDevice(mDevice);
	
	delete mSndFactory;
}


void ALSoundPlayer::getListOfDevices()
{
	if (alcIsExtensionPresent(NULL,"ALC_ENUMERATE_EXT") == AL_TRUE)
	{
		string st(alcGetString(NULL,ALC_DEVICE_SPECIFIER));
		std::cout << st << endl;
	}
	else
	{
		std::cout << "Vide "<< endl;
	}
}


bool ALSoundPlayer::addSong(string filename)
{
	SongDescriptor *descriptor = SoundFactory::getDescriptor(filename);

	if (mPlaylist.empty())
	{
		loadSound(descriptor);
	}

	mPlaylist.push_back(mSongsNames.size());
	mSongsNames.push_back(descriptor);
	
	if (mReceiver != 0)
	{
		mReceiver->onNewSong(descriptor);
	}

	return true;
}

void ALSoundPlayer::next()
{
	int next = getNextSongId();	
	ISoundPlayer::next();
	if (next != -1)
	{
		loadSound(mSongsNames[next]);
		play();
	}
}

void ALSoundPlayer::prev()
{
	int prev = getPrevSongId();
	ISoundPlayer::prev();
	if (prev != -1)
	{
		loadSound(mSongsNames[prev]);
		play();
	}
}

void ALSoundPlayer::fillBuffers()
{
	mCurrentBuffer = 0;
	for (int i=0; i<PLAYER_BUFFER_NBR;i++)
	{
		readData();
	}
	alSourceQueueBuffers(mSource,PLAYER_BUFFER_NBR,mBuffers);

}


// Lis notre son audio par buffer de SECOND_READ_BYFRAME
void ALSoundPlayer::playStream()
{         
	int nbProcessed;
	alGetSourcei(mSource,AL_BUFFERS_PROCESSED,&nbProcessed);
	if (!mCurrentSound->isFinished())
	{
		while (nbProcessed-- >= 1 )
		{
			int buffer = mCurrentBuffer;
			alSourceUnqueueBuffers(mSource,1,mBuffers+buffer);
			readData();
			alSourceQueueBuffers(mSource,1,mBuffers+buffer);
			mSecondRead += SECOND_READ_BYFRAME;
		}
	}
	else
	{
		int next = getNextSongId();
		
		if (next != -1)
		{
			mCurrentSound = mSndFactory->getSound(mSongsNames[next]);
		}
	}
}


//Lis le buffer du son courant et les charge dans les buffer openal
void ALSoundPlayer::readData()
{
	int nbRead = mCurrentSound->readData(SECOND_READ_BYFRAME);

	int format;

	switch(mCurrentSound->getNbChannel())
	{
		case 1:
			format = AL_FORMAT_MONO16;
			break;
		case 2:
			format = AL_FORMAT_STEREO16;
			break;
	}

	alBufferData(mBuffers[mCurrentBuffer],format,mCurrentSound->getLastData(),
			sizeof(ALushort)*nbRead,mCurrentSound->getSamplesFreq());

	mCurrentBuffer = (mCurrentBuffer +1)%PLAYER_BUFFER_NBR;
}