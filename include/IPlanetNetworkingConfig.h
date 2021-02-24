#pragma once
#ifndef IPLANET_NETWORKING_CONFIG_H
#define IPLANET_NETWORKING_CONFIG_H

#if defined(IPLANET_NETWORKING_CONFIG_OVERRIDE) || !defined(IPLANET_NETWORKING_CMAKE)
#define IPLANET_NETWORKING_CONFIG_OVERRIDE true
#else
#define IPLANET_NETWORKING_CONFIG_OVERRIDE false
#endif

#if !IPLANET_NETWORKING_CONFIG_OVERRIDE || !defined(IPLANET_NETWORKING_DEBUG)
	#ifndef NDEBUG
		#define IPLANET_NETWORKING_DEBUG
	#endif
#endif

#if !IPLANET_NETWORKING_CONFIG_OVERRIDE || !defined(IPLANET_NETWORKING_EXCEPTIONS)
#define IPLANET_NETWORKING_EXCEPTIONS
#endif

#if !IPLANET_NETWORKING_CONFIG_OVERRIDE || !defined(IPLANET_NETWORKING_USE_SAELIB)
#define IPLANET_NETWORKING_USE_SAELIB
#endif

#define IPN_MSTR(a) #a
#define IPLANET_NETWORKING_NO_TARGET_DEFINITION(function) static_assert(false, "IPlanetNetworking::" IPN_MSTR(function)  "() has no definition for the target OS" )

namespace ipn
{
	constexpr static bool ipn_exceptions_v =
#ifdef IPLANET_NETWORKING_EXCEPTIONS
		true;
#else
		false;
#endif
	
	constexpr static bool ipn_noexcept_v = !ipn_exceptions_v;

};

#define IPLANET_NETWORKING_SOCKET_LIB_NULL 0
#define IPLANET_NETWORKING_SOCKET_LIB_WINSOCK 1
#define IPLANET_NETWORKING_SOCKET_LIB_UNIX 2
namespace ipn
{
	enum SOCKET_LIBRARY_E : int
	{
		libNull = IPLANET_NETWORKING_SOCKET_LIB_NULL,
		libWinsock = IPLANET_NETWORKING_SOCKET_LIB_WINSOCK,
		libUnix = IPLANET_NETWORKING_SOCKET_LIB_UNIX
	};
}

#if !defined(IPLANET_NETWORKING_SOCKET_LIB)
	#ifdef IPLANET_NETWORKING_USE_SAELIB
		#include <SAELib_OS.h>
		#if SAELIB_OS_WINDOWS
			#define IPLANET_NETWORKING_SOCKET_LIB IPLANET_NETWORKING_SOCKET_LIB_WINSOCK
		#elif SAELIB_OS_MAC or SAELIB_OS_APPLE
			#define IPLANET_NETWORKING_SOCKET_LIB IPLANET_NETWORKING_SOCKET_LIB_NULL
		#elif SAELIB_OS_UNIX or SAELIB_OS_LINUX or SAELIB_OS_POSIX
			#define IPLANET_NETWORKING_SOCKET_LIB IPLANET_NETWORKING_SOCKET_LIB_UNIX
		#endif
	#else
		#define IPLANET_NETWORKING_SOCKET_LIB IPLANET_NETWORKING_SOCKET_LIB_NULL
	#endif
#endif

#if (IPLANET_NETWORKING_SOCKET_LIB == IPLANET_NETWORKING_SOCKET_LIB_WINSOCK)
	#define IPN_WINSOCK
#elif (IPLANET_NETWORKING_SOCKET_LIB == IPLANET_NETWORKING_SOCKET_LIB_UNIX)
	#define IPN_UNIXSOCK
#elif (IPLANET_NETWORKING_SOCKET_LIB == IPLANET_NETWORKING_SOCKET_LIB_NULL)
	#define IPN_NULLSOCK
	#error "No networking socket library was set"
#endif

#ifdef IPN_WINSOCK
#define IPN_WINSOCK_V true
#else
#define IPN_WINSOCK_V false
#endif

#ifdef IPN_UNIXSOCK
#define IPN_UNIXSOCK_V true
#else
#define IPN_UNIXSOCK_V false
#endif

#ifdef IPN_NULLSOCK
#define IPN_NULLSOCK_V true
#else
#define IPN_NULLSOCK_V false
#endif

#ifdef IPLANET_NETWORKING_DEBUG
#define IPN_DEBUG
#define IPN_DEBUG_V true
#else
#define IPN_DEBUG_V false
#endif

#include <utility>

#ifndef IPN_PANIC
#define IPN_PANIC(...) std::terminate()
#endif

#if IPN_DEBUG_V
#define IPN_ASSERT(conditionVal) if (!(conditionVal)) { IPN_PANIC(); } else {}
#else
#define IPN_ASSERT(conditionVal) static_assert(true,"")
#endif

namespace ipn
{
	constexpr static SOCKET_LIBRARY_E ipn_socket_library_v = (SOCKET_LIBRARY_E)IPLANET_NETWORKING_SOCKET_LIB;
};


#ifndef IPN_CONSTEXPR
#define IPN_CONSTEXPR constexpr
#endif

#ifndef IPN_UNLIKELY
#define IPN_UNLIKELY [[unlikely]]
#endif

#ifndef IPN_LIKELY
#define IPN_LIKELY [[likely]]
#endif

#ifndef IPN_NOEXCEPT
#define IPN_NOEXCEPT(condition) noexcept(condition)
#endif

#ifndef IPN_DEPRECATED
#define IPN_DEPRECATED(msg) [[deprecated( msg )]]
#endif

#endif
