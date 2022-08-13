/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabenjam <jabenjam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:04:45 by jabenjam          #+#    #+#             */
/*   Updated: 2022/08/02 21:11:33 by jabenjam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../utils/utils.hpp"
#include "../utils/node.hpp"
#include "../utils/pair.hpp"
#include "../utils/algorithm.hpp"
#include "../utils/type_traits.hpp"
#include "../iterator/iterator.hpp"
#include <limits>

namespace ft
{
	template< class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T> > >
	class map
	{
		public:
			typedef	Key														key_type;
			typedef	T														mapped_type;
			typedef	pair<const Key, T>										value_type;
			typedef	std::size_t												size_type;
			typedef	std::ptrdiff_t											difference_type;
			typedef	Compare													key_compare;
			typedef	Allocator												allocator_type;
			typedef	value_type&												reference;
			typedef	const value_type&										const_reference;
			typedef	typename allocator_type::pointer						pointer;
			typedef	typename allocator_type::const_pointer					const_pointer;
			typedef	typename ft::bidirectional_iterator<value_type>			iterator;
			typedef	typename ft::bidirectional_iterator<const value_type>	const_iterator;
			typedef	typename ft::reverse_iterator<iterator>					reverse_iterator;
			typedef	typename ft::reverse_iterator<const_iterator>			const_reverse_iterator;

			class value_compare : public std::binary_function<value_type, value_type, bool>
			{
				friend class map; // peut etre retirer
				public :
					typedef bool		result_type;
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;

					bool operator()(const value_type& lhs, const value_type& rhs) const
					{ return this->_comp(lhs.first, rhs.first); }

				protected :
					key_compare _comp;

					value_compare(key_compare c)
					: _comp(c)
					{}
			};

		private:
			allocator_type		_allocator;
			node<value_type>	*_root;
			size_type			_size;
			key_compare			_comp;

		public:
			//default (1) ---
			map()
			: _allocator(allocator_type()), _size(0), _comp(key_compare())
			{ this->_root = new node<value_type>(); }

			//comp (2) ---
			explicit map(const key_compare& comp,
				const allocator_type& allocator = allocator_type())
			: _allocator(allocator), _size(0), _comp(comp)
			{ this->_root = new node<value_type>(); }

			//iterator (3) ---
			template <class InputIt>
			map(InputIt first,
				typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type last,
				const key_compare& comp = key_compare(),
				const allocator_type& allocator = allocator_type()) 
			: _allocator(allocator), _size(0), _comp(comp)
			{ this->insert(first, last); }

			//copy (4) ---
			map(const map& rhs)
			{ *this = rhs; }

			virtual ~map()
			{
				if (this->_size)
					this->clear();
				delete this->_root;
			}

			map& operator=(const map& rhs)
			{
				if (this != &rhs)
				{
					if (this->_size)
						this->clear();
					this->insert(rhs.begin(), rhs.end());
				}
				return (*this);
			}

			allocator_type get_allocator() const
			{ return (this->_allocator); }

			/*
			** ELEMENT ACCESS
			*/

			T& at(const key_type& key)
			{
				node<value_type> *n = this->find(key);
				if (n)
					return (n->get_value()->second);
				else
					throw std::out_of_range("Out of Range");
			}

			const T& at(const key_type& key) const
			{
				node<value_type> *n = this->find(key);
				if (n)
					return (n->get_value()->second);
				else
					throw std::out_of_range("Out of Range");
			}

			T& operator[](const Key& key)
			{
				node<value_type> *n = this->find(key);
				if (n)
					return (n->get_value()->second);
				else
				{
					insert(make_pair<key_type, T>(key, T()));
					return (this->at(key));
				}
			}

			/*
			** ITERATORS
			*/

			iterator begin()
			{
				node<value_type> *first = this->_root;
				while(first->get_left() && first->get_left()->get_value())
					first = first->get_left();
				return (iterator(first));
			}

			const_iterator begin() const
			{
				node<value_type> *first = this->_root;
				while(first->get_left() && first->get_left()->get_value())
					first = first->get_left();
				return (const_iterator(first));
			}

			iterator end()
			{
				node<value_type> *last = this->_root;
				while(last->get_right())
					last = last->get_right();
				return (iterator(last));
			}

			const_iterator end() const
			{
				node<value_type> *last = this->_root;
				while(last->get_right())
					last = last->get_right();
				return (const_iterator(last));
			}

			reverse_iterator rbegin()
			{
				node<value_type> *last = this->_root;
				while(last->get_right())
					last = last->get_right();
				return (reverse_iterator(last));
			}

			const_reverse_iterator rbegin() const
			{
				node<value_type> *last = this->_root;
				while(last->get_right())
					last = last->get_right();
				return (const_reverse_iterator(last));
			}

			reverse_iterator rend()
			{
				node<value_type> *first = this->_root;
				while(first->get_right())
					first = first->get_right();
				return (reverse_iterator(first));
			}

			const_reverse_iterator rend() const
			{
				node<value_type> *first = this->_root;
				while(first->get_right())
					first = first->get_right();
				return (const_reverse_iterator(first));
			}

			/*
			** CAPACITY
			*/

			bool empty() const
			{ return (!_size); }

			size_type size() const
			{ return (this->_size); }

			size_type max_size() const
			{ return (this->_allocator.max_size()); }

			/*
			** MODIFIERS
			*/

			void clear()
			{ this->erase(this->begin(), this->end()); }

			pair<iterator, bool> insert(const value_type& value)
			{
				node<value_type> *to_return = _find_node_by_key(value.first);

				if (to_return)
					return (ft::make_pair(iterator(to_return), 0));
				pointer new_value = this->_allocator.allocate(1);
				this->_allocator.construct(new_value, value_type(value));
				node<value_type> *	new_node = new node<value_type>(new_value);
				this->_insert_node(new_node);
				this->_insert_rebalancing(new_node);
				this->_size++;
				return (ft::make_pair(iterator(to_return), 1));
			}

			iterator insert(iterator hint, const value_type& value)
			{
				(void)hint;
				pair<iterator, bool> new_pair = this->insert(value);
				return (new_pair.first);
			}

			template<class InputIt>
			void insert(InputIt first,
				typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type last)
			{
				while (first != last)
					this->insert(*(first++));
			}

			void erase(iterator pos)
			{
				if (this->_size)
					this->_erase_node(pos, this->_find_node_by_key(pos->first));
				else
					this->_erase_root();
				this->_size--;
			}

			void erase(iterator first, iterator last)
			{
				iterator it = first;
				iterator next = first;

				while (it != last)
				{
					it = first;
					next = ++first;
					erase(it);
				}
			}

			size_type erase(const Key& key)
			{
				node<value_type> *to_erase = _find_by_key(key);

				if (!to_erase)
					return (0);
				if (this->_size)
					_erase_node(iterator(to_erase), to_erase);
				else
				{
					this->_allocator.destroy(this->_root->get_value());
					this->_allocator.deallocate(this->_root->get_value(), 1);
					delete this->_root;
					this->_root = new node<value_type>();
				}
				_size--;
				return (1);
			}

			void swap(map& rhs)
			{
				node<value_type>	*lhs_root = this->_root;
				size_type			lhs_size = this->_size;

				this->_root = rhs._root;
				this->_size = rhs._size;

				rhs._root = lhs_root;
				rhs._size = lhs_size;
			}

			/*
			** LOOKUP
			*/

			size_type count(const Key& key) const
			{ return (_find_node_by_key(key) ? true : false); }

			iterator find(const Key& key)
			{
				node<value_type> *found = _find_node_by_key(key);
				return (found ? iterator(found) : this->end());
			}

			const_iterator find(const Key& key) const
			{
				node<value_type> *found = _find_node_by_key(key);
				return (found ? const_iterator(found) : this->end());
			}

			pair<iterator,iterator> equal_range(const Key& key)
			{ return(ft::make_pair<iterator, iterator>(lower_bound(key), upper_bound(key))); }

			pair<const_iterator,const_iterator> equal_range(const Key& key) const
			{ return(ft::make_pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key))); }

			iterator lower_bound(const Key& key)
			{
				iterator it = this->begin();
				iterator ite = this->end();

				while (it != ite)
				{
					if (!this->key_comp()(it->first, key))
						return (it);
					it++;
				}
				return (this->end());
			}

			const_iterator lower_bound(const Key& key) const
			{
				const_iterator it = this->begin();
				const_iterator ite = this->end();

				while (it != ite)
				{
					if (!this->key_comp()(it->first, key))
						return (it);
					it++;
				}
				return (this->end());
			}

			iterator upper_bound(const Key& key)
			{
				iterator it = this->begin();
				iterator ite = this->end();

				while (it != ite)
				{
					if (this->key_comp()(key, it->first))
						return (it);
					it++;
				}
				return (this->end());
			}

			const_iterator upper_bound(const Key& key) const
			{
				const_iterator it = this->begin();
				const_iterator ite = this->end();

				while (it != ite)
				{
					if (this->key_comp()(key, it->first))
						return (it);
					it++;
				}
				return (this->end());
			}

			/*
			** OBSERVERS
			*/

			key_compare key_comp() const
			{ return (this->_comp); }

			value_compare value_comp() const
			{ return (value_compare(this->_comp)); }


			/*
			** BEHIND THE SCENES
			*/

			private:
				node<value_type> *_find_node_by_key(key_type key) const
				{
					node<value_type> *found = this->_root;
					bool			 direction;

					while (found && !found->empty())
					{
						if (found->get_value()->first == key)
							return (found);
						direction = key_compare()(found->get_value()->first, key);
						found = found->get_child(direction);
					}
					return (NULL);
				}

				void _insert_rebalancing(node<value_type> *n)
				{
					node<value_type> *uncle;
					bool			direction = E_LEFT;

					if (this->_root == n)
					{
						n->set_color(E_BLACK);
						return ;
					}
					while (n != this->_root && n->get_parent()->get_color() == E_RED)
					{
						uncle = n->get_uncle();
						direction = n->get_parent()->get_direction();
						if (uncle && uncle->get_color() == E_RED)
						{
							n->get_parent()->set_color(E_BLACK);
							uncle->set_color(E_BLACK);
							uncle->get_parent()->set_color(E_RED);
							n = n->get_grandparent();
						}
						else if (n->get_direction() == !direction)
						{
							n = n->get_parent();
							this->_rotation(n, direction);
						}
						else
						{
							n->get_parent()->set_color(E_BLACK);
							n->get_grandparent()->set_color(E_RED);
							n = n->get_grandparent();
							this->_rotation(n, !direction);
						}
						this->_root->set_color(E_BLACK);
					}
				}

				void _erase_root()
				{
					this->_allocator.destroy(this->_root->get_value());
					this->_allocator.deallocate(this->_root->get_value(), 1);
					delete this->_root;
					this->_root = new node<value_type>();
				}

				void _switch_nodes(node<value_type> *to_replace, node<value_type> *replacement)
				{
					node<value_type>	*parent = to_replace->get_parent();

					if (parent->empty())
						this->_root = replacement;
					else
						parent->set_child(replacement, to_replace->get_direction());
					replacement->set_parent(parent);
				}

				void _steal_child(node<value_type> *new_parent, node<value_type> *old_parent, bool direction)
				{
					new_parent->set_child(old_parent->get_child(direction), direction);
					new_parent->get_child(direction)->set_parent(old_parent);
					old_parent->set_child(NULL, direction);
				}

				void _destroy_node(node<value_type> *to_destroy)
				{
					this->_allocator.destroy(to_destroy->get_value());
					this->_allocator.deallocate(to_destroy->get_value(), 1);
					delete to_destroy;
				}

				void _removal_rebalancing(node<value_type> *n)
				{
					for (node<value_type> *n2; n != this->_root && n->get_color(); n2 = n->get_twin())
					{
						//peut etre ajouter un n2 = n->get_twin()
						if (n2->get_color() == E_RED)
						{
							n2->set_color(E_BLACK);
							n->get_parent()->set_color(E_RED);
							this->_rotation(n->get_parent(), n->get_direction());
						}
						else if (n2->get_value() 
								&& n2->get_child(n->get_direction())->get_color() == E_BLACK
								&& n2->get_child(n2->get_direction())->get_color() == E_BLACK)
						{
							n2->set_color(E_RED);
							n = n->get_parent();
						}
						else
						{
							if (n2->get_value() && n2->get_child(n2->get_direction())->get_color() == E_BLACK)
							{
								n2->get_child(n->get_direction())->set_color(E_BLACK);
								n2->set_color(E_RED);
								this->_rotation(n2, n2->get_direction());
								n2 = n->get_twin();
							}
							n2->set_color(n->get_parent()->get_color());
							n->get_parent()->set_color(E_BLACK);
							if (n2->get_value())
								n2->get_child(n2->get_direction())->set_color(E_BLACK);
							this->_rotation(n->get_parent(), n->get_direction());
							n = this->_root;
						}
						n2 = n->get_twin();
					}
					n->set_color(E_BLACK);
				}

				void _erase_node(iterator it, node<value_type> *to_delete)
				{
					node<value_type>	*n;
					bool				direction;
					bool				color;

					if (to_delete->get_left()->empty() || to_delete->get_right()->empty())
					{
						color = to_delete->get_color();
						direction = (to_delete->get_left()->empty() ? E_RIGHT : E_LEFT);
						n = to_delete->get_child(direction);
						this->_switch_nodes(to_delete, n);
						to_delete->set_child(0, direction);
					}
					else
					{
						node<value_type> *replacement = _find_node_by_key((++it)->first);
						color = replacement->get_color();
						n = replacement->get_right();
						if (to_delete == replacement->get_parent())
							n->set_parent(replacement);
						else
						{
							this->_switch_nodes(replacement, n);
							this->_steal_child(replacement, to_delete, E_RIGHT);
						}
						this->_switch_nodes(to_delete, replacement);
						if (replacement->get_left() && replacement->get_left()->empty())
							delete replacement->get_left();
						this->_steal_child(replacement, to_delete, E_LEFT);
						replacement->set_color(to_delete->get_color());
					}
					this->_destroy_node(to_delete);
					if (color != E_RED)
						_removal_rebalancing(n);
				}

				void _insert_node(node<value_type> *new_node)
				{
					node<value_type>	*parent = this->_root;
					bool				direction;

					if (this->_root->empty())
					{
						delete this->_root;
						this->_root = new_node;
						return ;
					}
					while (parent->get_value())
					{
						direction = value_compare(this->_comp)(*(parent->get_value()), *(new_node->get_value()));
						if (parent->get_child(direction) && parent->get_child(direction)->get_value())
							parent = parent->get_child(direction);
						else
						{
							delete parent->get_child(direction);
							parent->set_child(new_node, direction);
							new_node->set_parent(parent);
							return ;
						}
					}
				}

				void _rotation(node<value_type> *target, bool direction)
				{
					if (!target)
						return ;
					node<value_type> *n = target->get_child(!direction);
					target->set_child(n->get_child(direction), !direction);
					n->get_child(direction)->set_parent(target);
					n->set_child(target, direction);
					n->set_parent(target->get_parent());
					if (!n->get_parent())
						this->_root = n;
					else
						target->get_parent()->set_child(n, target->get_direction());
					n->set_child(target, direction);
					target->set_parent(n);
				}
	};

	/*
	** NON MEMBER FUNCTIONS
	*/

	template <class Key, class T, class Compare, class Alloc>
	bool operator==(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs)
	{ return (!(lhs == rhs)); }

	template <class Key, class T, class Compare, class Alloc>
	bool operator<(const map<Key, T, Compare, Alloc> &lhs,
				   const map<Key, T, Compare, Alloc> &rhs)
	{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); }

	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs)
	{ return (!(lhs > rhs)); }

	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const map<Key, T, Compare, Alloc> &lhs,
				   const map<Key, T, Compare, Alloc> &rhs)
	{ return (lhs != rhs || rhs < lhs); }

	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs)
	{ return (!(lhs < rhs)); }

	template <class Key, class T, class Compare, class Alloc>
	void swap(map<Key, T, Compare, Alloc> &lhs,
			  map<Key, T, Compare, Alloc> &rhs)
	{ lhs.swap(rhs); }
}