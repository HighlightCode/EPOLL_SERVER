#ifndef SPSCQUEUE_H
#define SPSCQUEUE_H

#include <atomic>

/* Single Producer Single Consumer Queue*/
template <typename TElem, int QSize>
class SPSCQueue
{
public:
	SPSCQueue() : mHeadPos(0), mTailPos(0) {}
	~SPSCQueue() {}

	bool PushBack(const TElem &item);

	bool PopFront(TElem &item);

private:
	std::atomic<int> mHeadPos;
	std::atomic<int> mTailPos;

	TElem mQueueArray[QSize + 1];
};

template <typename TElem, int QSize>
bool SPSCQueue<TElem, QSize>::PushBack(const TElem &item)
{
	int currTailPos = mTailPos.load(std::memory_order_relaxed);

	int nextTailPos = (currTailPos + 1) % (QSize + 1);

	if (nextTailPos == mHeadPos.load(std::memory_order_acquire))
	{
		return false;
	}

	mQueueArray[currTailPos] = item;
	mTailPos.store(nextTailPos, std::memory_order_release);

	return true;
}

template <typename TElem, int QSize>
bool SPSCQueue<TElem, QSize>::PopFront(TElem &item)
{
	int currHeadPos = mHeadPos.load(std::memory_order_relaxed);

	if (currHeadPos == mTailPos.load(std::memory_order_acquire))
	{
		return false;
	}

	item = mQueueArray[currHeadPos];

	int nextHeadPos = (currHeadPos + 1) % (QSize + 1);

	mHeadPos.store(nextHeadPos, std::memory_order_release);

	return true;
}

#endif