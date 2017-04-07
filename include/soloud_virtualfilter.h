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

#ifndef SOLOUD_VIRTUALFILTER_H
#define SOLOUD_VIRTUALFILTER_H

#include "soloud.h"

namespace SoLoud
{
	// Maximum number of virtual filters
	const unsigned int MAXIMUM_VIRTUAL_FILTERS = 128;

	class VirtualFilter;
	
	class VirtualFilterInstance : public FilterInstance
	{
		VirtualFilter *mParent;
		unsigned int mId;

		void (*mDestructor)();
		void (*mFilter)(float *, unsigned int, unsigned int, float, time);
		void (*mFilterChannel)(float *, unsigned int, float, time, unsigned int, unsigned int);

	public:
		virtual ~VirtualFilterInstance();
		virtual void filter(float *aBuffer, unsigned int aSamples, unsigned int aChannels,
		                    float aSamplerate, time aTime);
		virtual void filterChannel(float *aBuffer, unsigned int aSamples, float aSamplerate,
		                           time aTime, unsigned int aChannel, unsigned int aChannels);
		
		VirtualFilterInstance(VirtualFilter *aParent, unsigned int aId, int aNumParams,
		                      void (*aConstructor)(), void (*aDestructor)(),
		                      void (*aFilter)(float *, unsigned int, unsigned int, float, time),
		                      void (*aFilterChannel)(float *, unsigned int, float, time, unsigned int, unsigned int));
		unsigned int getId();
	};

	class VirtualFilter : public Filter
	{
		unsigned int mId;
		int mNumParams;
		void (*mConstructor)();
		void (*mDestructor)();
		void (*mFilter)(float *, unsigned int, unsigned int, float, time);
		void (*mFilterChannel)(float *, unsigned int, float, time, unsigned int, unsigned int);

	public:
		enum CAPI_ACTION
		{
			GET,
			SET,
			REMOVE
		};

		virtual FilterInstance *createInstance();
		VirtualFilter(unsigned int aId, int aNumParams, void (*aConstructor)(), void (*aDestructor)(),
		              void (*aFilter)(float *, unsigned int, unsigned int, float, time),
		              void (*aFilterChannel)(float *, unsigned int, float, time, unsigned int, unsigned int));
		unsigned int getId();
	};
}

#endif // SOLOUD_VIRTUALFILTER_H
