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

#include <execinfo.h>
#include <cxxabi.h>

#include <dirent.h>
#include <sys/time.h>

#include "da_condor_utils.h"

namespace {

std::string demangle( const char* const mangled ) {
        // just a guess, template names will go much wider
    size_t sz = 1024; 
    char* function = static_cast<char*>( malloc(sz) );

    int status;
    char* const ret = abi::__cxa_demangle( mangled, function, &sz, &status );
    std::string result;
    if ( ret && status == 0 ) {
            // return value may be a realloc() of the input
        function = ret;
        result = function;
    } else {
            // demangling failed, just pretend it's a C function with no args
        result = mangled;
        result += result.empty() ? "??" : "()";
    }

    free( function );
    return result;
}

}

bool try_sync_directory_on_nfs( const std::string& path , const int timeout_sec , const bool change_dir ) {
    if ( path.empty() ) {
        return false;
    }

    std::string parent = path;
    parent += parent[parent.size() - 1] == '/' ? ".." : "/..";

    timeval end;
    gettimeofday( &end, NULL );
    end.tv_sec += timeout_sec;

    auto syncdir = [ & ] ( const std::string& dirname, const bool parent ) -> bool {
        timeval now;
        int nitems = 0;
        while ( gettimeofday( &now, NULL ) == 0 && timercmp( &now, &end, < ) ) {
            if ( DIR* dir = opendir( dirname.c_str() ) ) {
                nitems = 0;
                while ( readdir( dir ) ) {
                    ++nitems;
                }
                closedir( dir );
                if ( parent ) {
                    if ( nitems ) {
                        return true;
                    }
                } else {
                    if ( !change_dir ) {
                        return true;
                    }
                    if ( chdir( dirname.c_str() ) == 0 ) {
                        return true;
                    }
                }
            }
            // Limit scans to 20 times in a second. Sleep for 50 milliseconds.
            usleep( 50000 );
        }
        return false;
    };

    if ( syncdir( parent, true ) ) {
        if ( syncdir( path, false ) ) {
            return true;
        }
        dprintf(D_ALWAYS, "Failed to synchronize directory: %s!\n", path.c_str());
    } else {
        dprintf(D_ALWAYS, "Failed to synchronize directory parent: %s!\n", parent.c_str());
    }
    return false;
}

void dprintf_backtrace( const int level ) {
    const size_t max_depth = 100;
    void* stack_addrs[max_depth];
    char** stack_strings;

    const size_t stack_depth = ::backtrace( stack_addrs, max_depth );
    stack_strings = ::backtrace_symbols( stack_addrs, stack_depth );

    for ( size_t i = 1; i < stack_depth; i++ ) {
        char *begin = 0, *end = 0;
            // find the parentheses and address offset surrounding the mangled name
        for ( char* j = stack_strings[i]; *j; ++j ) {
            if ( *j == '(' ) {
                begin = j;
            } else if ( *j == '+' ) {
                end = j;
            }
        }
        if ( begin && end ) {
            *begin++ = '\0';
            *end = '\0';
            dprintf(level, "#%d   %s: %s\n", (int)i, stack_strings[i], demangle(begin).c_str());
        } else {
            dprintf(level, "#%d   %s\n", (int)i, stack_strings[i]);
        }
    }
        // malloc()ed by backtrace_symbols
    free( stack_strings );
}
