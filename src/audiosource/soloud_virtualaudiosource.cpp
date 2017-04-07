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

#include "soloud.h"
#include "soloud_virtualaudiosource.h"

namespace SoLoud
{
	VirtualAudioSourceInstance::VirtualAudioSourceInstance(VirtualAudioSource *aParent, unsigned int aId,
	                                                       void (*aConstructor)(), void (*aDestructor)(),
	                                                       void (*aGetAudio)(float *, int), int (*aHasEnded)(),
	                                                       void (*aSeek)(float, float *, int), int (*aRewind)(),
	                                                       float (*aGetInfo)(unsigned int))
	{
		mId = aId;
		mParent = aParent;

		mDestructor = aDestructor;
		mGetAudio = aGetAudio;
		mHasEnded = aHasEnded;
		mSeek = aSeek;
		mRewind = aRewind;
		mGetInfo = aGetInfo;

		if (aConstructor)
			aConstructor();
	}

	VirtualAudioSourceInstance::~VirtualAudioSourceInstance()
	{
		if (mDestructor)
			mDestructor();
	}

	unsigned int VirtualAudioSourceInstance::getId()
	{
		return mId;
	}

	void VirtualAudioSourceInstance::getAudio(float *aBuffer, unsigned int aSamples)
	{
		mGetAudio(aBuffer, aSamples);
	}
	
	bool VirtualAudioSourceInstance::hasEnded()
	{
		return mHasEnded();
	}
	
	void VirtualAudioSourceInstance::seek(time aSeconds, float *aScratch, unsigned int aScratchSize)
	{
		if (mSeek)
			mSeek(aSeconds, aScratch, aScratchSize);
		else
			AudioSourceInstance::seek(aSeconds, aScratch, aScratchSize);
	}
	
	result VirtualAudioSourceInstance::rewind()
	{
		if (mRewind)
			return mRewind();
		return AudioSourceInstance::rewind();
	}
	
	float VirtualAudioSourceInstance::getInfo(unsigned int aInfoKey)
	{
		if (mGetInfo)
			return mGetInfo(aInfoKey);
		return AudioSourceInstance::getInfo(aInfoKey);
	}


	VirtualAudioSource::VirtualAudioSource(unsigned int aId,
	                                       void (*aConstructor)(), void (*aDestructor)(),
	                                       void (*aGetAudio)(float *, int), int (*aHasEnded)(),
	                                       void (*aSeek)(float, float *, int), int (*aRewind)(),
	                                       float (*aGetInfo)(unsigned int))
	{
		mId = aId;

		mConstructor = aConstructor;
		mDestructor = aDestructor;
		mGetAudio = aGetAudio;
		mHasEnded = aHasEnded;
		mSeek = aSeek;
		mRewind = aRewind;
		mGetInfo = aGetInfo;
	}

	unsigned int VirtualAudioSource::getId()
	{
		return mId;
	}

	AudioSourceInstance *VirtualAudioSource::createInstance()
	{
		return new VirtualAudioSourceInstance(this, mId, mConstructor, mDestructor,
		                                      mGetAudio, mHasEnded, mSeek, mRewind,
		                                      mGetInfo);
	}
}
