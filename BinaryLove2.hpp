#ifndef REDSKITTLEFOX_BINARY_LOVE_2_H_
#define REDSKITTLEFOX_BINARY_LOVE_2_H_
#pragma once

//#define consteval consteval

#ifdef _MSC_VER
#define force_inline __forceinline
#define no_vtable __declspec(novtable)
#elif
#define force_inline inline
#define no_vtable 
#endif

#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <stdexcept>

#include <typeinfo>

#define BINARYLOVE2_EXCEPTIONS

#ifdef BINARYLOVE2_EXCEPTIONS
#define THROW_OUT_OF_RANGE_EXCEPTION(MSG) throw std::out_of_range(MSG)
#else
#define THROW_OUT_OF_RANGE_EXCEPTION(MSG)
#endif

namespace BinaryLove2
{
	using byte_t = int8_t;

	namespace details
	{
		template<class T, typename = void> struct is_loop_iteration_count;

		template <template<class> class Trait, class T, class... Args>
		struct check_all_types {
			enum { value = Trait<T>::value && check_all_types<Trait, Args...>::value };
		};

		template <template<class> class Trait, class T>
		struct check_all_types<Trait, T> {
			enum { value = Trait<T>::value };
		};

		template<class T, typename = void>
		struct is_pod : std::false_type {};

		template<class T>
		struct is_pod<T, std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>>> : std::true_type {};

		template<typename Test, template<typename...> class Ref>
		struct is_specialization : std::false_type {};

		template<template<typename...> class Ref, typename... Args>
		struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

		template<typename Test, template<size_t...> class Ref>
		struct is_specialization_int : std::false_type {};

		template<template<size_t...> class Ref, size_t I>
		struct is_specialization_int<Ref<I>, Ref> : std::true_type {};

		template<typename Test, template<size_t, typename...> class Ref>
		struct is_specialization_itc : std::false_type {};

		template<template<size_t, typename...> class Ref, size_t I, typename... Args>
		struct is_specialization_itc<Ref<I, Args...>, Ref> : std::true_type {};

		template<typename Test, template<typename, size_t> class Ref>
		struct is_specialization_array : std::false_type {};

		template<template<typename, size_t> class Ref, typename T, size_t I>
		struct is_specialization_array<Ref<T, I>, Ref> : std::true_type {};

		namespace compatible_class_tratis
		{
			template<class T, typename = void>
			struct has_parse
			{
				static constexpr bool value = std::false_type::value;
			};

			template<class T>
			struct has_parse<T, std::enable_if_t<std::is_class_v<T> && !is_loop_iteration_count<T>::value>>
			{
				template<class U, size_t(U::*)(const byte_t*, size_t, std::map<uint32_t, size_t>&)> struct test_t {};
				template<class U> static int8_t test(test_t<U, &U::parse>*);
				template<class U> static int16_t test(...);
				static constexpr bool value = (sizeof(test<T>(0)) == sizeof(int8_t));
			};

			template<class T>
			struct has_parse<T, std::enable_if_t<is_loop_iteration_count<T>::value>> : std::true_type {};

			template<class T, typename = void>
			struct has_parse_small
			{
				static constexpr bool value = std::false_type::value;
			};

			template<class T>
			struct has_parse_small<T, std::enable_if_t<std::is_class_v<T>>>
			{
				template<class U, size_t(U::*)(const byte_t*, size_t)> struct test_t {};
				template<class U> static int8_t test(test_t<U, &U::parse>*);
				template<class U> static int16_t test(...);
				static constexpr bool value = (sizeof(test<T>(0)) == sizeof(int8_t));
			};

			template<class T, typename = void>
			struct has_unparse
			{
				static constexpr bool value = std::false_type::value;
			};

			template<class T>
			struct has_unparse<T, std::enable_if_t<std::is_class_v<T> && !is_loop_iteration_count<T>::value>>
			{
				template<class U, size_t(U::*)(byte_t*, size_t)> struct test_t {};
				template<class U> static int8_t test(test_t<U, &U::unparse>*);
				template<class U> static int16_t test(...);
				static constexpr bool value = (sizeof(test<T>(0)) == sizeof(int8_t));
			};

			template<class T>
			struct has_unparse<T, std::enable_if_t<is_loop_iteration_count<T>::value>> : std::true_type {};

			template<class T, typename = void>
			struct has_size
			{
				static constexpr bool value = std::false_type::value;
			};

			template<class T>
			struct has_size<T, std::enable_if_t<std::is_class_v<T> && !is_loop_iteration_count<T>::value>>
			{
				template<class U, size_t(U::*)() const> struct test_t {};
				template<class U> static int8_t test(test_t<U, &U::size>*);
				template<class U> static int16_t test(...);
				static constexpr bool value = (sizeof(test<T>(0)) == sizeof(int8_t));
			};
		
			template<class T>
			struct has_size<T, std::enable_if_t<is_loop_iteration_count<T>::value>> : std::true_type {};

			template<class T, typename = void>
			struct has_static_size
			{
				static constexpr bool value = std::false_type::value;
			};

			template<class T>
			struct has_static_size<T, std::enable_if_t<std::is_class_v<T>>>
			{
				template<class U, size_t(*)()> struct test_t {};
				template<class U> static int8_t test(test_t<U, &U::static_size>*);
				template<class U> static int16_t test(...);
				static constexpr bool value = (sizeof(test<T>(0)) == sizeof(int8_t));
			};

			template<class T, class = std::enable_if_t<std::is_class_v<T>>>
			struct has_is_const_size
			{
				static constexpr bool value = std::false_type::value;
			};

			template<class T>
			struct has_is_const_size<T, std::enable_if_t<std::is_class_v<T>>>
			{
				template<class U, bool(*)()> struct test_t {};
				template<class U> static int8_t test(test_t<U, &U::is_const_size>*);
				template<class U> static int16_t test(...);
				static constexpr bool value = (sizeof(test<T>(0)) == sizeof(int8_t));
			};
		}

		template<class T, class = void>
		struct is_bl2_compatible_class : std::false_type {};

		template<class T>
		struct is_bl2_compatible_class<T, std::enable_if_t<
			((compatible_class_tratis::has_parse<T>::value || compatible_class_tratis::has_parse_small<T>::value)
			&& compatible_class_tratis::has_unparse<T>::value
			&& compatible_class_tratis::has_size<T>::value 
			&& compatible_class_tratis::has_is_const_size<T>::value 
			&& (compatible_class_tratis::has_static_size<T>::value >= T::is_const_size()))
			|| is_loop_iteration_count<T>::value>> : std::true_type {};

		template<class... Args>
		constexpr force_inline size_t static_sizeof_args()
		{
			return static_sizeof_args<0, Args...>();
		}

		template<size_t I = 0, class... Args>
		constexpr force_inline size_t static_sizeof_args()
		{
			using curtype_t = std::tuple_element_t<I, std::tuple<Args...>>;
			size_t size = 0;

			if constexpr (details::is_bl2_compatible_class<curtype_t>::value)
				size += curtype_t::static_size();
			else
				if constexpr(details::is_pod<curtype_t>::value)
					size += sizeof(curtype_t);

			if constexpr (I + 1 < sizeof...(Args))
			{
				size += static_sizeof_args<I + 1, Args...>();
			}

			return size;
		}

		template<class... Args>
		force_inline size_t sizeof_args(const std::tuple<Args...>& val_)
		{
			return sizeof_args<0, Args...>(val_);
		}

		template<size_t I = 0, class... Args>
		force_inline size_t sizeof_args(const std::tuple<Args...>& val_)
		{
			using curtype_t = std::tuple_element_t<I, std::tuple<Args...>>;

			size_t size = 0;

			if constexpr (details::is_bl2_compatible_class<curtype_t>::value)
				if constexpr (curtype_t::is_const_size())
					size += curtype_t::static_size();
				else
					size += std::get<I, Args...>(val_).size();
			else
				if constexpr (details::is_pod<curtype_t>::value)
					size += sizeof(curtype_t);
		
			if constexpr (I + 1 < sizeof...(Args))
			{
				size += sizeof_args<I + 1, Args...>(val_);
			}

			return size;
		}

		template<size_t I = 0, class... Args>
		size_t details_parse(const byte_t* data_, size_t size_, std::tuple<Args...>& tuple_, std::map<uint32_t, size_t>& map_);

		template<size_t I, class... Args>
		size_t details_unparse(byte_t* data_, size_t size_, std::tuple<Args...>& tuple_);
	};

	template<class...> class layout;
	template<class...> class group;
	template<size_t, class...> class loop_iteration_count;
	template<size_t, class...> class loop;

	template<size_t Size>
	class padding
	{
		template<size_t I_, class... Args_>
		friend size_t details::details_parse<I_, Args_...>(const byte_t*, size_t, std::tuple<Args_...>&, std::map<uint32_t, size_t>&);

		template<size_t I_, class... Args_>
		friend size_t details::details_unparse<I_, Args_...>(byte_t* data_, size_t size_, std::tuple<Args_...>& tuple_);

	public:
		static constexpr force_inline bool is_const_size() noexcept { return true; };

		constexpr static force_inline size_t static_size() noexcept
		{
			return Size;
		}

		constexpr force_inline size_t size() const noexcept
		{
			return Size;
		}
	};

	template<class... Args>
	class group
	{
		template<class...> friend class layout;
		template<size_t, class...> friend class loop_iteration_count;
		template<size_t, class...> friend class loop;

		template<size_t I_, class... Args_>
		friend size_t details::details_parse<I_, Args_...>(const byte_t*, size_t, std::tuple<Args_...>&, std::map<uint32_t, size_t>&);

		template<size_t I_, class... Args_>
		friend size_t details::details_unparse<I_, Args_...>(byte_t*, size_t, std::tuple<Args_...>&);

		static_assert(details::check_all_types<details::is_bl2_compatible_type, Args...>::value,
			"Binary Love 2 Error: Type T is not compatible with Binary Love 2.");

		std::tuple<Args...> m_elements;

		template<size_t I, class... Args>
		static constexpr force_inline bool is_compile_time_constant_() noexcept
		{
			using curtype_t = std::tuple_element_t<I, std::tuple<Args...>>;

			if constexpr (!std::is_fundamental_v<curtype_t> && !details::is_pod<curtype_t>::value)
			{
				if constexpr(!(curtype_t::is_const_size()))
					return false;

			}
			
			if constexpr (I + 1 < sizeof...(Args))
				return is_compile_time_constant_<I + 1, Args...>();

			return true;
		}

	public:
		static constexpr force_inline bool is_const_size() noexcept 
		{ 
			return is_compile_time_constant_<0, Args...>();
		};
		
		template<typename = std::enable_if_t<is_const_size()>>
		static constexpr force_inline size_t static_size() noexcept
		{
			return details::static_sizeof_args<0, Args...>();
		}

		force_inline size_t size() const noexcept
		{
			if constexpr (is_const_size())
				return details::static_sizeof_args<0, Args...>();
			else
				return details::sizeof_args<0, Args...>(m_elements);
		};

	public:
		template<size_t I> 
		constexpr force_inline std::tuple_element_t<I, std::tuple<Args...>>& get() noexcept
		{
			return std::get<I>(m_elements);
		};

	// TODO: Make this private
	public:
		size_t parse(const byte_t* data_, size_t size_, std::map<uint32_t, size_t>& map_)
		{
			return details::details_parse<0, Args...>(data_, size_, m_elements, map_);
		}

		size_t unparse(byte_t* data_, size_t size_)
		{
 			return details::details_unparse<0, Args...>(data_, size_, m_elements);
		}
	};

	template<size_t Identifier, class... Args>
	class loop_iteration_count
	{
		template<class...> friend class layout;
		template<class...> friend class group;
		template<size_t, class...> friend class loop;

		template<size_t I_, class... Args_>
		friend size_t details::details_parse<I_, Args_...>(const byte_t*, size_t, std::tuple<Args_...>&, std::map<uint32_t, size_t>&);

		template<size_t I_, class... Args_>
		friend size_t details::details_unparse<I_, Args_...>(byte_t*, size_t, std::tuple<Args_...>&);

		static_assert(details::check_all_types<details::is_bl2_compatible_type, Args...>::value,
			"Binary Love 2 Error: Type T is not compatible with Binary Love 2.");

		group<Args...> m_group;
	public:
		static constexpr force_inline bool is_const_size() noexcept
		{
			return group<Args...>::is_const_size();
		};

		template<typename = std::enable_if_t<is_const_size()>>
		static constexpr force_inline size_t static_size() noexcept
		{
			return group<Args...>::static_size();
		}

		force_inline size_t size() const noexcept
		{
			return m_group.size();
		}

	public:
		virtual constexpr force_inline size_t parse_count() noexcept
		{
			return this->get<0>();
		};

		template<size_t I>
		constexpr force_inline std::tuple_element_t<I, std::tuple<Args...>>& get() noexcept
		{
			return m_group.get<I>();
		};

	// TODO: Make this private
	public:
		size_t parse(const byte_t* data_, size_t size_, std::map<uint32_t, size_t>& map_)
		{
			size_t size = m_group.parse(data_, 
				std::forward<size_t>(size_), 
				std::forward<std::map<uint32_t, size_t>& >(map_));

			map_[Identifier] = parse_count();

			return size;
		};
		
		size_t unparse(byte_t* data_, size_t size_)
		{
			return m_group.unparse(data_, size_);
		};
	};

	template<size_t Identifier = 0, class... Args>
	class loop
	{
		template<class...> friend class layout;
		template<class...> friend class group;
		template<size_t, class...> friend class loop_iteration_count;

		template<size_t I_, class... Args_>
		friend size_t details::details_parse<I_, Args_...>(const byte_t*, size_t, std::tuple<Args_...>&, std::map<uint32_t, size_t>&);

		template<size_t I_, class... Args_>
		friend size_t details::details_unparse<I_, Args_...>(byte_t*, size_t, std::tuple<Args_...>&);

		static_assert(details::check_all_types<details::is_bl2_compatible_type, Args...>::value,
			"Binary Love 2 Error: Type T is not compatible with Binary Love 2.");

	public:
		typedef group<Args...> group_t;
	private:
		std::vector<group_t> m_elements;
	public:
		static constexpr force_inline bool is_const_size() noexcept { return false; };

		force_inline size_t size() const noexcept
		{
			size_t size{ 0u };

			for (auto& e : m_elements)
			{
				if constexpr (is_const_size())
					size += group_t::static_size();
				else
					size += e.size();
			}

			return size;
		};

	public:
		typedef typename std::vector<group_t>::iterator iterator;
		typedef typename std::vector<group_t>::const_iterator const_iterator;
		typedef typename std::vector<group_t>::reverse_iterator reverse_iterator;
		typedef typename std::vector<group_t>::const_reverse_iterator const_reverse_iterator;

		constexpr iterator begin() noexcept { return begin(m_elements); };
		constexpr const_iterator cbegin() noexcept { return cbegin(m_elements); };
		constexpr iterator end() noexcept { return end(m_elements); };
		constexpr const_iterator cend() noexcept { return cend(m_elements); };

		constexpr reverse_iterator rbegin() noexcept { return rbegin(m_elements); };
		constexpr const_reverse_iterator crbegin() noexcept { return crbegin(m_elements); };
		constexpr reverse_iterator rend() noexcept { return rend(m_elements); };
		constexpr const_reverse_iterator crend() noexcept { return crend(m_elements); };

		group_t& operator[](const size_t pos_) { return m_elements[pos_]; };
		const group_t& operator[](const size_t pos_) const { return m_elements[pos_]; };
	
	// TODO: Make this private
	public:
		size_t parse(const byte_t* data_, size_t size_, std::map<uint32_t, size_t>& map_)
		{
			size_t count{ 0 }; // Not sure if size_t default val will always be 0x0b
			if(map_.contains(Identifier))
				count = map_[Identifier];

			m_elements.insert(m_elements.end(), count, group_t());

			size_t size{ 0u };

			// TODO: Optimise it here for to do less memcpy calls
			for (size_t i{ 0u }; i < m_elements.size(); ++i)
			{
				size += m_elements[i].parse(data_ + size, size_ - size, map_);
			}

			return size;
		};

		size_t unparse(byte_t* data_, size_t size_)
		{
			size_t size{ 0u };
			
			for (size_t i{ 0u }; i < m_elements.size(); ++i)
			{
				size += m_elements[i].unparse(data_ + size, size_ - size);
			}

			return size;
		};
	};


	template<class... Args>
	class layout
	{
		template<size_t I_, class... Args_>
		friend size_t details::details_parse<I_, Args_...>(const byte_t*, size_t, std::tuple<Args_...>&, std::map<uint32_t, size_t>&);

		template<size_t I_, class... Args_>
		friend size_t details::details_unparse<I_, Args_...>(byte_t* data_, size_t size_, std::tuple<Args_...>& tuple_);

		static_assert(details::check_all_types<details::is_bl2_compatible_type, Args...>::value,
			"Binary Love 2 Error: Type T is not compatible with Binary Love 2.");

		std::tuple<Args...> m_elements;
		std::map<uint32_t, size_t> m_loop_iteration_count_map;

	public:
		static constexpr force_inline bool is_const_size() noexcept 
		{ 
			return group<Args...>::is_const_size();
		};

		template<typename = std::enable_if_t<is_const_size()>>
		static constexpr force_inline size_t static_size() noexcept
		{
			return group<Args...>::static_size();
		}

		force_inline size_t size() const noexcept
		{
			return details::sizeof_args<0, Args...>(m_elements);
		};

	public:
		template<size_t I>
		constexpr force_inline std::tuple_element_t<I, std::tuple<Args...>>& get() noexcept
		{
			return std::get<I, Args...>(m_elements);
		};

	public:
		size_t parse(const byte_t* data_, size_t size_)
		{
			return details::details_parse<0, Args...>(data_, size_, m_elements, m_loop_iteration_count_map);
		};

		template<class T, class = std::enable_if_t<sizeof(T) == 1 && std::is_fundamental_v<T>>>
		size_t parse(const std::vector<T>& buffer_)
		{
			return parse((byte_t*)buffer_.data(), buffer_.size());
		}

		size_t unparse(byte_t* data_, size_t size_)
		{
			return details::details_unparse<0, Args...>(data_, size_, m_elements);
		};

		template<class T, class = std::enable_if_t<sizeof(T) == 1 && std::is_fundamental_v<T>>>
		std::vector<T> unparse()
		{
			std::vector<T> buffer(this->size());
			this->unparse((byte_t*)buffer.data(), buffer.size());
			return buffer;
		}
	};

	namespace details
	{
		template<template <size_t, typename...> class Base, class Derived>
		struct is_base_of_generic
		{
			using U = typename std::remove_cv_t<Derived>;

			template<size_t I_, class... Args_> static std::true_type test(Base<I_, Args_...>*);
			static std::false_type test(...);

			using type = decltype(test(std::declval<U*>()));
			static constexpr bool value = type::value;
		};

		template<class T, typename> struct is_loop_iteration_count : std::false_type {};
		template<class T> struct is_loop_iteration_count<T, std::enable_if_t<is_specialization_itc<T, loop_iteration_count>::value>> : std::true_type {};

		template<class T>
		struct is_loop_iteration_count <T,
			std::enable_if_t<is_base_of_generic<loop_iteration_count, T>::value>>
			: std::true_type {};


		template<class T, typename = void> struct is_bl2_compatible_type : std::false_type 
		{
			static_assert(compatible_class_tratis::has_size<T>::value,
				"Binary Love 2 Error: Incompatible class T. Class T has no member function with signature 'size_t size() const noexcept`.");
			
			static_assert(compatible_class_tratis::has_parse<T>::value || compatible_class_tratis::has_parse_small<T>::value,
				"Binary Love 2 Error: Incompatible class T. Class T has no member function with signature:"
				"`size_t parse(const byte_t*, size_t, std::map<uint32_t, size_t>&)`"
				"or"
				"'size_t parse(const byte_t*, size_t, std::map<uint32_t, size_t>&)`.");
			
			static_assert(compatible_class_tratis::has_unparse<T>::value,
				"Binary Love 2 Error: Incompatible class T. Class T has no member function with signature 'size_t unparse(byte_t*, size_t)`.");

			static_assert(compatible_class_tratis::has_is_const_size<T>::value,
				"Binary Love 2 Error: Incompatible class T. Class T has no static function with signature 'static constexpr bool is_const_size() noexcept`.");
			
			static_assert(compatible_class_tratis::has_static_size<T>::value >= T::is_const_size(),
				"Binary Love 2 Error: Incompatible class T. Class T is said to be of a const size but has no static function with signature 'constexpr size_t static_size() noexcept`.");
		};

		template<class T> struct is_bl2_compatible_type<T, 
			std::enable_if_t<
				details::is_pod<T>::value ||
				details::is_specialization_int<T, padding>::value ||
				details::is_specialization_itc<T, loop_iteration_count>::value ||
				details::is_specialization<T, group>::value ||
				details::is_specialization_itc<T, loop>::value ||
				details::is_loop_iteration_count<T>::value ||
				details::is_bl2_compatible_class<T>::value
			>> : std::true_type {};

		template<size_t I, class... Args>
		size_t details_parse(const byte_t* data_, size_t size_, std::tuple<Args...>& tuple_, std::map<uint32_t, size_t>& map_)
		{
			// TODO: Check if all types are pod && !bl2_compatible_class, perform one memcpy for everything

			using curtype_t = std::tuple_element_t<I, std::tuple<Args...>>;
			curtype_t& curelement_v = std::get<I>(tuple_);

			size_t size{ 0u };
			// No need to parse if we are padding
			if constexpr (is_specialization_int<curtype_t, padding>::value)
			{
				size = curtype_t::static_size();
			}
			else
			{
				if constexpr (is_bl2_compatible_class<curtype_t>::value)
				{
					// Perform size checking if is compile time known size
					if constexpr (curtype_t::is_const_size())
					{
						if (curtype_t::static_size() > size_)
						{
							throw std::out_of_range("Not enough memory left to parse T.");
							return 0u;
						}
					}

					if constexpr (compatible_class_tratis::has_parse<curtype_t>::value)
						size = curelement_v.parse(data_, size_, map_);
					else
						size = curelement_v.parse(data_, size_);
				}
				else
				{
					if constexpr (details::is_pod<curtype_t>::value)
					{
						if (sizeof(curtype_t) > size_)
						{
							THROW_OUT_OF_RANGE_EXCEPTION("Binary Love 2 Exception: Supplied data is smaller than the specified file layout.");
							return 0u;
						}

						memcpy(&(curelement_v), data_, sizeof(curtype_t));
						size = sizeof(curtype_t);
					}
				}
			}
			
			if constexpr (I + 1 < sizeof...(Args))
				size += details_parse<I + 1, Args...>(data_ + size, size_ - size, tuple_, map_);

			return size;
		}
	
		template<size_t I, class... Args>
		size_t details_unparse(byte_t* data_, size_t size_, std::tuple<Args...>& tuple_)
		{
			// TODO: Check if all types are pod && !bl2_compatible_class, perform one memcpy for everything

			using curtype_t = std::tuple_element_t<I, std::tuple<Args...>>;
			curtype_t& curelement_v = std::get<I>(tuple_);

			size_t size{ 0u };
			// No need to parse if we are padding
			if constexpr (is_specialization_int<curtype_t, padding>::value)
			{
				size = curtype_t::static_size();
			}
			else
			{
				// If class has parse-unparse functions, use them, otherwise use memcpy
				if constexpr (is_bl2_compatible_class<curtype_t>::value)
				{
					// Perform size checking if is compile time known size
					if constexpr (curtype_t::is_const_size())
					{
						if (curtype_t::static_size() > size_)
						{
							throw std::out_of_range("Not enough memory left to parse T.");
							return 0u;
						}
					}

					size = curelement_v.unparse(data_, size_);
				}
				else
				{
					if constexpr (details::is_pod<curtype_t>::value)
					{
						if (sizeof(curtype_t) > size_)
						{
							THROW_OUT_OF_RANGE_EXCEPTION("Binary Love 2 Exception: Not enough free space to unparse data.");
							return 0u;
						}

						memcpy(data_, &(curelement_v), sizeof(curtype_t));
						size = sizeof(curtype_t);
					}
				}
			}
	
			if constexpr (I + 1 < sizeof...(Args))
				size += details_unparse<I + 1, Args...>(data_ + size, size_ - size, tuple_);

			return size;
		}
	}
}

namespace bl2 = BinaryLove2;

#endif