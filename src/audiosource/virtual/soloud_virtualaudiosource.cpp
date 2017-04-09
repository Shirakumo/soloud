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
	VirtualAudioSource::VirtualAudioSource(unsigned int classID)
	{
		this->mClassID = classID;
	}

	void (*VirtualAudioSource::getAudioC)(unsigned int, float *, int) = 0;
	int (*VirtualAudioSource::hasEndedC)(unsigned int) = 0;
	void (*VirtualAudioSource::seekC)(unsigned int, float, float *, int) = 0;
	int (*VirtualAudioSource::rewindC)(unsigned int) = 0;
	float (*VirtualAudioSource::getInfoC)(unsigned int, unsigned int) = 0;

	AudioSourceInstance *VirtualAudioSource::createInstance()
	{
		return new VirtualAudioSourceInstance(this);
	}

	unsigned int VirtualAudioSource::getClassID()
	{
		return this->mClassID;
	}
	
	VirtualAudioSourceInstance::VirtualAudioSourceInstance(VirtualAudioSource *aParent)
	{
		this->mParent = aParent;
	}

	void VirtualAudioSourceInstance::getAudio(float *aBuffer, unsigned int aSamples)
	{
		if(VirtualAudioSource::getAudioC)
			VirtualAudioSource::getAudioC(this->mParent->getClassID(), aBuffer, aSamples);
	}

	bool VirtualAudioSourceInstance::hasEnded()
	{
		if(VirtualAudioSource::hasEndedC)
			return VirtualAudioSource::hasEndedC(this->mParent->getClassID());
		return true;
	}

	void VirtualAudioSourceInstance::seek(time aSeconds, float *aScratch, unsigned int aScratchSize)
	{
		if(VirtualAudioSource::seekC)
			VirtualAudioSource::seekC(this->mParent->getClassID(), aSeconds,
																aScratch, aScratchSize);
	}

	result VirtualAudioSourceInstance::rewind()
	{
		if(VirtualAudioSource::rewindC)
			return VirtualAudioSource::rewindC(this->mParent->getClassID());
		return 0;
	}

	float VirtualAudioSourceInstance::getInfo(unsigned int aInfoKey)
	{
		if(VirtualAudioSource::getInfoC)
			return VirtualAudioSource::getInfoC(this->mParent->getClassID(), aInfoKey);
		return 0.0;
	}

	VirtualAudioCollider::VirtualAudioCollider(unsigned int classID){
		this->mClassID = classID;
	}

	float (*VirtualAudioCollider::collideC)(unsigned int, void *, void *, int) = 0;

	float VirtualAudioCollider::collide(Soloud *aSoloud, AudioSourceInstance3dData *aAudioInstance3dData,	int aUserData){
		if(VirtualAudioCollider::collideC)
			return VirtualAudioCollider::collideC(this->mClassID, (void *)aSoloud, (void *)aAudioInstance3dData, aUserData);
		return 0.0;
	}

	VirtualAudioAttenuator::VirtualAudioAttenuator(unsigned int classID){
		this->mClassID = classID;
	}

	float (*VirtualAudioAttenuator::attenuateC)(unsigned int, float, float, float, float) = 0;

	float VirtualAudioAttenuator::attenuate(float aDistance, float aMinDistance, float aMaxDistance, float aRolloffFactor){
		if(VirtualAudioAttenuator::attenuateC)
			return VirtualAudioAttenuator::attenuateC(this->mClassID, aDistance, aMinDistance, aMaxDistance, aRolloffFactor);
		return 0.0;
	}
}
