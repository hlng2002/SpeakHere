#include "circular_buffer.h"
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

namespace B_MODULE {

	Circular_Buffer::Circular_Buffer(int capacity /*= 5 * 1024*/):
		m_read_pos(0),
		m_write_pos(0),
		m_size(0),
		m_capacity(0),
		m_data(0)
	{
		if (capacity <= 1) {
			capacity = 512;
		}
		inc_capacity(capacity);
	}

	Circular_Buffer::Circular_Buffer(const Circular_Buffer& cb):
		m_read_pos(0),
		m_write_pos(0),
		m_size(0),
		m_capacity(0),
		m_data(0)
	{
		this->assign(cb);
	}

	const Circular_Buffer& Circular_Buffer::operator=(const Circular_Buffer& cb)
	{
		this->assign(cb);
		return *this;
	}

	Circular_Buffer::~Circular_Buffer()
	{
		m_read_pos = 0;
		m_write_pos = 0;
		m_size = 0;
		m_capacity = 0;

		if (m_data) {
			free(m_data);
		}
		m_data = 0;
	}

	int Circular_Buffer::inc_capacity(int ncapacity, bool copy_data /*= true*/)
	{
		if (ncapacity <= m_capacity) {
			return m_capacity;
		}
		char* ndata = (char*)malloc(ncapacity);
		if (ndata == 0) {
			exit(-1);
		}
		if (m_size > 0 && copy_data) {
			int ret = read_peek(ndata, m_size);
			if (ret < m_size) {
				exit(-1);
			}
			m_write_pos = m_size;
		}
		else {
			m_write_pos = 0;
			m_size = 0;
		}
		if (m_data) {
			free(m_data);
		}
		m_data = ndata;
		m_read_pos = 0;
		m_capacity = ncapacity;

		return ncapacity;
	}

	int Circular_Buffer::size() const
	{
		return m_size;
	}

	int Circular_Buffer::capacity() const
	{
		return m_capacity;
	}

	int Circular_Buffer::read(char* data, int bytes)
	{
		if (bytes == 0) return 0;

		int capacity = m_capacity;
		int bytes_to_read = std::min(bytes, m_size);

		if (bytes_to_read <= capacity - m_read_pos)
		{	// Read in a single step
			memcpy(data, m_data + m_read_pos, bytes_to_read);
			m_read_pos += bytes_to_read;
			if (m_read_pos == capacity) {
				m_read_pos = 0;
			}
		}
		else
		{	// Read in two steps
			int size_1 = capacity - m_read_pos;
			memcpy(data, m_data + m_read_pos, size_1);
			int size_2 = bytes_to_read - size_1;
			memcpy(data + size_1, m_data, size_2);
			m_read_pos = size_2;
		}
		m_size -= bytes_to_read;
		return bytes_to_read;
	}

	int Circular_Buffer::read_peek(char* data, int bytes) const
	{
		if (bytes == 0) return 0;

		int capacity = m_capacity;
		int bytes_to_read = std::min(bytes, m_size);

		if (bytes_to_read <= capacity - m_read_pos) {
			// Read in a single step
			memcpy(data, m_data + m_read_pos, bytes_to_read);
		}
		else
		{	// Read in two steps
			int size_1 = capacity - m_read_pos;
			memcpy(data, m_data + m_read_pos, size_1);
			int size_2 = bytes_to_read - size_1;
			memcpy(data + size_1, m_data, size_2);
		}
		return bytes_to_read;
	}

	int Circular_Buffer::write(const char* data, int bytes, bool overlap /*= false*/)
	{
		if (bytes == 0) return 0;

		int empty_size = m_capacity - m_size;
		int bytes_to_write = bytes;

		if (true == overlap) {
			if (bytes_to_write > empty_size) {
				skip(bytes_to_write - empty_size);
			}
			if (bytes_to_write > m_capacity) {
				data += bytes_to_write - m_capacity;
				bytes_to_write = m_capacity;
			}
		}
		else {

			bytes_to_write = std::min(bytes, m_capacity - m_size);
		}

		if (bytes_to_write <= m_capacity - m_write_pos)
		{	// Write in a single step
			memcpy(m_data + m_write_pos, data, bytes_to_write);
			m_write_pos += bytes_to_write;
			if (m_write_pos == m_capacity) {
				m_write_pos = 0;
			}
		}
		else
		{	// Write in two steps
			int size_1 = m_capacity - m_write_pos;
			memcpy(m_data + m_write_pos, data, size_1);
			int size_2 = bytes_to_write - size_1;
			memcpy(m_data, data + size_1, size_2);
			m_write_pos = size_2;
		}

		m_size += bytes_to_write;
		return bytes_to_write;
	}

	int Circular_Buffer::skip(int bytes)
	{
		if (bytes == 0) return 0;

		int capacity = m_capacity;
		int bytes_to_read = std::min(bytes, m_size);

		if (bytes_to_read <= capacity - m_read_pos)
		{	// Read in a single step
			m_read_pos += bytes_to_read;
			if (m_read_pos == capacity) {
				m_read_pos = 0;
			}
		}
		else
		{	// Read in two steps
			int size_1 = capacity - m_read_pos;
			int size_2 = bytes_to_read - size_1;
			m_read_pos = size_2;
		}

		m_size -= bytes_to_read;
		return bytes_to_read;
	}

	bool Circular_Buffer::full()
	{
		return m_size == m_capacity;
	}

	bool Circular_Buffer::empty()
	{
		return m_size == 0;
	}

	void Circular_Buffer::clear()
	{
		m_size = 0;
		m_read_pos = 0;
		m_write_pos = 0;
	}

	void Circular_Buffer::assign(const Circular_Buffer& cb)
	{
		if (m_capacity != cb.m_capacity) {
			m_capacity = cb.m_capacity;
			char* ndata = (char*)realloc(m_data, m_capacity);
			if (ndata == 0) {
				exit(-1);
			}
			m_data = ndata;
		}
		m_read_pos = 0;
		m_write_pos = cb.m_size;
		m_size = cb.m_size;

		if (m_write_pos == m_capacity) m_write_pos = 0;

		if (cb.m_size > 0) {
			int ret = cb.read_peek(m_data, cb.m_size);
			if (ret != cb.m_size) {
				exit(-1);
			}
		}
	}

	void Circular_Buffer::swap(Circular_Buffer& cb)
	{
		char* tdata = m_data;
		m_data = cb.m_data;
		cb.m_data = tdata;

		int  temp = m_read_pos;
		m_read_pos = cb.m_read_pos;
		cb.m_read_pos = temp;

		temp = m_write_pos;
		m_write_pos = cb.m_write_pos;
		cb.m_write_pos = temp;

		temp = m_size;
		m_size = cb.m_size;
		cb.m_size = temp;

		temp = m_capacity;
		m_capacity = cb.m_capacity;
		cb.m_capacity = temp;
	}

}