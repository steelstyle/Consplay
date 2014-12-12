#include "MP3StreamedSound.hpp"
#include "ALSoundPlayer.hpp"
#include "libmpg123/mpg123.h"

void addIfInfoAvaible(string& field, mpg123_string* container)
{
	if (container != 0)
	{
		field.append(container->p);
	}
}

SongDescriptor* MP3StreamedSound::getDescriptor(string filename)
{
	mpg123_init();
	SongDescriptor* desc = new SongDescriptor();
	mpg123_handle* fileHandle = mpg123_new(NULL,NULL);
	mpg123_open(fileHandle,filename.c_str());

	desc->filename = filename;

	mpg123_scan(fileHandle);
	int metadata = mpg123_meta_check(fileHandle);

	// Our metadata are id3 tags
	if (metadata & MPG123_ID3)
	{
		mpg123_id3v1 *id3v1_tags;
		mpg123_id3v2 *id3v2_tags;

		if (mpg123_id3(fileHandle,&id3v1_tags,&id3v2_tags) == MPG123_OK)
		{
			if (id3v1_tags != NULL)
			{
				id3v1_tags->album;
			}
			if (id3v2_tags != NULL)
			{
				addIfInfoAvaible(desc->album,id3v2_tags->album);
				addIfInfoAvaible(desc->artist,id3v2_tags->artist);
				addIfInfoAvaible(desc->genre,id3v2_tags->genre);
				addIfInfoAvaible(desc->title,id3v2_tags->title);
			}
		}
	}

	mpg123_close(fileHandle);
	mpg123_delete(fileHandle);
	
	return desc;
}


MP3StreamedSound::MP3StreamedSound(SongDescriptor* descriptor)
	:IStreamedSound(descriptor)
{
	mMpgHandle = mpg123_new(NULL,NULL);
	mpg123_open(mMpgHandle, descriptor->filename.c_str());
	// We should call scan to get the remaining data
	mpg123_scan(mMpgHandle);
	// We create a mMpgInfo to handle file info
	mMpgInfo = new mpg123_frameinfo;
	mpg123_info(mMpgHandle,mMpgInfo);
	
	bufferSize =  mpg123_outblock(mMpgHandle) * SECOND_READ_BYFRAME * getSamplesFreq();
	buffer = new unsigned char[bufferSize];
	
	mTotalFrameNbr = mpg123_length(mMpgHandle);
	mTotalTime = getTotalFrameNbr()/getSamplesFreq();
	
	mNbChannel = 2;
	if (mMpgInfo->mode == MPG123_M_MONO)
	{
		mNbChannel = 1;
	}
}

MP3StreamedSound::~MP3StreamedSound()
{
	mpg123_close(mMpgHandle);
	mpg123_delete(mMpgHandle);
	delete []buffer;
	delete mMpgInfo;
}

/**
	Read nbSeconds of data et store then into buffer
	return the number or data read;
**/
int MP3StreamedSound::readData(int nbSeconds)
{
	size_t read;
	
	mpg123_read(mMpgHandle,buffer,bufferSize,&read);
	
	return read/2;
}
