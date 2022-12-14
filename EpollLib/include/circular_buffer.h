#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdlib.h>
#define OUT

class CircularBuffer
{
public:

	CircularBuffer(size_t capacity) : mARegionSize(0), mBRegionSize(0), mBRegionPointer(nullptr)
	{
		mBuffer = new char[capacity] ;
		mBufferEnd = mBuffer + capacity ;
		mARegionPointer = mBuffer ;
	}

	~CircularBuffer()
	{
		delete [] mBuffer ;
	}

	bool Peek(OUT char* destbuf, size_t bytes) const ;
	bool Read(OUT char* destbuf, size_t bytes) ;
	bool Write(const char* data, size_t bytes) ;


	void Remove(size_t len) ;

	size_t GetFreeSpaceSize()
	{
		if ( mBRegionPointer != nullptr )
			return GetBFreeSpace() ;
		else
		{
			if ( GetAFreeSpace() < GetSpaceBeforeA() )
			{
				AllocateB() ;
				return GetBFreeSpace() ;
			}
			else
				return GetAFreeSpace() ;
		}
	}

	size_t GetStoredSize() const
	{
		return mARegionSize + mBRegionSize ;
	}

	size_t GetContiguiousBytes() const 
	{
		if ( mARegionSize > 0 )
			return mARegionSize ;
		else
			return mBRegionSize ;
	}

	void* GetBuffer() const
	{
		if( mBRegionPointer != nullptr )
			return mBRegionPointer + mBRegionSize ;
		else
			return mARegionPointer + mARegionSize ;
	}

	
	void Commit(size_t len)
	{
		if ( mBRegionPointer != nullptr )
			mBRegionSize += len ;
		else
			mARegionSize += len ;
	}
    
    // return first part of buffer
	void* GetBufferStart() const
	{
		if ( mARegionSize > 0 )
			return mARegionPointer ;
		else
			return mBRegionPointer ;
	}


private:

	void AllocateB()
	{
		mBRegionPointer = mBuffer ;
	}

	size_t GetAFreeSpace() const
	{ 
		return (mBufferEnd - mARegionPointer - mARegionSize) ; 
	}

	size_t GetSpaceBeforeA() const
	{ 
		return (mARegionPointer - mBuffer) ; 
	}


	size_t GetBFreeSpace() const
	{ 
		if (mBRegionPointer == nullptr)
			return 0 ; 

		return (mARegionPointer - mBRegionPointer - mBRegionSize) ; 
	}

private:

	char*	mBuffer = nullptr;
	char*	mBufferEnd = nullptr;

	char*	mARegionPointer = nullptr;
	size_t	mARegionSize = 0;

	char*	mBRegionPointer = nullptr;
	size_t	mBRegionSize = 0;

};

#endif