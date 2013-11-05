/* Copyright (c) 2013 Research In Motion Limited.
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

	// Initialize the ALUT
	if (alutInit(NULL, NULL) == false)
	{
		reportALUTError(alutGetError());
	}

	// Generate 2 sources, one for the guitar and one for the sample loop
	alGenSources(1, &mGuitarSourceId);
	qDebug() << "[SoundManager] Constructor - Created source for guitar";

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

	//Delete the buffers
	alDeleteBuffers(1, &mGuitarBufferId);
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

bool SoundManager::stop(ALuint sourceId)
{

	alSourceStop(sourceId);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		reportOpenALError(error);
		return false;
	}

	return true;
}

void SoundManager::playGuitar(float pitch, float gain)
{
	qDebug() << "[SoundManager] playGuitar - Pitch: " << pitch << " gain: " << gain;
	playSource(mGuitarSourceId, pitch, gain, false);
}

bool SoundManager::playSource(ALuint sourceId, float pitch, float gain, bool shouldLoop)
{
	qDebug() << "[SoundManager] playSource - Playing source, id: " << sourceId << " pitch: " << pitch << " gain: " << gain << " loop? " << shouldLoop;

	ALenum error;
	//Set the source to loop
	if (shouldLoop)
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
