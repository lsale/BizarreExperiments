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

#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <QtCore/qstring.h>
#include <qhash.h>

// The number of max number of sound sources
#define SOUNDMANAGER_MAX_NBR_OF_SOURCES 16

/**
 * SoundManager Description:
 *
 * A very basic sound manager class for playing sounds using OpenAL library.
 */
class SoundManager {
public:

	/**
	 * This is our constructor which initializes the sound manager.
	 *
	 */
	SoundManager();

	/**
	 * This is our destructor which destroys all buffers and sources.
	 */
	~SoundManager();

	/**
	 * Load a WAV file, preparing it for playback
	 *
	 * @return true if loading was success, false if not
	 */
	bool load(QString filePath);

	/**
	 * Plays a sound, with modified pitch and gain.
	 *
	 * @param filePath Path of file we wish to play, must have been loaded first using @see load
	 * @param pitch Specifies the pitch to be applied to a sound Range: [0.5-2.0]
	 * @param gain Sound gain (volume amplification)
	 * @return true if buffer is able to be played
	 */
	bool play(QString filePath, float pitch, float gain);

	/**
	 * Stop playing the specified sound
	 *
	 * @param filePath Path of file we wish to stop playing
	 *
	 */
	bool stop(QString filePath);

	bool setPitch(QString filePath, float newPitch);

	bool playSquareTone();

	void playGuitar(float pitch, float gain);

private:

	// Sound buffers (Key: filename, Value: bufferId)
	QHash<QString, ALuint> mSoundBuffers;

	// Sound sources
	ALuint mGuitarSourceId, mDrumLoopSourceId;

	ALuint mSourceIds[SOUNDMANAGER_MAX_NBR_OF_SOURCES];

	// Map of buffers which have been attached to sources. Key: bufferId, value: sourceId
	QHash<ALuint, ALuint> mAttachedSourceIds;

	/**
	 * Gets the next available source ID.
	 *
	 * @return sourceId
	 */
	ALuint getNextAvailableSourceId();

	/**
	 * Attach a buffer to a source.
	 *
	 * @param bufferId the ID of the buffer
	 * @param sourceId the ID of the source
	 */
	bool attachBufferToSource(ALuint bufferId, ALuint sourceId);

	/**
	 * Play the given source
	 *
	 * @param sourceId the ID of the source
	 */
	bool playSource(ALuint sourceId, float pitch, float gain, bool shouldLoop);


};

#endif //_SOUNDMANAGER_H
