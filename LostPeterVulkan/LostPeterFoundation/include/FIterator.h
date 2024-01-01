/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_ITERATOR_H_
#define _F_ITERATOR_H_

namespace LostPeterFoundation
{
    ////////////////////////// FIteratorVector //////////////////////////
    template <class T>
	class FIteratorVector
	{
	private:
		FIteratorVector()
		{

		}

	public:
		FIteratorVector(typename T::iterator itStart, typename T::iterator itEnd)
			: m_itCurrent(itStart)
			, m_itEnd(itEnd)
		{

		}

		explicit FIteratorVector(T& c)
			: m_itCurrent(c.begin())
			, m_itEnd(c.end())
		{

		}

	public:
		typedef typename T::value_type ValueType;

	private:
		typename T::iterator m_itCurrent;
		typename T::iterator m_itEnd;

	public:
		bool HasMoreElements() const
		{
			return m_itCurrent != m_itEnd;
		}

		typename T::value_type GetNext()
		{
			return *m_itCurrent++;
		}

		typename T::value_type PeekNext()
		{
			return *m_itCurrent;
		}   

		typename T::pointer	PeekNextPtr()
		{
			return &(*m_itCurrent);
		}

		void MoveNext()
		{
			++m_itCurrent;
		}
	};


    ////////////////////////// FIteratorVectorConst /////////////////////
    template <class T>
	class FIteratorVectorConst
	{
	private:
		FIteratorVectorConst()
		{

		}

	public:
		FIteratorVectorConst(typename T::const_iterator itStart, typename T::const_iterator itEnd)
			: m_itCurrent(itStart)
			, m_itEnd(itEnd)
		{

		}

		explicit FIteratorVectorConst(const T& c)
			: m_itCurrent(c.begin())
			, m_itEnd(c.end())
		{

		}

	public:
		typedef typename T::value_type ValueType;

    private:
		mutable typename T::const_iterator m_itCurrent;
		typename T::const_iterator m_itEnd;

	public:
		bool HasMoreElements() const
		{
			return m_itCurrent != m_itEnd;
		}

		typename T::value_type GetNext()
		{
			return *m_itCurrent++;
		}

		typename T::value_type PeekNext() const
		{
			return *m_itCurrent;
		}

		typename T::const_pointer PeekNextPtr() const
		{
			return &(*m_itCurrent);
		}

		void MoveNext() const
		{
			++m_itCurrent;
		}
	};


    ////////////////////////// FIteratorMap /////////////////////////////
    template <class T>
	class FIteratorMap
	{
	private:
		FIteratorMap() 
		{

		}

	public:
		FIteratorMap(typename T::iterator itStart, typename T::iterator itEnd)
			: m_itCurrent(itStart)
			, m_itEnd(itEnd)
		{

		}

		explicit FIteratorMap(T& c)
			: m_itCurrent(c.begin())
			, m_itEnd(c.end())
		{

		}

    public:
		typedef typename T::mapped_type	MappedType;
		typedef typename T::key_type KeyType;

	private:
		typename T::iterator m_itCurrent;
		typename T::iterator m_itEnd;

	public:
		bool HasMoreElements() const
		{
			return m_itCurrent != m_itEnd;
		}

		typename T::mapped_type GetNext()
		{
			return (m_itCurrent++)->second;
		}

		typename T::mapped_type PeekNextValue()
		{
			return m_itCurrent->second;
		}

		typename T::key_type PeekNextKey()
		{
			return m_itCurrent->first;
		}

		FIteratorMap<T> &operator=(FIteratorMap<T> &rhs)
		{
			m_itCurrent = rhs.m_itCurrent;
			m_itEnd	  = rhs.m_itEnd;

			return *this;
		}

		typename T::mapped_type* PeekNextValuePtr()
		{
			return &(m_itCurrent->second);
		}

		void MoveNext()
		{
			++m_itCurrent;
		}
	};


    ////////////////////////// FIteratorMapConst ////////////////////////
    template <class T>
	class FIteratorMapConst
	{
	private:
		FIteratorMapConst() 
		{

		}

	public:
		FIteratorMapConst(typename T::const_iterator itStart, typename T::const_iterator itEnd)
			: m_itCurrent(itStart)
			, m_itEnd(itEnd)
		{

		}

		explicit FIteratorMapConst(const T& c)
			: m_itCurrent(c.begin())
			, m_itEnd(c.end())
		{

		}   

    public:
		typedef typename T::mapped_type	MappedType;
		typedef typename T::key_type KeyType;

	private:
		mutable typename T::const_iterator m_itCurrent;
		typename T::const_iterator m_itEnd;

	public:	
		bool HasMoreElements() const
		{
			return m_itCurrent != m_itEnd;
		}

		typename T::mapped_type	GetNext()
		{
			return (m_itCurrent++)->second;
		}

		typename T::mapped_type PeekNextValue() const
		{
			return m_itCurrent->second;
		}

		typename T::key_type PeekNextKey() const
		{
			return m_itCurrent->first;
		}

		FIteratorMapConst<T> &operator=(FIteratorMapConst<T> &rhs)
		{
			m_itCurrent = rhs.m_itCurrent;
			m_itEnd	= rhs.m_itEnd;
			return *this;
		}

		const typename T::mapped_type* PeekNextValuePtr() const
		{
			return &(m_itCurrent->second);
		}

		void MoveNext() const
		{
			++m_itCurrent;
		}
	};

}; //LostPeterFoundation

#endif