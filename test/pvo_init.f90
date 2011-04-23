
! vi: tabstop=4:expandtab:hlsearch

program pvo_init_f
    use mpi
    use pvo
    implicit none

    integer :: ierr

    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )
    if( ierr .ne. 0 ) then
        write(*,*) 'pvo_init failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_quit( ierr )
    if( ierr .ne. 0 ) then
        write(*,*) 'pvo_quit failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call mpi_finalize( ierr )

end program

