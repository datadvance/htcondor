/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Copyright (C) DATADVANCE, 2018-2022
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

// Common code used by DATADVANCE patches.

#ifndef _DA_CONDOR_UTILS_H_
#define _DA_CONDOR_UTILS_H_

#include "condor_debug.h"

// Try to synchronize directory existence and its content by flushing NFS cache.
// If `change_dir` is true also tries to change current directory to path.
// Returns `false` on fail.
bool try_sync_directory_on_nfs( const std::string& path, const int timeout_sec, const bool change_dir = false );

// Print backtrace on linux.
void dprintf_backtrace( const int level = D_ALWAYS );

#endif // _DA_CONDOR_UTILS_H_
