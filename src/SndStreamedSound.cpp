#include <algorithm>

#include "SndStreamedSound.hpp"
#include "ALSoundPlayer.hpp"
#include <iostream>

void addIfInfoAvaible(SNDFILE* file, string& field, int infoType)
{
	const char* info = sf_get_string(file,infoType);
	if (info != 0)
	{
		field += info;
	}
}
	
SongDescriptor* SndStreamedSound::getDescriptor(string filename)
{
	SongDescriptor* desc = new SongDescriptor();
	desc->type = SongDescriptor::SMT_LIBSND;
	SF_INFO sfInfo;
	desc->filename = filename;
	
	SNDFILE* file = sf_open(filename.c_str(),SFM_READ,&sfInfo);
	
	// On récupere tout les informations nécéssaires
	addIfInfoAvaible(file,desc->album, SF_STR_ALBUM);
	addIfInfoAvaible(file,desc->artist, SF_STR_ARTIST);
	addIfInfoAvaible(file,desc->genre, SF_STR_GENRE);
	addIfInfoAvaible(file,desc->title, SF_STR_TITLE);
	addIfInfoAvaible(file,desc->trackNumber, SF_STR_TRACKNUMBER);
	
	sf_close(file);
	return desc;
}


SndStreamedSound::SndStreamedSound(SongDescriptor* descriptor)
	:data(0),mReadSize(0),IStreamedSound(descriptor)
{	
	cout << "Loading " << descriptor->filename << endl;
	mSndFile = sf_open(descriptor->filename.c_str(),SFM_READ,&mFileInfo);
	mTotalTime = getTotalFrameNbr()/getSamplesFreq();
} 

SndStreamedSound::~SndStreamedSound()
{
	sf_close(mSndFile);
	delete [] data;
	data = 0;
}

// seconds : seconds to read
int SndStreamedSound::readData(int seconds)
{
	int read = 0;
	int secondItems = seconds*getNbChannel()*getSamplesFreq();
	
	if (data == 0)
	{
		data = new ALshort[secondItems];
	}
	
	read = sf_read_short(mSndFile,data,secondItems);
	mReadSize += seconds*getSamplesFreq();
	
	if (read < secondItems)
	{
		mFinished = true;
	}
	
	return read;
}