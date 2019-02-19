#pragma once

#ifdef BVE_VTUNE
#	include <ittnotify.h>
#else
// ReSharper disable CppInconsistentNaming
#	define __itt_sync_acquired(...)
#	define __itt_sync_cancel(...)
#	define __itt_sync_prepare(...)
#	define __itt_sync_prepare(...)
#	define __itt_sync_releasing(...)
// ReSharper restore CppInconsistentNaming
#endif
