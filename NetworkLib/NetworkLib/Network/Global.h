#pragma once
#pragma warning (disable:4996)

#include <winsdkver.h>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>
#include <boost/signals2.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <algorithm>
#include <condition_variable>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_priority_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

#include "Singleton.h"
#include "NetworkUser.h"
#include "LogSystem.h"
#include "Thread.h"
#include "Packet.h"
#include "Queue.h"

typedef enum
{
	DISPATCH_THREAD = 0,
	THREAD_MAX
}threadType;

extern ObjectQueue		gPkRecvQueue;
extern Thread*			gThread[THREAD_MAX];

extern std::atomic<int> gPort;