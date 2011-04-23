
! vi: tabstop=4:expandtab:hlsearch

program pvo_cookie_f
    use mpi
    use pvo
    implicit none

    integer :: ierr, num
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer, dimension(2) :: p_i32
    integer*8, dimension(2) :: p_i64
    real, dimension(2) :: p_f32
    real*8, dimension(2) :: p_f64

    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )

    call pvo_cookie_create( mod(pvo_world_rank(),2), cookie, ierr )
    if( 0 .ne. ierr ) then
        write(*,*), 'pvo_cookie_create failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_insert_var_int32( cookie, PVO_VAR_NODEDATA, 8, "I32"//char(0), p_i32, ierr )
    if( 0 .ne. ierr ) then
        write(*,*), 'pvo_cookie_insert_var_int32 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(1 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_insert_var_int64( cookie, PVO_VAR_CELLDATA, 8, "I64"//char(0), p_i64, ierr )
    if( 0 .ne. ierr ) then
        write(*,*), 'pvo_cookie_insert_var_int64 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(2 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_insert_var_float32( cookie, PVO_VAR_NODEDATA, 8, "F32"//char(0), p_f32, ierr )
    if( 0 .ne. ierr ) then
        write(*,*), 'pvo_cookie_insert_var_float32 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(3 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_insert_var_float64( cookie, PVO_VAR_CELLDATA, 8, "F64"//char(0), p_f64, ierr )
    if( 0 .ne. ierr ) then
        write(*,*), 'pvo_cookie_insert_var_float64 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(4 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    
    call pvo_cookie_remove_var( cookie, "F32"//char(0), ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_remove_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(3 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_remove_var( cookie, "I32"//char(0), ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_remove_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(2 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_remove_var( cookie, "I64"//char(0), ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_remove_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(1 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif


    call pvo_cookie_remove_var( cookie, "F64"//char(0), ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_remove_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_number_var( cookie, num, ierr )
    if(0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_number_var failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    if(0 .ne. num ) then
        write(*,*) 'wrong number of variables in cookie'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_remove_var( cookie, "UNKNOWN"//char(0), ierr )
    if(0 .eq. ierr ) then
        write(*,*) 'pvo_cookie_remove_var was expected to fail but returned zero'
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

