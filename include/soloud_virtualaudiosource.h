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
	class VirtualAudioSource : public AudioSource
	{
	public:
		static void (*getAudioC)(void *, float *, int);
		static int (*hasEndedC)(void *);
		static void (*seekC)(void *, float, float *, int);
		static int (*rewindC)(void *);
		static float (*getInfoC)(void *, unsigned int);
		
		virtual AudioSourceInstance *createInstance();
		unsigned int getClassID();
		
		VirtualAudioSource();
	};
	
	class VirtualAudioSourceInstance : public AudioSourceInstance
	{
	private:
		VirtualAudioSource *mParent;
		
	public:
		virtual void getAudio(float *aBuffer, unsigned int aSamples);
		virtual bool hasEnded();
		virtual void seek(time aSeconds, float *aScratch, unsigned int aScratchSize);
		virtual result rewind();
		virtual float getInfo(unsigned int aInfoKey);
		
		VirtualAudioSourceInstance(VirtualAudioSource *aParent);
	};

	class VirtualAudioCollider : public AudioCollider
	{
	public:
		static float (*collideC)(void *, void *, void *, int);

		virtual float collide(Soloud *aSoloud, AudioSourceInstance3dData *aAudioInstance3dData,	int aUserData);
		unsigned int getClassID();

		VirtualAudioCollider();
	};

	class VirtualAudioAttenuator : public AudioAttenuator
	{
	public:
		static float (*attenuateC)(void *, float, float, float, float);

		virtual float attenuate(float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor);
		unsigned int getClassID();

		VirtualAudioAttenuator();
	};
}

#endif // SOLOUD_VIRTUALAUDIOSOURCE_H
