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

// Error message function for ALUT.
static void reportALUTError() {
	qDebug() << "ALUT reported the following error: "
			<< alutGetErrorString(alutGetError());
}

// Error message function for OpenAL.
static void reportOpenALError() {
	qDebug() << "OpenAL reported the following error: \n"
			<< alutGetErrorString(alGetError());
}

SoundManager::SoundManager() {

	// Initialize the ALUT
	if (alutInit(NULL, NULL) == false) {
		reportALUTError();
	}

	// Generate a number of sources used to attach buffers and play.
	alGenSources(SOUNDMANAGER_MAX_NBR_OF_SOURCES, mSoundSourceIds);

	if (alGetError() != AL_NO_ERROR) {
		reportOpenALError();
	}
}

SoundManager::~SoundManager() {

	// Clear all the sources.
	for (int sourceIndex = 0; sourceIndex < SOUNDMANAGER_MAX_NBR_OF_SOURCES;
			sourceIndex++) {
		ALuint source = mSoundSourceIds[sourceIndex];
		alDeleteSources(1, &source);

		if (alGetError() != AL_NO_ERROR) {
			reportOpenALError();
		}
	}

	// Clear the buffers by iterating through the hash table keys
	QHashIterator<ALuint, ALuint> iterator(mAttachedSourceIds);

	while (iterator.hasNext()) {
		iterator.next();

		// Get the buffer id and delete it.
		ALuint bufferId = iterator.key();
		alDeleteBuffers(1, &bufferId);

		if (alGetError() != AL_NO_ERROR) {
			reportOpenALError();
		}
	}

	// Clear the QHash for buffer IDs.
	mAttachedSourceIds.clear();

	// Exit the ALUT.
	if (alutExit() == false) {
		reportALUTError();
	}
}

/**
 * Load a WAV file into an OpenAL buffer
 *
 * @return the id of the sound buffer, or 0 if the loading fails
 *
 */
ALuint SoundManager::load(QString filePath) {

	ALuint bufferId = alutCreateBufferFromFile(filePath.toStdString().c_str());

	qDebug() << "Created buffer (id: " << bufferId << ") from file for: " << filePath;

	if (alutGetError() != ALUT_ERROR_NO_ERROR) {
		reportALUTError();
	}

	return bufferId;
}

bool SoundManager::play(ALuint bufferId, float pitch, float gain) {

	qDebug() << "Attempting to play buffer with id: " << bufferId;

	static uint unusedSourceIndex = 0; //The source index keeps track of unused sources

	ALuint sourceId;

	//If the buffer isn't already attached to a source then attach it
	if (!mAttachedSourceIds.contains(bufferId)) {

		qDebug() << "Buffer is not attached to a source";

		//Get the next available source
		if (unusedSourceIndex > SOUNDMANAGER_MAX_NBR_OF_SOURCES) {
			qDebug() << "No available sources, all "
					<< SOUNDMANAGER_MAX_NBR_OF_SOURCES << " are in use";
			return false;
		}

		sourceId = mSoundSourceIds[unusedSourceIndex];
		qDebug() << "Attaching to sourceId: " << sourceId;

		unusedSourceIndex++;

		//Attach the buffer to it
		alSourcei(sourceId, AL_BUFFER, bufferId);

		if (alGetError() != AL_NO_ERROR) {
			reportOpenALError();
			return false;
		}

		mAttachedSourceIds.insert(bufferId, sourceId);

	} else {

		//This buffer is already attached to a source
		sourceId = mAttachedSourceIds.value(bufferId);
	}

	// Set the source pitch value.
	alSourcef(sourceId, AL_PITCH, pitch);

	if (alGetError() != AL_NO_ERROR) {
		reportOpenALError();
		return false;
	}

	// Set the source gain value.
	alSourcef(sourceId, AL_GAIN, gain);

	if (alGetError() != AL_NO_ERROR) {
		reportOpenALError();
		return false;
	}

	// Play the source.
	alSourcePlay(sourceId);

	if (alGetError() != AL_NO_ERROR) {
		reportOpenALError();
		return false;
	}

	return true;
}
