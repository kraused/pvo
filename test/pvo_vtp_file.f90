
! vi: tabstop=4:expandtab:hlsearch

program pvo_vtp_file_f
    use mpi
    use pvo
    implicit none

    integer :: ierr, num
    integer*8 :: npoints
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer(kind = PVO_VTP_FILE_KIND) :: fh
    real, dimension(3,9) :: pt
    integer, dimension(9) :: X
   
    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )

    X   = (/ 1, 2, 3, 4, 5, 6, 7, 8, 9 /)
    
    pt(1,1) = 2*pvo_world_rank() + 0.5
    pt(2,1) = 2*pvo_world_rank() + 0.5
    pt(3,1) = 2*pvo_world_rank() + 0.5

    pt(1,2) = 2*pvo_world_rank() + 1.5
    pt(2,2) = 2*pvo_world_rank() + 0.5
    pt(3,2) = 2*pvo_world_rank() + 0.5

    pt(1,3) = 2*pvo_world_rank() + 1.5
    pt(2,3) = 2*pvo_world_rank() + 1.5
    pt(3,3) = 2*pvo_world_rank() + 0.5

    pt(1,4) = 2*pvo_world_rank() + 0.5
    pt(2,4) = 2*pvo_world_rank() + 1.5
    pt(3,4) = 2*pvo_world_rank() + 0.5

    pt(1,5) = 2*pvo_world_rank() + 0.5
    pt(2,5) = 2*pvo_world_rank() + 0.5
    pt(3,5) = 2*pvo_world_rank() + 1.5

    pt(1,6) = 2*pvo_world_rank() + 1.5
    pt(2,6) = 2*pvo_world_rank() + 0.5
    pt(3,6) = 2*pvo_world_rank() + 1.5

    pt(1,7) = 2*pvo_world_rank() + 1.5
    pt(2,7) = 2*pvo_world_rank() + 1.5
    pt(3,7) = 2*pvo_world_rank() + 1.5

    pt(1,8) = 2*pvo_world_rank() + 0.5
    pt(2,8) = 2*pvo_world_rank() + 1.5
    pt(3,8) = 2*pvo_world_rank() + 1.5

    pt(1,9) = 2*pvo_world_rank() + 1.75
    pt(2,9) = 2*pvo_world_rank() + 1.75
    pt(3,9) = 2*pvo_world_rank() + 1.75

    call pvo_cookie_create( mod(pvo_world_rank(),2), cookie, ierr )
    if( 0 .ne. ierr ) then
        write(*,*)  'pvo_cookie_create failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_insert_var_int32( cookie, PVO_VAR_NODEDATA, 1, "X"//char(0), X, ierr )
    if( 0 .ne. ierr ) then
        write(*,*)  'pvo_cookie_insert_var_int32 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    
    npoints = 9
    call pvo_vtp_file_open( "test12"//char(0), cookie, npoints, pt, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vtp_file_open failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_file_write( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_write failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_vtp_file_close( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vtp_file_close failed'
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

