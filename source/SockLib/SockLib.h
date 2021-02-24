#pragma once

#include "IPlanetNetworking.h"

#if IPN_WINSOCK_V

#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>

#undef min
#undef max

namespace ipn::bkl::slib
{
	struct WinsockWSADataTag {};
	using socket_t = ::SOCKET;

	int start_socklib(const ipn_settings& _settings);
	int shutdown_socklib();

	using ::socket;
	using ::closesocket;
	using ::bind;
	using ::listen;
	using ::accept;
	using ::shutdown;
	using ::send;
	using ::recv;

	IPN_CONSTEXPR static result convert_result(int _slibResult) IPN_NOEXCEPT(true)
	{
		return (_slibResult == SOCKET_ERROR) ? result::Error : result::Good;
	};
	IPN_CONSTEXPR static result is_socket_valid(const socket_t& _sock) IPN_NOEXCEPT(true)
	{
		return (_sock != INVALID_SOCKET) ? result::Good : result::Error;
	};

	error_code get_last_error();

};

#else
#error "No socket library is defined for the target OS"
#endif

