#pragma once
#ifndef IPLANET_NETWORKING_H
#define IPLANET_NETWORKING_H

#include "IPlanetNetworkingConfig.h"

namespace ipn
{
	enum result : bool
	{
		Good = true,
		Error = false
	};

	struct version
	{
		int major;
		int minor;
	};

	struct ipn_settings
	{
		version request_version;
	};

	result start_ipn(const ipn_settings& _settings);
	void shutdown_ipn();



}


// Socket buffer type

namespace ipn
{
	struct sockbuff;

	sockbuff* new_sockbuff();
	void delete_sockbuff(sockbuff*& _buff);

};


// Error handling

namespace ipn
{
	using error_code = int;
	
	struct error
	{
		error_code err;
	};

	// Throws an error into the running process
	void throw_error(error _err);
	void throw_error(error_code _ecode);


	// using error_callback = void(*)(const error_code& _err);

};


// Enumerators for things like ai_family and related
namespace ipn::bkl
{
	enum ai_family : int
	{
		afUnspec = 0,
		afIPv4 = 2,
		afINET = afIPv4,
		afIPv6 = 23,
		afINET6 = afIPv6
	};
	enum ai_socktype : int
	{
		skStream = 1,
		skDGram = 2
	};
	enum ai_protocol : int
	{
		pAuto = 0,
		pTCP = 6,
		pUDP = 17
	};
};



// This may be moved into a sub directory and hidden

#include <cstdint>

namespace ipn::bkl
{
	using socket_t = uint64_t;
	enum : socket_t
	{
		nullSock = 0
	};

	socket_t socket(ai_family _domain, ai_socktype _type, ai_protocol _protocol);
	result closesocket(socket_t& _sock);

};


// Socket type

namespace ipn::sk
{
	class socket_handle
	{
	protected:
		bkl::socket_t& bklsock() IPN_NOEXCEPT(true);
		const bkl::socket_t& bklsock() const IPN_NOEXCEPT(true);

	public:
		bool good() const IPN_NOEXCEPT(true);

		void release();
		void reset();

		const bkl::socket_t& get_bklsock() const IPN_NOEXCEPT(true);
		void set_bklsock(bkl::socket_t&& _sock);

		socket_handle(bkl::ai_family _domain, bkl::ai_socktype _type, bkl::ai_protocol _protocol);
		socket_handle(bkl::ai_family _domain, bkl::ai_socktype _type);

		socket_handle();

		explicit socket_handle(bkl::socket_t&& _bkSocket);
		socket_handle& operator=(bkl::socket_t&& _bkSocket);

		socket_handle(const socket_handle& other) = delete;
		socket_handle& operator=(const socket_handle& other) = delete;

		socket_handle(socket_handle&& other) IPN_NOEXCEPT(true);
		socket_handle& operator=(socket_handle&& other) IPN_NOEXCEPT(true);

		~socket_handle();

	private:
		bkl::socket_t sock_ = bkl::nullSock;
	};

};


namespace ipn
{



	struct connection_host
	{
	public:
		




		connection_host();
		~connection_host();

	private:
		sk::socket_handle listen_sock_;

	};


}

#endif