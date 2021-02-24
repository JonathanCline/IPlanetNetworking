#pragma once

#include "IPlanetNetworkingConfig.h"

#include <SAELib_Singleton.h>

namespace ipn
{
		template <typename T, typename TagT>
		static auto& get_thread_global()
		{
			return sae::get_singleton_thread_local<T, TagT>();
		};
		template <typename T, typename TagT>
		static auto& get_global()
		{
			return sae::get_singleton<T, TagT>();
		};
};
