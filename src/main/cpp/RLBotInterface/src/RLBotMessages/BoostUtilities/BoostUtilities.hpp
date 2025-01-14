#ifndef BOOSTUTILITIES_HPP
#define BOOSTUTILITIES_HPP

#include "MessageStructs/ByteBuffer.hpp"
#include "ErrorCodes/ErrorCodes.hpp"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_sharable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>

#include "BoostConstants.hpp"

/*
This typedef is advice from one of the boost maintainers on how to make message queues work between 32 and 64 bit processes.
It looks pretty janky. I believe the reason it's not "fixed" in the library is that it can't be done consistently on windows vs linux.

"In general I regret putting message_queue in Interprocess, as IMHO it's not good enough to be in the library. Some people find it useful, though."
https://lists.boost.org/Archives/boost/2014/06/214746.php
*/
typedef boost::interprocess::message_queue_t< boost::interprocess::offset_ptr<void, boost::int32_t, boost::uint64_t>> interop_message_queue;

namespace BoostUtilities
{
	class QueueSender
	{
	private:
		interop_message_queue* pQueue;

	public:
		QueueSender(const char* pQueueName);
		RLBotCoreStatus sendMessage(void* pMessage, int messageSize);
	};

	class SharedMemReader
	{
	private:
		boost::interprocess::shared_memory_object* pSharedMem;
		boost::interprocess::named_sharable_mutex* pMutex;
		bool hasLock = false;

	public:
		SharedMemReader(const char* pName);
		ByteBuffer fetchData();
		void unlockMutex();
	};

	class SharedMemWriter
	{
	private:
		boost::interprocess::shared_memory_object* pSharedMem;
		boost::interprocess::named_sharable_mutex* pMutex;
		const char* pMemName;

	public:
		SharedMemWriter(const char* pName);
		~SharedMemWriter();
		void writeData(void* pAddress, int size);
	};
}

#endif