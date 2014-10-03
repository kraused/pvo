
/// vim: tabstop=4:expandtab:hlsearch

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

#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED 1

/**
 * parse_cmdline_T(): Helper routines for parsing
 * input to a program.
 *
 * These are very primitive routines. All they do
 * is reading a line from stdint, choping of possible
 * comments and then returning the value read as
 * a variable of type T.
 *
 * The easiest way to run an application using these
 * functions is to write a small input file, e.g.
 * 	5.0 # value one
 *	1   # value two
 * and then run the application as follows:
 * % ./app < app.in
 *
 * The ordering of the values in the input file is
 * obviously very important.
 */

static int parse_cmdline_int()
{
    int myrank, v;

    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    if( 0 == myrank )
    {
        char buf[1024];
        int  i, k;

        fgets( buf, sizeof(buf), stdin );

        k = strlen(buf);
        for( i = 0; i < k; ++i )
            if( '#' == buf[i] )
                buf[i] = '\0';

        v = atoi(buf);
    }

    MPI_Bcast( &v, 1, MPI_INT, 0, MPI_COMM_WORLD );
    return v;
}

static double parse_cmdline_double()
{
    int    myrank;
    double v;

    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    if( 0 == myrank )
    {
        char buf[1024];
        int  i, k;

        fgets( buf, sizeof(buf), stdin );

        k = strlen(buf);
        for( i = 0; i < k; ++i )
            if( '#' == buf[i] )
                buf[i] = '\0';

        v = atof(buf);
    }

    MPI_Bcast( &v, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
    return v;
}

#endif

