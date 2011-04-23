
! vi: tabstop=4:expandtab:hlsearch

program rand_f
    use mpi
    use pvo
    implicit none

#include "pvof.h"

    integer :: ierr, ni
    integer*8 :: N
    real, dimension(:,:), allocatable :: P
    real*8, dimension(:,:), allocatable :: U
    real*8 :: t0, t1, t2, t3
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer(kind = PVO_VTP_FILE_KIND) :: fh
    namelist /input/ N, ni
    logical :: file_exists

    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )

    call mpi_barrier( MPI_COMM_WORLD, ierr )
    t0 = mpi_wtime()

    inquire( file = "rand_f.in", exist = file_exists )
    if( file_exists ) then
        open (55, file = 'rand_f.in', status = 'old')
        read (55, nml = input)
        close(55)
    else
        write(*,*) 'cannot open rand_f.in'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    end if

    allocate ( P(3,N), U(3,N) )

    if( 0 .eq. pvo_world_rank() ) then
        write(*,*) '+----------------------------------------+'
        write(*,*) '|         PVO RAND VTP BENCHMARK         |'
        write(*,*) '+----------------------------------------+'
        write(*,*) ''
        write(*,*) 'PVO_DEFAULT_LOW_IO_LAYER : ?'
        write(*,*) 'no of cores              : ', pvo_world_size()
        write(*,*) 'no of particles per core : ', N
        write(*,*) 'no of islands            : ', ni
    end if

    call create_random_points( N, P, U )

    call pvo_cookie_create( mod(pvo_world_rank(),ni), cookie, ierr )
    if( 0 .ne. ierr ) then
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_insert_var_float64( cookie, PVO_VAR_NODEDATA, 3, "U"//char(0), U, ierr )
    if( 0 .ne. ierr ) then
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    
    call pvo_vtp_file_open( "rand"//char(0), cookie, N, P, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vtp_file_open failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call mpi_barrier( MPI_COMM_WORLD, ierr )
    t1 = mpi_wtime()

    call pvo_file_write( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_write failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call mpi_barrier( MPI_COMM_WORLD, ierr )
    t2 = mpi_wtime()

    if( 0 .eq. pvo_world_rank() ) then
        write(*,*) 'time [sec]                 : ', t2-t1
        write(*,*) 'bandwidth [MB/sec]         : ', (36.0*N*pvo_world_size())/(1048576*(t2-t1))
    end if

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

    call mpi_barrier( MPI_COMM_WORLD, ierr )
    t3 = mpi_wtime()

    if( 0 .eq. pvo_world_rank() ) then
        write(*,*) 'total execution time [sec] : ', t3-t0
    end if

    call pvo_quit( ierr )
    call mpi_finalize( ierr )

contains

#ifdef PVO_HAVE_MPI
    subroutine compute_offset( o )
        integer, dimension(3), intent(out) :: o
        integer, dimension(3) :: cartdim
        integer :: ierr
       
        cartdim = (/ 0, 0, 0 /)
        call mpi_dims_create( pvo_world_size(), 3, cartdim, ierr )
 
        o(1) = mod(pvo_world_rank(),cartdim(1))
        o(2) = mod((pvo_world_rank()/cartdim(1)),cartdim(2))
        o(3) = pvo_world_rank()/(cartdim(1)*cartdim(2))
    end subroutine
#endif

    subroutine create_random_points( N, P, U )
        integer*8, intent(in) :: N
        integer*8 :: i
        real, dimension(3,N) :: P
        real*8, dimension(3,N) :: U
        integer, dimension(3) :: o

#ifdef PVO_HAVE_MPI
        call compute_offset( o )
#else
        o = (/ 0, 0, 0 /) 
#endif

        call srand( 0 )
        do i = 1, N
            P(1,i) = rand() + o(1)
            P(2,i) = rand() + o(2)
            P(3,i) = rand() + o(3)

            U(1,i) = rand()
            U(2,i) = rand()
            U(3,i) = rand()
        end do
    end subroutine

end program

