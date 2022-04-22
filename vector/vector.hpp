/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabenjam <jabenjam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:24:50 by jabenjam          #+#    #+#             */
/*   Updated: 2022/04/22 17:13:50 by jabenjam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../iterator/random_access_iterator.hpp"
#include <memory>

namespace ft
{
	template<class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T											value_type;
			typedef Alloc										allocator_type;
			typedef T&											reference;
			typedef const T&									const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename random_access_iterator<T>			iterator;
			typedef	typename random_access_iterator<const T>	const_iterator;
			//typedef	reverse_iterator<iterator>					reverse_iterator;
			//typedef	reverse_iterator<const iterator>			const_reverse_iterator;
			typedef	ptrdiff_t									difference_type;
			typedef	size_t										size_type;

		private:
			allocator_type	_allocator;
			pointer			_base;
			size_type		_capacity;
			size_type		_size;

		public:
			//default (1) ---
			vector(const allocator_type& alloc = allocator_type());

			//fill (2) ---
			vector(size_type n, const value_type& val = value_type(),
					const allocator_type& alloc = allocator_type());
			template <class InputIterator>

			//range (3) ---
			vector(InputIterator first, InputIterator last,
					const allocator_type& alloc = allocator_type());

			//copy (4) ---
			vector(const vector& rhs);

			~vector();

			vector &operator=(const vector& rhs);
			{
				this->_allocator = rhs._allocator;
				this->_base = rhs._base;
				this->_capacity = rhs._capacity;
				this->_size = rhs._size;
				return (*this);
			}

			/*
			** ITERATORS
			*/

			iterator begin(); { return(iterator(this->_base)); }

			const_iterator begin() const; { return(const_iterator(this->_base)); }

			iterator end(); { return(iterator(this->_base) + size); }

			const_iterator end() const; { return(const_iterator(this->_base) + size); }

			//reverse_iterator rbegin();

			//const reverse_iterator rbegin() const;

			//reverse_iterator rend();

			//const reverse_iterator rend() const;

			/*
			** CAPACITY
			*/

			size_type size() const; { return(this->_size); }
			
			size_type max_size() const; { return(allocator_type.max_size()); }

			//void resize(size_type n, value_type val = value_type());

			size_type capacity() const; { return(this->_capacity); }

			bool empty() const; { return(this->_size == 0); }

			// void reserve(size_type n);

			/*
			** ELEMENT ACCESS
			*/

			// reference operator[](size_type n);
			// const_reference operator[](size_type n) const;

			// reference at(size_type n);
			// const_reference at(size_type n) const;

			// reference front();
			// const_reference front() const;

			// reference back();
			// const_reference back() const;

			/*
			** MODIFIERS
			*/

			// range (1) ---
			// template<class InputIterator>
			// void assign(InputIterator first, InputIterator last);
			
			// fill (2) ---
			// void assign(size_type n, const value_type& val);

			// void push_back(const value_type& val);

			// void pop_back();

			// single element(1) ---
			// iterator insert(iterator position, const value_type& val);

			// fill (2) ---
			// void insert(iterator position, size_type n, const value_type& val);

			// range (3) ---
			// template<class InputIterator>
			// void insert(iterator position, InputIterator first, InputIterator last);

			// iterator erase(iterator position);
			// iterator erase(iterator first, iterator last);

			// void swap(vector& x);

			// void clear();

			/*
			** ALLOCATOR
			*/

			// allocator_type get_allocator() const;

			/*
			** NON MEMBER FUNCTION OVERLOADS
			*/

			// (1) ---
			// template <class T, class Alloc>
			//   bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

			// (2) ---
			// template <class T, class Alloc>
			//   bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

			// (3) ---
			// template <class T, class Alloc>
			//   bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

			// (4) ---
			// template <class T, class Alloc>
			//   bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

			// (5) ---
			// template <class T, class Alloc>
			//   bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

			// (6) ---
			// template <class T, class Alloc>
			//   bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

			// template <class T, class Alloc>
			// void swap (vector<T,Alloc>& x, vector<T,Alloc>& y);

			// TEMPLATE SPECIALIZATION FOR BOOL IS NOT NEEDED
	};
};