/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabenjam <jabenjam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:24:50 by jabenjam          #+#    #+#             */
/*   Updated: 2022/08/13 15:10:26 by jabenjam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../utils/utils.hpp"
#include "../iterator/iterator.hpp"
#include "../utils/type_traits.hpp"
#include "../utils/algorithm.hpp"
#include <limits>

namespace ft
{
	template<class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T												value_type;
			typedef Alloc											allocator_type;
			typedef	std::size_t										size_type;
			typedef	std::ptrdiff_t									difference_type;
			typedef T&												reference;
			typedef typename allocator_type::const_reference		const_reference;
			typedef typename allocator_type::pointer				pointer;
			typedef typename allocator_type::const_pointer			const_pointer;
			typedef ft::random_access_iterator<T>					iterator;
			typedef	ft::random_access_iterator<const T>				const_iterator;
			typedef	ft::reverse_iterator<iterator>					reverse_iterator;
			typedef	ft::reverse_iterator<const_iterator>			const_reverse_iterator;

		private:
			allocator_type	_allocator;
			pointer			_base;
			size_type		_capacity;
			size_type		_size;

		public:
			//default (1) ---
			explicit vector(const allocator_type& alloc = allocator_type())
			: _allocator(alloc)
			, _base(NULL)
			, _capacity(0)
			, _size(0)
			{}

			//fill (2) ---
			explicit vector(size_type n, const value_type& val = value_type(),
					const allocator_type& alloc = allocator_type())
			: _allocator(alloc)
			, _base(NULL)
			, _capacity(0)
			, _size(0)
			{ this->insert(NULL, n, val); }

			//range (3) ---
			template <class InputIterator>
			vector (InputIterator first, typename ft::enable_if< !is_integral<InputIterator >::value, InputIterator>::type last,
					const allocator_type& alloc = allocator_type())
			: _allocator(alloc)
			, _base(NULL)
			, _capacity(0)
			, _size(0)
			{ assign(first, last); }

			//copy (4) ---
			vector(const vector& rhs)
			: _base(NULL)
			, _capacity(0)
			, _size(0)
			{ *this = rhs; }

			virtual ~vector()
			{
				this->clear();
				if (this->_capacity > 0 && this->_base != NULL)
					this->_allocator.deallocate(this->_base, this->_capacity);
			}

			vector& operator=(const vector& rhs)
			{
				if (&rhs != this)
				{
					this->_allocator = rhs._allocator;
					assign(rhs.begin(), rhs.end());
				}
				return (*this);
			}

			/*
			** ITERATORS
			*/

			iterator begin()
			{ return (this->_base); }

			const_iterator begin() const
			{ return (this->_base); }

			iterator end()
			{ return (this->_base + this->_size); }

			const_iterator end() const
			{ return (this->_base + this->_size); }

			reverse_iterator rbegin()
			{ return (reverse_iterator(this->end())); }

			const_reverse_iterator rbegin() const
			{ return (const_reverse_iterator(this->end())); }

			reverse_iterator rend()
			{ return (reverse_iterator(this->begin())); }

			const_reverse_iterator rend() const
			{ return (const_reverse_iterator(this->begin())); }

			/*
			** CAPACITY
			*/

			size_type size() const
			{ return (this->_size); }
			
			size_type max_size() const
			{ return (this->_allocator.max_size()); }

			void resize(size_type n, value_type val = value_type())
			{
				if (this->_size < n)
				{
					this->insert(this->end(), n - this->_size, val);
				}
				else
				{
					while (this->_size > n)
						this->erase(this->end() - 1);
				}
			}

			size_type capacity() const
			{ return (this->_capacity); }

			bool empty() const
			{ return (this->_size == 0); }

			void reserve(size_type n)
			{
				if (n > this->max_size())
					throw std::length_error("vector::reserve");
				if (this->_capacity < n)
				{
					pointer old_mem = this->_base;
					this->_base = this->_allocator.allocate(n);
					if (this->_capacity)
					{
						for (size_type i = 0; i < this->_size; i++)
						{
							this->_allocator.construct(this->_base + i, old_mem[i]);
							this->_allocator.destroy(old_mem + i);
						}
					}
					this->_allocator.deallocate(old_mem, this->_capacity);
					this->_capacity = n;
				}
			}

			/*
			** ELEMENT ACCESS
			*/

			reference operator[](size_type n)
			{ return (this->at(n)); }

			const_reference operator[](size_type n) const
			{ return (this->at(n)); }

			reference at(size_type n)
			{
				if (n >= this->_size)
					throw std::out_of_range("Out of range");
				return (this->_base[n]);
			}

			const_reference at(size_type n) const
			{
				if (n >= this->_size)
					throw std::out_of_range("Out of range");
				return (this->_base[n]);
			}

			reference front()
			{ return (this->_base[0]); }
 
			const_reference front() const
			{ return (this->_base[0]); }

			reference back()
			{ return (this->_base[this->_size - 1]); }

			const_reference back() const
			{ return (this->_base[this->_size - 1]); }

			/*
			** MODIFIERS
			*/

			// range (1) ---
			template<class InputIterator>
			void assign(InputIterator first, typename ft::enable_if<!is_integral<InputIterator>::value, InputIterator>::type last)
			{
				InputIterator	tmp = first;
				size_type		len = 0;
				while (tmp++ != last)
					len++;
				this->resize(len);
				for(int i = 0; first != last; i++, first++)
					this->_base[i] = *first;
			}
			
			// fill (2) ---
			void assign(size_type n, const value_type& val)
			{
				this->clear();
				if (n > this->_capacity)
					this->reserve(n);
				iterator it = this->begin();
				for (size_type i(0); i < n ; i++, it++)
					this->insert(it, val);
			}

			void push_back(const value_type& val)
			{ this->insert(this->end(), val); }

			void pop_back()
			{ erase(this->end() - 1); }

			// single element(1) ---
			iterator insert(iterator position, const value_type& val)
			{
				size_type idx = position - this->begin();
				this->insert(position, 1, val);
				return (this->begin() + idx);
			}

			// fill (2) ---
			void insert(iterator position, size_type n, const value_type& val)
			{
				difference_type index = position - this->begin();
				size_type j = this->_size;
				int to_construct = n, to_move = (this->_size - index), i = n;
	
				if (this->_size + n > this->_capacity)
				{
					if (this->_size + n > this->_capacity)
					{
						if ((this->_size * 2) > (this->_size + n))
							reserve(this->_size * 2);
						else
							reserve(this->_size + n);
					}
				}
				if (this->_size != 0 && static_cast<unsigned long>(index) != this->_size)
				{
					for (; to_construct; to_construct--)
						this->_allocator.construct(this->_base + j++, T());
					j = this->_size - 1;
					for (; to_move; to_move--, j--)
						this->_base[j + n] = this->_base[j] ;
					this->_size += n;
					for (; i; i--)
						this->_base[index++] = val;
				}
				else
				{
					for (; i; i--)
						this->_allocator.construct((this->_base + this->_size++), val);
				}
			}

			// range (3) ---
			template<class InputIterator>
			void insert(iterator position, InputIterator first,
					typename ft::enable_if<!is_integral<InputIterator>::value, InputIterator>::type last)
			{
				difference_type index = position - this->begin();
				difference_type index_tmp = index;
				size_type to_move = this->_size - index;
				size_type i = this->_size - 1;
				size_type size = 0, size_bkp;
				InputIterator tmp = first;

				while (tmp++ != last)
					size++;
				if (size + this->_size > this->_capacity)
				{
					if ((this->_capacity * 2) > (this->_capacity + size))
						reserve(this->_size * 2);
					else
						reserve(this->_capacity + size);
				}
				size_bkp = size;
				for (; size > 0; size--)
					this->_allocator.construct((this->_base + this->_size++), T());
				for (; to_move > 0; to_move--, i--)
					this->_base[i + size_bkp] = this->_base[i] ;
				for (; first != last; first++)
					this->_base[index_tmp++] = *first;
			}

			// single element (1) ---
			iterator erase(iterator position)
			{ return erase(position, position + 1); }
			
			// range (2) ---
			iterator erase(iterator first, iterator last)
			{
				difference_type size = last - first;
				difference_type index = first - this->begin();
				difference_type it = index;
				int i = this->_size - (size + index);

				for (; i; i--, first++, index++)
					this->_base[index] = this->_base[index + size];
				for (; size; this->_size--, size--)
					this->_allocator.destroy(this->_base + (this->_size - 1));
				return iterator(this->_base + it);
			}

			void swap(vector& rhs)
			{
				pointer tmp = this->_base;
				size_type tmpsize = this->_size;
				size_type tmpcapacity =this-> _capacity;
				allocator_type tmpalloc = this->_allocator;

				this->_base = rhs._base;
				this->_size = rhs._size;
				this->_capacity = rhs._capacity;
				this->_allocator = rhs._allocator;

				rhs._base = tmp;
				rhs._size = tmpsize;
				rhs._capacity = tmpcapacity;
				rhs._allocator = tmpalloc;
			}

			void clear()
			{ erase(this->begin(), this->end()); }

			/*
			** ALLOCATOR
			*/

			allocator_type get_allocator() const
			{ return (this->_allocator); }

	};

	/*
	** Non Member Functions
	*/

	// (1) ---
	template <class T, class Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	};

	// (2) ---
	template <class T, class Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{ return (!(lhs == rhs)); };

	// (3) ---
	template <class T, class Alloc>
	bool operator<(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return (lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); };

	// (4) ---
	template <class T, class Alloc>
	bool operator<=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return (!(rhs < lhs)); };

	// (5) ---
	template <class T, class Alloc>
	bool operator>(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return (rhs < lhs); };

	// (6) ---
	template <class T, class Alloc>
	bool operator>=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return (!(lhs < rhs)); };

	template <class T, class Alloc>
	void swap(vector<T,Alloc>& x, vector<T,Alloc>& y)
	{ x.swap(y); }
};