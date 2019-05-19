#pragma once

#include "option.h"

#include "plf/plf_colony.h"

// To be used exclusively with plf_colony iterators, basically a wrapper for an option of the iterator type.

template <typename T>
class handle {
public:

	handle() {}
	handle(typename plf::colony<T>::iterator it) : m_it(it) {}

	T* operator*() {
		if (m_it.valid()) {
			return &(*(m_it.val()));
		}
		return nullptr;
	}

	T* operator->() {
		if (m_it.valid()) {
			return &(*(m_it.val()));
		}
		return nullptr;
	};

	void operator=(const typename plf::colony<T>::iterator& iterator) {
		m_it = iterator;
	}

	typename plf::colony<T>::iterator* it() {
		if (m_it.valid()) {
			return &m_it.val();
		}
		return nullptr;
	}

private:
	option<typename plf::colony<T>::iterator> m_it;

};