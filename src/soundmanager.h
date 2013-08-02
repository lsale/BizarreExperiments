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
class SoundManager
{
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
   * Load a sound
   *
   * @return bufferId
   */
  ALuint load(QString filePath);

  /**
   * Plays a sound, with modified pitch and gain.
   *
   * @param bufferId The ID of the buffer we wish to play, obtained using load
   * @param pitch Specifies the pitch to be applied to a sound Range: [0.5-2.0]
   * @param gain Sound gain (volume amplification) Range: ]0.0-  ]
   */
  bool play(ALuint bufferId, float pitch, float gain);

private:

  // Sound sources
  ALuint mSoundSourceIds[SOUNDMANAGER_MAX_NBR_OF_SOURCES];

  // Map of buffers which have been attached to sources. Key: bufferId, value: sourceId
  QHash<ALuint,ALuint> mAttachedSourceIds;

};

#endif //_SOUNDMANAGER_H
