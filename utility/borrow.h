#pragma once

#include "common.h"

// Borrow is basically just a pointer container that acts as a broadcast of "you do not own this, but you can use it" and "this is managed somewhere else".
// More expressive than just having normal pointer containers or const/non const raw pointers and avoids having to store const refs.

template <typename T>
class borrow_mut {
public:
	borrow_mut() {}
	borrow_mut(T* rhs) : m_value(rhs) {}
	borrow_mut(const borrow_mut& rhs) : m_value(rhs.m_value) {}
	b32 empty() const {
		return (m_value != nullptr);
	}
	b32 match(const T& text) {
		return (&text == m_value);
	}
	T& val() {
		if (m_value) {
			return *m_value;
		}
		throw;
	}
	template <typename D>
	D& as() {
		return *((D*)(m_value));
	}
	borrow_mut& operator= (const borrow_mut& rhs) {
		if (this == &rhs) { return *this; }
		m_value = rhs.m_value;
		return *this;
	}
private:
	T* m_value = nullptr;
};

// NOTE: A borrow can copy from a borrow_mut, but not vice versa as that freely circumvent immutability
template <typename T>
class borrow {
public:
	borrow() {}
	borrow(T* rhs) : m_value(rhs) {} // HERESY DETECTED
	borrow(const borrow& rhs) : m_value(rhs.m_value) {}
	borrow(const borrow_mut<T>& rhs) : m_value(rhs.m_value) {}
	b32 empty() const {
		return (m_value != nullptr);
	}
	b32 match(const T& text) {
		return (&text == m_value);
	}
	const T& val() {
		if (m_value) {
			return *m_value;
		}
		throw;
	}
	template <typename D>
	const D& as() {
		return *((D*)(m_value));
	}
	borrow& operator= (const borrow& rhs) {
		if (this == &rhs) { return *this; }
		m_value = rhs.m_value;
		return *this;
	}
	borrow& operator= (const borrow_mut<T>& rhs) {
		if (this == &rhs) { return *this; }
		m_value = rhs.m_value;
		return *this;
	}
private:
	T* m_value = nullptr;
};

// Hacky but avoids friend declarations.. which is also hacky
template <typename T>
b32 operator==(const borrow<T>& lhs, borrow<T>& rhs) {
	return &lhs.val() == &lhs.val();
}

template <typename T>
b32 operator==(const borrow_mut<T>& lhs, borrow_mut<T>& rhs) {
	return &lhs.val() == &lhs.val();
}

template <typename T>
b32 operator==(const borrow<T>& lhs, borrow_mut<T>& rhs) {
	return &lhs.val() == &lhs.val();
}

template <typename T>
b32 operator==(const borrow_mut<T>& lhs, borrow<T>& rhs) {
	return &lhs.val() == &lhs.val();
}