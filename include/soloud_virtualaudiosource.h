/*
  SoLoud audio engine
  Copyright (c) 2013-2015 Jari Komppa

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
  distribution.
*/
/*
  This file is for an altered version of SoLoud that allows subclassing
  SoLoud::AudioSource, SoLoud::Filter, SoLoud::AudioCollider, and
  SoLoud::AudioAttenuator through the C API.
*/

#ifndef SOLOUD_VIRTUALAUDIOSOURCE_H
#define SOLOUD_VIRTUALAUDIOSOURCE_H

#include "soloud.h"

namespace SoLoud
{	
	// Maximum number of virtual audio sources
	const unsigned int MAXIMUM_VIRTUAL_AUDIOSOURCES = 128;
	const unsigned int MAXIMUM_VIRTUAL_AUDIOCOLLIDERS = 128;
	const unsigned int MAXIMUM_VIRTUAL_AUDIOATTENUATORS = 128;
	

	class VirtualAudioCollider
	{
		float (*mCollide)(Soloud *, AudioSourceInstance3dData *, int);
	public:
		VirtualAudioCollider(float (*aCollide)(Soloud *, AudioSourceInstance3dData *, int));
		virtual float collide(Soloud *aSoloud, AudioSourceInstance3dData *aAudioInstance3dData,	int aUserData);
	};

	class VirtualAudioAttenuator
	{
		float (*mAttenuate)(float, float, float, float);
	public:
		VirtualAudioAttenuator(float (*aAttenuate)(float, float, float, float));
		virtual float attenuate(float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor);
	};


	class VirtualAudioSource;
	
	class VirtualAudioSourceInstance : public AudioSourceInstance
	{
		VirtualAudioSource *mParent;
		unsigned int mId;

		void (*mDestructor)();
		void (*mGetAudio)(float *, int);
		int (*mHasEnded)();
		void (*mSeek)(float, float *, int);
		int (*mRewind)();
		float (*mGetInfo)(unsigned int);

	public:
		virtual ~VirtualAudioSourceInstance();
		
		virtual void getAudio(float *aBuffer, unsigned int aSamples);
		virtual bool hasEnded();
		virtual void seek(time aSeconds, float *aScratch, unsigned int aScratchSize);
		virtual result rewind();
		virtual float getInfo(unsigned int aInfoKey);
		VirtualAudioSourceInstance(VirtualAudioSource *aParent, unsigned int aId,
		                           void (*aConstructor)(), void (*aDestructor)(),
		                           void (*aGetAudio)(float *, int), int (*aHasEnded)(),
		                           void (*aSeek)(float, float *, int), int (*aRewind)(),
		                           float (*aGetInfo)(unsigned int));
		unsigned int getId();
	};

	class VirtualAudioSource : public AudioSource
	{
		unsigned int mId;
		void (*mSetFilter)(unsigned int, Filter*);

		void (*mConstructor)();
		void (*mDestructor)();
		void (*mGetAudio)(float *, int);
		int (*mHasEnded)();
		void (*mSeek)(float, float *, int);
		int (*mRewind)();
		float (*mGetInfo)(unsigned int);

	public:
		enum CAPI_ACTION
		{
			GET,
			SET,
			REMOVE
		};

		virtual void setFilter(unsigned int aFilterId, Filter *aFilter);
		virtual AudioSourceInstance *createInstance();
		VirtualAudioSource(unsigned int aId, void (*aSetFilter)(unsigned int, Filter*),
		                   void (*aConstructor)(), void (*aDestructor)(),
		                   void (*aGetAudio)(float *, int), int (*aHasEnded)(),
		                   void (*aSeek)(float, float *, int), int (*aRewind)(),
		                   float (*aGetInfo)(unsigned int));
		unsigned int getId();
	};
}

#endif // SOLOUD_VIRTUALAUDIOSOURCE_H
