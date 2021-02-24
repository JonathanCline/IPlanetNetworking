#include "SockLib.h"

#include "IPlanetNetworking.h"

#include "Utility/Global.h"

namespace ipn::bkl::slib
{

	int start_socklib(const ipn_settings& _settings)
	{
		auto _wsaVersion = MAKEWORD(_settings.request_version.major, _settings.request_version.minor);
		auto& _wsaData = get_global<WSAData, WinsockWSADataTag>();
		return WSAStartup(_wsaVersion, &_wsaData);
	};
	int shutdown_socklib()
	{
		return WSACleanup();
	};

	error_code get_last_error()
	{
		return (error_code)WSAGetLastError();
	};


}