
#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED 1

/// vim: tabstop=4:expandtab:hlsearch

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

