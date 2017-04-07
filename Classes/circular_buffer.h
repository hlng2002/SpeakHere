#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

namespace B_MODULE {
	class Circular_Buffer
	{
	public:
		//! Create a Circular_Buffer of size \c n
		Circular_Buffer(int capacity = 5 * 1024);
		Circular_Buffer(const Circular_Buffer& cb);
		const Circular_Buffer& operator = (const Circular_Buffer&);

		~Circular_Buffer();
	public:

		/*
		increase buffer capacity:
		param : copy_data  True copy old data ,False clear buffer;
		*/
		int inc_capacity(int, bool copy_data = true);

		/*
		get buffer data length
		*/
		int size() const;

		/*
		get buffer capacity;
		*/
		int capacity() const;

		/*
		read data from buffer to buffer,and remove data from buffer
		param dest : target memory address
		param dlen : max read length

		return     : real read data from buffer
		*/
		int read(char* dest, int dlen);

		/*
		read data from buffer to buffer, but not remove data from buffer
		param dest : target memory address
		param dlen : max read length

		return     : real read data from buffer
		*/
		int read_peek(char*, int) const;

		/*
		write data into buffer
		param dest   : source memory address
		param dlen   : max write length
		param overlap: weather overlap old data
		return     : real write data from buffer
		*/
		int write(const char*, int, bool overlap = false);

		/*
		remove some data from buffer
		param dlen : max remove data length
		return     : real remove data from buffer
		*/
		int skip(int);

		bool full();

		bool empty();

		void clear();

		void assign(const Circular_Buffer&);

		void swap(Circular_Buffer&);


	private:

		int  m_read_pos;
		int  m_write_pos;
		int  m_size;
		int  m_capacity;
		char* m_data;
	};
}












#endif