
#pragma once

#include "CommonPublic.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

typedef boost::shared_mutex CommonMutex;
typedef boost::shared_lock<CommonMutex> CommonReadLock;
typedef boost::unique_lock<CommonMutex> CommonWriteLock;
