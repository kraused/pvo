
! vi: tabstop=4:expandtab:hlsearch

program pvo_file_f
    use mpi
    use pvo
    implicit none

    integer :: ierr, num
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer(kind = PVO_FILE_KIND) :: fh
    integer, dimension(2) :: p_i32
    real*8, dimension(2) :: p_f64

    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )

    call pvo_cookie_create( mod(pvo_world_rank(),2), cookie, ierr )
    if( 0 .ne. ierr ) then
        write(*,*)  'pvo_cookie_create failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_insert_var_int32( cookie, PVO_VAR_NODEDATA, 8, "I32"//char(0), p_i32, ierr )
    if( 0 .ne. ierr ) then
        write(*,*)  'pvo_cookie_insert_var_int32 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    
    call pvo_cookie_insert_var_float64( cookie, PVO_VAR_CELLDATA, 8, "F64"//char(0), p_f64, ierr )
    if( 0 .ne. ierr ) then
        write(*,*)  'pvo_cookie_insert_var_float64 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_file_create( "test10", PVO_FILE_BYTE_ORDER_MACHINE, cookie, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_create failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_file_delete( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_delete failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_delete( cookie, ierr )
    if( 0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_delete failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    

    call pvo_quit( ierr )
    call mpi_finalize( ierr )

end program

