#include "IPlanetNetworking.h"
#include "SockLib/SockLib.h"

#ifndef IPN_NOCOMP 
#define IPN_NOCOMP static_assert(false, "")
#endif

#undef min
#undef max

#include <algorithm>

namespace ipn
{
	template <typename T>
	concept comparable = requires (T& _a, T& _b)
	{
		_a < _b;
	};

	template <comparable T>
	constexpr static const T& min(const T& _lhs, const T& _rhs) noexcept
	{
		return ((_lhs < _rhs) ? _lhs : _rhs);
	};
	template <comparable T>
	constexpr static const T& max(const T& _lhs, const T& _rhs) noexcept
	{
		return ((_rhs < _lhs) ? _lhs : _rhs);
	};

};


namespace ipn
{
	
	result start_ipn(const ipn_settings& _settings)
	{
		auto _rawResult = bkl::slib::start_socklib(_settings);
		auto _result = bkl::slib::convert_result(_rawResult);
		return _result;
	};
	void shutdown_ipn()
	{
		auto _rawResult = bkl::slib::shutdown_socklib();
		auto _result = bkl::slib::convert_result(_rawResult);
		if (!_result)
		{
			throw_error(_rawResult);
		};
	};

};


#include <array>
#include <list>
#include <cstdint>
#include <vector>

#ifndef IPN_ASSERT
#include <cassert>
#define IPN_ASSERT(conditionBool) assert(conditionBool)
#endif

namespace ipn
{
#define IPN_SOCKBUFF_USE_STD_LIST true

#ifndef IPN_SOCKBUFF_USE_STD_LIST
#define IPN_SOCKBUFF_USE_STD_LIST false
#endif

	class sockbuff
	{
	public:
		using value_type = char;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using size_type = size_t;

	private:
		constexpr static size_type block_size_v = 1024;
		constexpr static size_type block_size() noexcept { return block_size_v; };

#if IPN_SOCKBUFF_USE_STD_LIST
		struct charblock
		{
			size_type head = 0;
			size_type rhead = 0;
			value_type data[block_size_v]{ 0 };
		};
		std::list<charblock> data_{};
#else
		struct charblock
		{
			size_type head = 0;
			size_type rhead = 0;
			charblock* next = nullptr;
			charblock* previous = nullptr;
			value_type data[block_size_v]{ 0 };
		};

		charblock* first_ = nullptr;
		charblock* last_ = nullptr;
#endif
		auto front() noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return &this->data_.front();
#else
			return this->first_;
#endif
		};
		auto back() noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return &this->data_.back();
#else
			return this->last_;
#endif
		};

		auto front() const noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return &this->data_.front();
#else
			return this->first_;
#endif
		};
		auto back() const noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return &this->data_.back();
#else
			return this->last_;
#endif
		};

		auto begin() noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return this->data_.begin();
#else
			return this->first_;
#endif
		};
		auto begin() const noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return this->data_.begin();
#else
			return this->first_;
#endif
		};

		auto end() noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return this->data_.end();
#else
			return this->last_ + 1;
#endif
		};
		auto end() const noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return this->data_.end();
#else
			return this->last_ + 1;
#endif
		};

		bool empty() const noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return this->data_.empty();
#else
			return this->first_ == this->last_;
#endif
		};

		void push_back()
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			this->data_.push_back({});
#else
			auto _last = this->back();
			auto _node = new charblock{};
			_node->previous = _last;
			if (_last)
			{
				_last->next = _node;
			};
			this->last_ = _node;
#endif
		};
		void push_front()
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			this->data_.push_front({});
#else
			IPN_NOCOMP;
#endif
		};

		void pop_front()
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			this->data_.pop_front();
#else
			IPN_NOCOMP;
#endif
		};
		void pop_back()
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			this->data_.pop_back();
#else
			IPN_NOCOMP;
#endif
		};

		size_type size() const
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			return this->data_.size();
#else
			IPN_NOCOMP;
#endif
		};
		void clear_f() noexcept
		{
#if IPN_SOCKBUFF_USE_STD_LIST
			this->data_.clear();
#else
			IPN_NOCOMP;
#endif
		};

	public:
		void write(const value_type* _data, const size_type _len)
		{
			constexpr auto _blockSize = block_size();
			auto _rcount = 0;
			if (this->empty()) [[unlikely]]
			{
				this->push_back();
			};

			auto _last = this->back();
			while (_rcount != _len)
			{
				if (_last->head >= this->block_size())
				{
					this->push_back();
					_last = this->back();
				};
				const auto _blockEnd = _last->data + _last->head;
				const auto _wCount = min(_blockSize - _last->head, _len - _rcount);
				std::copy_n(_data + _rcount, _wCount, _blockEnd);
				_last->head += _wCount;
				_rcount += _wCount;
			};

		};
		
		bool can_read() const noexcept
		{
			return !this->empty() && (this->front()->rhead < this->front()->head);
		};	
		size_type read(value_type* _writeTo, const size_type _max)
		{
			IPN_ASSERT(this->can_read());

			const auto _at = this->front();
			const auto _rbegin = _at->data + _at->rhead;
			const auto _rend = _at->data + _at->head;
			const auto _count = min<size_t>(_max, std::distance(_rbegin, _rend));

			std::copy(_rbegin, _rend, _writeTo);
			_at->rhead += _count;

			if (_at->rhead == block_size())
			{
				this->pop_front();
			};
			
			return _count;
		};

		void clear()
		{
			this->clear_f();
		};

	};

	sockbuff* new_sockbuff()
	{ 
		return new sockbuff{}; 
	};
	void delete_sockbuff(sockbuff*& _buff) 
	{
		delete _buff;
		_buff = nullptr;
	};


}

#include <SAELib_ThreadQueue.h>

namespace ipn
{
	using error_queue = sae::thread_queue<error>;




	void throw_error(error _err)
	{
		IPN_PANIC();
	};
	void throw_error(error_code _ecode)
	{
		throw_error(error{ _ecode });
	};

};

namespace ipn::bkl
{
	socket_t socket(ai_family _domain, ai_socktype _type, ai_protocol _protocol)
	{
		auto _sockResult = slib::socket((int)_domain, (int)_type, (int)_protocol);
		auto _result = slib::is_socket_valid(_sockResult);
		if (!_result) IPN_UNLIKELY
		{
			throw_error((error_code)slib::get_last_error());
		};
		return _sockResult;
	};
	result closesocket(socket_t& _sock)
	{
		const auto _rawResult = slib::closesocket(_sock);
		const auto _result = slib::convert_result(_rawResult);
		_sock = nullSock;
		if (!_result) IPN_UNLIKELY
		{
			throw_error((error_code)slib::get_last_error());
		};
		return _result;
	};
};

namespace ipn::sk
{
	bkl::socket_t& socket_handle::bklsock() IPN_NOEXCEPT(true)
	{
		return this->sock_;
	};
	const bkl::socket_t& socket_handle::bklsock() const IPN_NOEXCEPT(true)
	{
		return this->sock_;
	};

	bool socket_handle::good() const IPN_NOEXCEPT(true)
	{
		return this->bklsock() != bkl::nullSock;
	};

	void socket_handle::release()
	{
		this->bklsock() = bkl::nullSock;
	};
	void socket_handle::reset()
	{
		if (this->good())
		{
			bkl::closesocket(this->bklsock());
			IPN_ASSERT(!this->good());
		};
	};

	const bkl::socket_t& socket_handle::get_bklsock() const IPN_NOEXCEPT(true)
	{
		return this->bklsock();
	};
	void socket_handle::set_bklsock(bkl::socket_t&& _sock)
	{
		this->reset();
		this->bklsock() = std::exchange(_sock, bkl::nullSock);
	};

	socket_handle::socket_handle(bkl::socket_t&& _bkSocket) :
		sock_{ std::exchange(_bkSocket, bkl::nullSock) }
	{};
	socket_handle::socket_handle() :
		socket_handle{ bkl::nullSock }
	{};

	socket_handle::socket_handle(bkl::ai_family _domain, bkl::ai_socktype _type, bkl::ai_protocol _protocol) : 
		socket_handle{ bkl::socket(_domain, _type, _protocol) }
	{};
	socket_handle::socket_handle(bkl::ai_family _domain, bkl::ai_socktype _type) : 
		socket_handle{ _domain, _type, bkl::pAuto }
	{};

	socket_handle& socket_handle::operator=(bkl::socket_t&& _bkSocket)
	{
		this->set_bklsock(std::move(_bkSocket));
		return *this;
	};

	socket_handle::socket_handle(socket_handle&& other) IPN_NOEXCEPT(true) :
		sock_{ std::exchange(other.bklsock(), bkl::nullSock) }
	{};
	socket_handle& socket_handle::operator=(socket_handle&& other) IPN_NOEXCEPT(true)
	{
		this->set_bklsock(std::move(other.bklsock()));
		other.release();
		return *this;
	};

	socket_handle::~socket_handle()
	{
		this->reset();
	};
};

namespace ipn
{




	connection_host::connection_host() : 
		listen_sock_{}
	{

	};
	connection_host::~connection_host()
	{

	};

}
