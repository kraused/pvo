
/// vim: tabstop=4:expandtab:hlsearch:

/* Copyright 2010 University of Lugano. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of the University of Lugano.
 */

#ifndef PVO_REPORT_H_INCLUDED
#define PVO_REPORT_H_INCLUDED 1

#ifdef __cplusplus
extern "C" {
#endif

/// Report an error together with its position
void pvo_error( const char* filename, const char* funct, long line,
                const char* err, ... );
/// Report an error together with its position and kill the application
void pvo_die  ( const char* filename, const char* funct, long line,
                const char* err, ... );
/// Display a warning together with its position
void pvo_warn ( const char* filename, const char* funct, long line,
                const char* err, ... );
/// Display an information for the user. This is intended for reporting
/// information which is useful for developers to spot (e.g., performance)
/// problems. It is not meant for printing output for users.
void pvo_info ( const char* filename, const char* funct, long line,
                const char* err, ... );

/// Macro wrappers which provide the filename, funct and line
/// values
#define PVO_ERROR( err, ... )   pvo_error( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )
#define PVO_DIE( err, ... )     pvo_die  ( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )
#define PVO_WARN( err, ... )    pvo_warn ( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )
#define PVO_INFO( err, ... )    pvo_info ( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )

#ifdef __cplusplus
}
#endif

#endif

