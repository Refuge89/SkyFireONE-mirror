/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2010-2017 Oregon <http://www.oregoncore.com/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SKYFIRE_COMMON_H
#define SKYFIRE_COMMON_H

 // config.h needs to be included 1st
 // TODO this thingy looks like hack , but its not, need to
 // make separate header however, because It makes mess here.
#ifdef HAVE_CONFIG_H
// Remove Some things that we will define
// This is in case including another config.h
// before Skyfire config.h
#ifdef PACKAGE
#undef PACKAGE
#endif //PACKAGE
#ifdef PACKAGE_BUGREPORT
#undef PACKAGE_BUGREPORT
#endif //PACKAGE_BUGREPORT
#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#endif //PACKAGE_NAME
#ifdef PACKAGE_STRING
#undef PACKAGE_STRING
#endif //PACKAGE_STRING
#ifdef PACKAGE_TARNAME
#undef PACKAGE_TARNAME
#endif //PACKAGE_TARNAME
#ifdef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif //PACKAGE_VERSION
#ifdef VERSION
#undef VERSION
#endif //VERSION

# include "config.h"

#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION
#endif //HAVE_CONFIG_H

#include "Define.h"
#include "Dynamic/UnorderedMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#if PLATFORM == PLATFORM_WINDOWS
#define STRCASECMP stricmp
#define stricmp _stricmp
#define putenv _putenv
#define access _access
#define chdir _chdir
#define rmdir _rmdir
#define getcwd _getcwd
#define swab _swab
#define unlnk _unlink
#define mkdir _mkdir
#define tempnam _tempnam
#define strdup _strdup
#define fdopen _fdopen
#else
#define STRCASECMP strcasecmp
#endif

#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>

#include "LockedQueue.h"
#include "Threading.h"

#include <ace/Basic_Types.h>
#include <ace/Guard_T.h>
#include <ace/RW_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>

#if PLATFORM == PLATFORM_WINDOWS
#  include <ace/config-all.h>
// XP winver - needed to compile with standard leak check in MemoryLeaks.h
// uncomment later if needed
//#define _WIN32_WINNT 0x0501
#  include <ws2tcpip.h>
//#undef WIN32_WINNT
#else
#  include <sys/types.h>
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
#  include <netdb.h>
#endif

#if COMPILER == COMPILER_MICROSOFT

#include <float.h>

#define I32FMT "%08I32X"
#define I64FMT "%016I64X"
#define snprintf _snprintf
#define atoll __atoi64
#define vsnprintf _vsnprintf
#define finite(X) _finite(X)
#define llabs _abs64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I32FMT "%08X"
#define I64FMT "%016llX"

#endif

#define UI64FMTD ACE_UINT64_FORMAT_SPECIFIER
#define UI64LIT(N) ACE_UINT64_LITERAL(N)

#define SI64FMTD ACE_INT64_FORMAT_SPECIFIER
#define SI64LIT(N) ACE_INT64_LITERAL(N)

#define SIZEFMTD ACE_SIZE_T_FORMAT_SPECIFIER

inline float finiteAlways(float f) { return finite(f) ? f : 0.0f; }

#define atol(a) strtoul( a, NULL, 10)

#define STRINGIZE(a) #a

// used for creating values for respawn for example
#define MAKE_PAIR64(l, h)  uint64(uint32(l) | (uint64(h) << 32))
#define PAIR64_HIPART(x)   (uint32)((uint64(x) >> 32) & UI64LIT(0x00000000FFFFFFFF))
#define PAIR64_LOPART(x)   (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))

#define MAKE_PAIR32(l, h)  uint32(uint16(l) | (uint32(h) << 16))
#define PAIR32_HIPART(x)   (uint16)((uint32(x) >> 16) & 0x0000FFFF)
#define PAIR32_LOPART(x)   (uint16)(uint32(x)         & 0x0000FFFF)

enum TimeConstants
{
	MINUTE = 60,
	HOUR = MINUTE * 60,
	DAY = HOUR * 24,
	WEEK = DAY * 7,
	MONTH = DAY * 30,
	YEAR = MONTH * 12,
	IN_MILLISECONDS = 1000
};

enum AccountTypes
{
	SEC_PLAYER = 0,
	SEC_MODERATOR = 1,
	SEC_GAMEMASTER = 2,
	SEC_ADMINISTRATOR = 3,
	SEC_CONSOLE = 4                                  // must be always last in list, accounts must have less security level always also
};

enum LocaleConstant
{
	LOCALE_enUS = 0,                                        // also enGB
	LOCALE_koKR = 1,
	LOCALE_frFR = 2,
	LOCALE_deDE = 3,
	LOCALE_zhCN = 4,
	LOCALE_zhTW = 5,
	LOCALE_esES = 6,
	LOCALE_esMX = 7,
	LOCALE_ruRU = 8
};

const uint8 TOTAL_LOCALES = 9;

extern char const* localeNames[TOTAL_LOCALES];

LocaleConstant GetLocaleByName(const std::string& name);

typedef std::vector<std::string> StringVector;

// we always use stdlibc++ std::max/std::min, undefine some not C++ standard defines (Win API and some other platforms)
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define SKYFIRE_GUARD(MUTEX, LOCK) \
  ACE_Guard< MUTEX > SKYFIRE_GUARD_OBJECT (LOCK); \
    if (SKYFIRE_GUARD_OBJECT.locked() == 0) ASSERT(false);

# define SKYFIRE_WRITE_GUARD(MUTEX, LOCK) \
  ACE_Write_Guard< MUTEX > SKYFIRE_GUARD_OBJECT (LOCK); \
    if (SKYFIRE_GUARD_OBJECT.locked() == 0) ASSERT(false);

# define SKYFIRE_READ_GUARD(MUTEX, LOCK) \
  ACE_Read_Guard< MUTEX > SKYFIRE_GUARD_OBJECT (LOCK); \
    if (SKYFIRE_GUARD_OBJECT.locked() == 0) ASSERT(false);

#endif
