
! vi: tabstop=4:expandtab:hlsearch

program pvo_vtu_file_f
    use mpi
    use pvo
    implicit none

    integer :: ierr, num
    integer*8 :: nnodes, ncells
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer(kind = PVO_VTU_FILE_KIND) :: fh
    real, dimension(3,9) :: pt
    integer, dimension(3) :: cia
    integer, dimension(9) :: cja
    integer, dimension(9) :: X
    real, dimension(2) :: Y
    integer, dimension(2) :: types
   
    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )


    cia = (/ 0, 8, 9 /)
    cja = (/ 0, 1, 2, 3, 4, 5, 6, 7, 8 /)
    X   = (/ 2, 2, 3, 0, -1, 2, 5, -7, 8 /)
    
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

    Y = (/ 2.3*pvo_world_rank(), -1.3*pvo_world_rank() /)

    types = (/ PVO_VTU_HEXAHEDRON, PVO_VTU_VERTEX /)


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
    
    call pvo_cookie_insert_var_float32( cookie, PVO_VAR_CELLDATA, 1, "Y"//char(0), Y, ierr )
    if( 0 .ne. ierr ) then
        write(*,*)  'pvo_cookie_insert_var_float32 failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    nnodes = 9
    ncells = 2
    call pvo_vtu_file_open( "test11"//char(0), cookie, nnodes, pt, ncells, cia, cja, types, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vtu_file_open failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_file_write( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_write failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_vtu_file_close( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vtu_file_close failed'
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

