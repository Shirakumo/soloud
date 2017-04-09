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
#include "soloud_virtualfilter.h"

namespace SoLoud
{
	VirtualFilter::VirtualFilter(unsigned int classID)
	{
		this->mClassID = classID;
	}

	void (*VirtualFilter::filterC)(unsigned int, float *, unsigned int, unsigned int, float, time) = 0;
	void (*VirtualFilter::filterChannelC)(unsigned int, float *, unsigned int, float, time, unsigned int, unsigned int) = 0;

	FilterInstance *VirtualFilter::createInstance()
	{
		return new VirtualFilterInstance(this);
	}

	unsigned int VirtualFilter::getClassID()
	{
		return this->mClassID;
	}
	
	VirtualFilterInstance::VirtualFilterInstance(VirtualFilter *aParent)
	{
		this->mParent = aParent;
	}

	void VirtualFilterInstance::filter(float *aBuffer, unsigned int aSamples,
	                                   unsigned int aChannels, float aSamplerate,
	                                   time aTime)
	{
		if(VirtualFilter::filterC)
			VirtualFilter::filterC(this->mParent->getClassID(), aBuffer, aSamples, aChannels,
														 aSamplerate, aTime);
	}

	void VirtualFilterInstance::filterChannel(float *aBuffer, unsigned int aSamples,
	                                          float aSamplerate, time aTime,
	                                          unsigned int aChannel,
	                                          unsigned int aChannels)
	{
		if(VirtualFilter::filterChannelC)
			VirtualFilter::filterChannelC(this->mParent->getClassID(), aBuffer, aSamples,
																		aSamplerate, aTime, aChannel, aChannels);
	}
}
