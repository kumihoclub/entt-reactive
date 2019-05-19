#pragma once

#include "common.h"

template <typename T>
class option {
public:
	option() {}
	option(const T& rhs) : m_value(rhs), m_valid(true) {}
	option(T&& rhs) : value(std::move(rhs)), m_valid(true) {}
	option(const option& rhs) : m_valid(rhs.m_valid) {
		if (m_valid) { new(&m_value) T(rhs.m_value); }
	}
	option(option&& rhs) : m_valid(rhs.m_valid) {
		if (m_valid) { new(&m_value) T(std::move(rhs.m_value)); }
	}
	b32 valid() const {
		return m_valid;
	}
	T& val() {
		if (m_valid) {
			return m_value;
		}
		throw;
	};
	void invalidate() {
		m_valid = false;
	}
	option& operator= (const option& rhs) {
		if (this == &rhs) { return *this; }
		m_valid = rhs.m_valid;
		if (m_valid) { m_value = rhs.m_value; }
		return *this;
	}
	option& operator= (const T& rhs) {
		m_valid = true;
		m_value = rhs;
		return *this;
	}
private:
	union { T m_value; };
	b32 m_valid = false;
};