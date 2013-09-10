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

//Error message functions
static void reportALUTError(ALenum error) {
	qDebug() << "ALUT reported the following error: "
			<< alutGetErrorString(error);
}

// Error message function for OpenAL.
static void reportOpenALError(ALenum error) {
	qDebug() << "OpenAL reported the following error: \n" << alGetString(error);
}

SoundManager::SoundManager() {

	// Initialize the ALUT
	if (alutInit(NULL, NULL) == false) {
		reportALUTError(alutGetError());
	}

	// Generate a number of sources used to attach buffers and play.
	alGenSources(SOUNDMANAGER_MAX_NBR_OF_SOURCES, mSourceIds);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		reportOpenALError(error);
	}
}

SoundManager::~SoundManager() {

	// Clear all the sources.
	for (int sourceIndex = 0; sourceIndex < SOUNDMANAGER_MAX_NBR_OF_SOURCES;
			sourceIndex++) {
		ALuint source = mSourceIds[sourceIndex];
		alDeleteSources(1, &source);

		ALenum error = alGetError();
		if (error != AL_NO_ERROR) {
			reportOpenALError(error);
		}
	}

	// Clear the buffers by iterating through the hash table keys
	QHashIterator<ALuint, ALuint> iterator(mAttachedSourceIds);

	while (iterator.hasNext()) {
		iterator.next();

		// Get the buffer id and delete it.
		ALuint bufferId = iterator.key();
		alDeleteBuffers(1, &bufferId);

		ALenum error = alGetError();
		if (error != AL_NO_ERROR) {
			reportOpenALError(error);
		}
	}

	// Clear the QHash for buffer IDs.
	mAttachedSourceIds.clear();

	// Exit the ALUT.
	if (alutExit() == false) {
		reportALUTError(alutGetError());
	}
}

bool SoundManager::load(QString filePath) {

	bool isSuccessful = false;

	//Check to see whether this file has already been loaded
	if (mSoundBuffers.contains(filePath)) {

		ALuint bufferId = mSoundBuffers[filePath];
		ALuint sourceId = mAttachedSourceIds[bufferId];

		qDebug() << "XXXX Deleting old source id:" << sourceId
				<< " attached to buffer id:" << bufferId << " for: "
				<< filePath;

		//First we need to detach the source from that buffer
		alSourcei(sourceId, AL_BUFFER, 0);
		ALenum error = alGetError();
		if (error != AL_NO_ERROR) {
			qDebug() << "XXXX There was a problem detaching source id:"
					<< sourceId << " from buffer id: " << bufferId;
			reportOpenALError(error);
		}

		alDeleteBuffers(1, &bufferId);
		error = alGetError();
		if (error != AL_NO_ERROR) {
			qDebug() << "XXXX There was a problem deleting buffer id:"
					<< bufferId;
			reportOpenALError(error);
		}

		mSoundBuffers.remove(filePath);
	}

	//Create a buffer and fill it with data from the WAV file
	ALuint bufferId = alutCreateBufferFromFile(filePath.toStdString().c_str());

	if (bufferId != AL_NONE) {

		qDebug() << "XXXX Created buffer (id: " << bufferId
				<< ") from file for: " << filePath;

		//Record the bufferId which this file was loaded into
		mSoundBuffers[filePath] = bufferId;

		//Attach this buffer to an available source
		ALuint sourceId = getNextAvailableSourceId();
		isSuccessful = attachBufferToSource(bufferId, sourceId);

	} else {

		qDebug() << "XXXX Failed to create buffer for: " << filePath;
		reportALUTError(alutGetError());
	}

	return isSuccessful;
}

bool SoundManager::play(QString filePath, float pitch, float gain) {

	qDebug() << "XXXX Attempting to play file: " << filePath;

	//Get the buffer and source IDs
	ALuint bufferId = mSoundBuffers[filePath];
	ALuint sourceId = mAttachedSourceIds[bufferId];

	return playSource(sourceId, pitch, gain, false);
}

bool SoundManager::stop(QString filePath) {

	//Get the buffer and source IDs
	ALuint bufferId = mSoundBuffers[filePath];
	ALuint sourceId = mAttachedSourceIds[bufferId];

	alSourceStop(sourceId);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		reportOpenALError(error);
		return false;
	}

	return true;
}

bool SoundManager::setPitch(QString filePath, float newPitch) {

	//Get the buffer and source IDs
	ALuint bufferId = mSoundBuffers[filePath];
	ALuint sourceId = mAttachedSourceIds[bufferId];

	qDebug() << "Attempting to change pitch of buffer ID: " << bufferId
			<< " attached to source ID: " << sourceId;

	alSourcef(sourceId, AL_PITCH, newPitch);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		reportOpenALError(error);
		return false;
	}

	return true;
}

ALuint SoundManager::getNextAvailableSourceId() {

	static uint sourceIndex = 0;

	//Get the next available source
	ALuint sourceId = mSourceIds[sourceIndex];

	//Increment the source index so it points the next available source
	sourceIndex = (sourceIndex + 1) % SOUNDMANAGER_MAX_NBR_OF_SOURCES;

	qDebug() << "XXXX Incremented source index, it is now:" << sourceIndex;

	return sourceId;
}

bool SoundManager::attachBufferToSource(ALuint bufferId, ALuint sourceId) {

	bool isSuccessful = false;

	qDebug() << "XXXX Attaching buffer to source, id:" << sourceId;

	// Attach the buffer the available source.
	alSourcei(sourceId, AL_BUFFER, bufferId);

	ALenum error = alGetError();
	if (error == AL_NO_ERROR) {

		//Store the buffer id with the source id
		mAttachedSourceIds[bufferId] = sourceId;
		isSuccessful = true;

	} else {
		reportOpenALError(error);
	}

	return isSuccessful;
}

bool SoundManager::playSource(ALuint sourceId, float pitch, float gain, bool shouldLoop) {

	//Set the source to loop
	if (shouldLoop) alSourcei(sourceId, AL_LOOPING, AL_TRUE);

	// Set the source pitch value.
	alSourcef(sourceId, AL_PITCH, pitch);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		reportOpenALError(error);
		return false;
	}

	// Set the source gain value.
	alSourcef(sourceId, AL_GAIN, gain);

	error = alGetError();
	if (error != AL_NO_ERROR) {
		reportOpenALError(error);
		return false;
	}

	// Play the source.
	alSourcePlay(sourceId);

	error = alGetError();
	if (error != AL_NO_ERROR) {
		reportOpenALError(error);
		return false;
	}

	return true;

}

bool SoundManager::playSquareTone() {

	//Create a sound buffer with a Saw tone in it
	ALuint bufferId = alutCreateBufferWaveform(ALUT_WAVEFORM_SQUARE, 440.0, 0,
			2);

	//Get the next available source
	ALuint sourceId = getNextAvailableSourceId();

	//Attach the buffer to the source
	if (attachBufferToSource(bufferId, sourceId)) {

		//Now play it
		return playSource(sourceId, 1.0, 0.3, false);

	} else {
		return false;
	}

}
