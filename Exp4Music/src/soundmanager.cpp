/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "soundmanager.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDir>
#include <qdebug.h>

#define GUITAR_WAV_PATH "/app/native/assets/bass.wav"
#define SAMPLE_WAV_PATH "/accounts/1000/shared/music/sample.wav"

//Error message functions
static void reportALUTError(ALenum error)
{
	qDebug() << "[SoundManager] ALUT error: " << alutGetErrorString(error);
}

// Error message function for OpenAL.
static void reportOpenALError(ALenum error)
{
	qDebug() << "[SoundManager] OpenAL error: \n" << alGetString(error);
}

SoundManager::SoundManager()
{

	qDebug() << "[SoundManager] Constructor - start";

	mSampleBufferId = 0;

	// Initialize the ALUT
	if (alutInit(NULL, NULL) == false)
	{
		reportALUTError(alutGetError());
	}

	// Generate 2 sources, one for the guitar and one for the sample loop
	alGenSources(1, &mGuitarSourceId);
	alGenSources(1, &mSampleSourceId);
	qDebug() << "[SoundManager] Constructor - Created sources for guitar and sample";

	// Preload the guitar sound
	QString guitarPath = QDir::currentPath();
	guitarPath.append(GUITAR_WAV_PATH);

	qDebug() << "[SoundManager] Constructor - Attempting to create buffer for guitar file: " << guitarPath;
	mGuitarBufferId = alutCreateBufferFromFile(guitarPath.toStdString().c_str());

	ALenum error = alutGetError();
	if (error == AL_NO_ERROR)
	{
		qDebug() << "[SoundManager] Constructor - Created buffer, id: " << mGuitarBufferId;
	}
	else
	{
		reportALUTError(error);
	}

	//Attach the buffer to the source
	alSourcei(mGuitarSourceId, AL_BUFFER, mGuitarBufferId);

	error = alGetError();
	if (error == AL_NO_ERROR)
	{
		qDebug() << "[SoundManager] Constructor - Loaded guitar wav and attached it to its source";
	}
	else
	{
		reportOpenALError(error);
	}

	qDebug() << "[SoundManager] Constructor - end";
}

SoundManager::~SoundManager()
{

	qDebug() << "[SoundManager] Destructor - start";

	// Delete the sources.
	alDeleteSources(1, &mGuitarSourceId);
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		reportOpenALError(error);
	}

	alDeleteSources(1, &mSampleSourceId);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		reportOpenALError(error);
	}

	//Delete the buffers
	alDeleteBuffers(1, &mGuitarBufferId);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		reportOpenALError(error);
	}

	alDeleteBuffers(1, &mSampleBufferId);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		reportOpenALError(error);
	}

	// Exit the ALUT.
	if (alutExit() == false)
	{
		reportALUTError(alutGetError());
	}

	qDebug() << "[SoundManager] Destructor - end";

}

void SoundManager::loadSample(QString samplePath)
{
	qDebug() << "[SoundManager] loadSample - start";

	ALenum error;

	if (mSampleBufferId != 0)
	{
		qDebug() << "[SoundManager] loadSample - Buffer already exists, id: " << mSampleBufferId;

		qDebug() << "[SoundManager] loadSample - Stopping source, id: " << mSampleSourceId;
		alSourceStop(mSampleSourceId);
		error = alGetError();
		if (error == AL_NO_ERROR)
		{
			qDebug() << "[SoundManager] loadSample - Stopped source id: " << mSampleSourceId;
		}
		else
		{
			reportOpenALError(error);
		}

		ALint count;
		qDebug() << "[SoundManager] loadSample - Counting processed buffers";

		alGetSourcei(mSampleSourceId, AL_BUFFERS_PROCESSED, &count);
		if (error == AL_NO_ERROR)
		{
			qDebug() << "[SoundManager] loadSample - Got processed buffer count: " << count;

			if (count > 0)
			{
				qDebug() << "[SoundManager] loadSample - Unqueuing buffer";

				//We already have a buffer, unqueue it from the source
				alSourceUnqueueBuffers(mSampleSourceId, 1, &mSampleBufferId);
				error = alGetError();
				if (error == AL_NO_ERROR)
				{
					qDebug() << "[SoundManager] loadSample - Unqueued buffer id: " << mSampleBufferId;
				}
				else
				{
					reportOpenALError(error);
				}
			}
		}
		else
		{
			reportOpenALError(error);
		}

		//Detach the buffer from the source
		qDebug() << "[SoundManager] loadSample - Setting source id: " << mSampleSourceId << " to have null buffer";

		alSourcei(mSampleSourceId, AL_BUFFER, AL_NONE);
		if (error == AL_NO_ERROR)
		{
			qDebug() << "[SoundManager] loadSample - Source id: " << mSampleSourceId << " now has null buffer";
		}
		else
		{
			reportOpenALError(error);
		}

		qDebug() << "[SoundManager] loadSample - Deleting buffer id: " << mSampleBufferId;
		alDeleteBuffers(1, &mSampleBufferId);

		if (error == AL_NO_ERROR)
		{
			qDebug() << "[SoundManager] loadSample - Buffer deleted";
		}
		else
		{
			reportOpenALError(error);
		}
	}

	//Create a new buffer from the sample wav file
	qDebug() << "[SoundManager] loadSample - Attempting to create buffer for sample file: " << samplePath;
	mSampleBufferId = alutCreateBufferFromFile(samplePath.toStdString().c_str());

	error = alutGetError();
	if (error == AL_NO_ERROR)
	{
		qDebug() << "[SoundManager] loadSample - Created buffer, id: " << mSampleBufferId;
	}
	else
	{
		reportALUTError(error);
	}

	//Attach the new buffer to the sample source
	alSourceQueueBuffers(mSampleSourceId, 1, &mSampleBufferId);
	error = alGetError();
	if (error == AL_NO_ERROR)
	{
		qDebug() << "[SoundManager] loadSample - Queued buffer id: " << mSampleBufferId << " onto the sample source";
	}
	else
	{
		reportOpenALError(error);
	}

	qDebug() << "[SoundManager] loadSample - end";
}

/*bool SoundManager::load(QString filePath)
 {

 bool isSuccessful = false;

 //Check to see whether this file has already been loaded
 if (mSoundBuffers.contains(filePath))
 {

 ALuint bufferId = mSoundBuffers[filePath];
 ALuint sourceId = mAttachedSourceIds[bufferId];

 qDebug() << "XXXX Deleting old source id:" << sourceId << " attached to buffer id:" << bufferId << " for: " << filePath;

 //First we need to detach the source from that buffer
 alSourcei(sourceId, AL_BUFFER, 0);
 ALenum error = alGetError();
 if (error != AL_NO_ERROR)
 {
 qDebug() << "XXXX There was a problem detaching source id:" << sourceId << " from buffer id: " << bufferId;
 reportOpenALError(error);
 }

 alDeleteBuffers(1, &bufferId);
 error = alGetError();
 if (error != AL_NO_ERROR)
 {
 qDebug() << "XXXX There was a problem deleting buffer id:" << bufferId;
 reportOpenALError(error);
 }

 mSoundBuffers.remove(filePath);
 }

 //Create a buffer and fill it with data from the WAV file
 ALuint bufferId = alutCreateBufferFromFile(filePath.toStdString().c_str());

 if (bufferId != AL_NONE)
 {

 qDebug() << "XXXX Created buffer (id: " << bufferId << ") from file for: " << filePath;

 //Record the bufferId which this file was loaded into
 mSoundBuffers[filePath] = bufferId;

 //Attach this buffer to an available source
 ALuint sourceId = getNextAvailableSourceId();
 isSuccessful = attachBufferToSource(bufferId, sourceId);

 }
 else
 {

 qDebug() << "XXXX Failed to create buffer for: " << filePath;
 reportALUTError(alutGetError());
 }

 return isSuccessful;
 }*/

/*bool SoundManager::play(QString filePath, float pitch, float gain)
 {

 qDebug() << "XXXX Attempting to play file: " << filePath;

 //Get the buffer and source IDs
 ALuint bufferId = mSoundBuffers[filePath];
 ALuint sourceId = mAttachedSourceIds[bufferId];

 return playSource(sourceId, pitch, gain, false);
 }*/
/*
 bool SoundManager::stop(QString filePath)
 {

 //Get the buffer and source IDs
 ALuint bufferId = mSoundBuffers[filePath];
 ALuint sourceId = mAttachedSourceIds[bufferId];

 alSourceStop(sourceId);

 ALenum error = alGetError();
 if (error != AL_NO_ERROR)
 {
 reportOpenALError(error);
 return false;
 }

 return true;
 }*/

/*
 bool SoundManager::setPitch(QString filePath, float newPitch)
 {

 //Get the buffer and source IDs
 ALuint bufferId = mSoundBuffers[filePath];
 ALuint sourceId = mAttachedSourceIds[bufferId];

 qDebug() << "Attempting to change pitch of buffer ID: " << bufferId << " attached to source ID: " << sourceId;

 alSourcef(sourceId, AL_PITCH, newPitch);

 ALenum error = alGetError();
 if (error != AL_NO_ERROR)
 {
 reportOpenALError(error);
 return false;
 }

 return true;
 }*/

/*
 ALuint SoundManager::getNextAvailableSourceId()
 {

 static uint sourceIndex = 0;

 //Get the next available source
 ALuint sourceId = mSourceIds[sourceIndex];

 //Increment the source index so it points the next available source
 sourceIndex = (sourceIndex + 1) % SOUNDMANAGER_MAX_NBR_OF_SOURCES;

 qDebug() << "XXXX Incremented source index, it is now:" << sourceIndex;

 return sourceId;
 }
 */
/*
 bool SoundManager::attachBufferToSource(ALuint bufferId, ALuint sourceId)
 {

 bool isSuccessful = false;

 qDebug() << "XXXX Attaching buffer to source, id:" << sourceId;

 // Attach the buffer the available source.
 alSourcei(sourceId, AL_BUFFER, bufferId);

 ALenum error = alGetError();
 if (error == AL_NO_ERROR)
 {

 //Store the buffer id with the source id
 mAttachedSourceIds[bufferId] = sourceId;
 isSuccessful = true;

 }
 else
 {
 reportOpenALError(error);
 }

 return isSuccessful;
 }
 */
void SoundManager::playGuitar(float pitch, float gain)
{
	qDebug() << "[SoundManager] playGuitar - Pitch: " << pitch << " gain: " << gain;
	playSource(mGuitarSourceId, pitch, gain, false);
}

void SoundManager::playSample()
{
	qDebug() << "[SoundManager] playSample - start";
	playSource(mSampleSourceId, 1.0F, 1.0F, false);
}

bool SoundManager::playSource(ALuint sourceId, float pitch, float gain, bool shouldLoop)
{

	qDebug() << "[SoundManager] playSource - Playing source, id: " << sourceId << " pitch: " << pitch << " gain: " << gain << " loop? " << shouldLoop;

	ALenum error;
	//Set the source to loop
	/*if (shouldLoop)
	 alSourcei(sourceId, AL_LOOPING, AL_TRUE);

	 // Set the source pitch value.
	 alSourcef(sourceId, AL_PITCH, pitch);

	 error = alGetError();
	 if (error != AL_NO_ERROR)
	 {
	 reportOpenALError(error);
	 return false;
	 }

	 // Set the source gain value.
	 alSourcef(sourceId, AL_GAIN, gain);

	 error = alGetError();
	 if (error != AL_NO_ERROR)
	 {
	 reportOpenALError(error);
	 return false;
	 }
	 */
	// Play the source.
	alSourcePlay(sourceId);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		reportOpenALError(error);
		return false;
	}

	return true;

}
/*
 bool SoundManager::playSquareTone()
 {

 //Create a sound buffer with a Saw tone in it
 ALuint bufferId = alutCreateBufferWaveform(ALUT_WAVEFORM_SQUARE, 440.0, 0, 2);

 //Get the next available source
 ALuint sourceId = getNextAvailableSourceId();

 //Attach the buffer to the source
 if (attachBufferToSource(bufferId, sourceId))
 {

 //Now play it
 return playSource(sourceId, 1.0, 0.3, false);

 }
 else
 {
 return false;
 }

 }*/
