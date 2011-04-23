
! vi: tabstop=4:expandtab:hlsearch

program chunk_f
    use mpi
    use pvo
    implicit none

#include "pvof.h"

    integer :: ierr, ni, n_u, n_v, n_r
    integer*8 :: nnodes, ncells, N
    real*8 :: t0, t1, t2, t3, r_min, r_max, gamma
    real, dimension(:,:,:,:), allocatable :: pts
    integer, dimension(:), allocatable :: cia
    integer, dimension(:,:,:,:), allocatable :: cja
    real*8, dimension(:,:), allocatable :: U
    real, dimension(:), allocatable :: V
    integer, dimension(:), allocatable :: types, ranks
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer(kind = PVO_VTU_FILE_KIND) :: fh
    namelist /input/ r_min, r_max, gamma, n_u, n_v, n_r, ni
    logical :: file_exists

    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )

    call mpi_barrier( MPI_COMM_WORLD, ierr )
    t0 = mpi_wtime()

    inquire( file = "chunk_f.in", exist = file_exists )
    if( file_exists ) then
        open (55, file = 'chunk_f.in', status = 'old')
        read (55, nml = input)
        close(55)
    else
        write(*,*) 'cannot open chunk_f.in'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    end if

    call decompose( n_r, r_min, r_max )

    nnodes = (n_u+1)*(n_v+1)*(n_r+1)
    ncells =  n_u   * n_v   * n_r

    allocate( pts(3,n_u+1,n_v+1,n_r+1), cia(ncells+1), cja(8,n_u,n_v,n_r), &
              U(3,nnodes), V(ncells), types(ncells), ranks(ncells) )


    if( 0 .eq. pvo_world_rank() ) then
        write(*,*) '+-----------------------------------------+'
        write(*,*) '|         PVO CHUNK VTU BENCHMARK         |'
        write(*,*) '+-----------------------------------------+'
        write(*,*) ''
        write(*,*) 'PVO_DEFAULT_LOW_IO_LAYER : ?'
        write(*,*) 'no of cores              : ', pvo_world_size()
        write(*,*) 'no of nodes              : ', nnodes
        write(*,*) 'no of cells              : ', ncells
        write(*,*) 'no of islands            : ', ni
    end if

    call create_chunk( n_u, n_v, n_r, r_min, r_max, gamma, nnodes, pts, &
                       ncells, cia, cja, types, U, V )
    ranks = pvo_world_rank()


    call pvo_cookie_create( mod(pvo_world_rank(),ni), cookie, ierr )
    if( 0 .ne. ierr ) then
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_insert_var_float64( cookie, PVO_VAR_NODEDATA, 3, "U"//char(0), U, ierr )
    if( 0 .ne. ierr ) then
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    call pvo_cookie_insert_var_float32( cookie, PVO_VAR_CELLDATA, 1, "V"//char(0), V, ierr )
    if( 0 .ne. ierr ) then
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif
    call pvo_cookie_insert_var_int32( cookie, PVO_VAR_CELLDATA, 1, "ranks"//char(0), ranks, ierr )
    if( 0 .ne. ierr ) then
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_vtu_file_open( "chunk"//char(0), cookie, nnodes, pts, ncells, cia, cja, types, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vtu_file_open failed'
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

    N = (3*4 + 3*8)*nnodes + (4 + 8*4 + 1 + 4 + 4)*ncells
    call mpi_allreduce( MPI_IN_PLACE, N, 1, MPI_INTEGER8, MPI_SUM, MPI_COMM_WORLD, ierr )

    if( 0 .eq. pvo_world_rank() ) then
        write(*,*) 'time [sec]                 : ', t2-t1
        write(*,*) 'bandwidth [MB/sec]         : ', (1.0*N)/(1048576*(t2-t1))
    end if

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

    call mpi_barrier( MPI_COMM_WORLD, ierr )
    t3 = mpi_wtime()

    if( 0 .eq. pvo_world_rank() ) then
        write(*,*) 'total execution time [sec] : ', t3-t0
    end if

    call pvo_quit( ierr )
    call mpi_finalize( ierr )

contains

    subroutine decompose( n_r, r_min, r_max )
        integer, intent(inout) :: n_r
        real*8, intent(inout) :: r_min, r_max
        real*8 :: dr
        integer :: n        

        dr = (r_max - r_min)/n_r
        n  = (1.0*n_r)/pvo_world_size()
        
        if( pvo_world_size() .eq. pvo_world_rank()+1 ) then
            ! Need to account for the possibility that n_r is not 
            ! divisible by the number of cores. This is a dump load
            ! balancing function but sufficies for meshes which are
            ! large enough
            n_r = n_r - (pvo_world_size()-1)*n
        else
            n_r = n
        end if

        r_min = r_min + pvo_world_rank()*n*dr
        r_max = r_min + n_r*dr
    end subroutine

    subroutine create_chunk( n_u, n_v, n_r, r_min, r_max, gamma, &
                             nnodes, pts, ncells, cia, cja, types, &
                             U, V )
        implicit none

        integer, intent(in) :: n_u, n_v, n_r
        real*8, intent(in) :: r_min, r_max, gamma
        integer*8, intent(in) :: nnodes, ncells
        real, dimension(3,n_u+1,n_v+1,n_r+1), intent(out) :: pts
        integer, dimension(ncells+1), intent(out) :: cia
        integer, dimension(8,n_u,n_v,n_r), intent(out) :: cja
        real*8, dimension(3,nnodes), intent(out) :: U
        real, dimension(ncells), intent(out) :: V
        integer, dimension(ncells), intent(out) :: types
        integer*8 :: i, iu, iv, ir, k
        real*8 :: dr, z, theta, phi, r, t, uu, vv
        integer, dimension(:), allocatable :: seed
        integer :: n


        dr = (r_max - r_min)/n_r

        ! We compute the values on the sphere by projection from the
        ! plane with this z value on the sphere. The closer this value
        ! is to 1.0, the lower is the curvature
        z = sqrt(0.5*(1 - gamma*gamma))

        do ir = 0, n_r
            do iv = 0, n_v
                do iu = 0, n_u
                    uu = -z + ( (2*z)/(1.0*n_u) )*iu
                    vv = -z + ( (2*z)/(1.0*n_v) )*iv
                    r  = r_min + ir*dr
                    t  = sqrt(uu*uu + vv*vv + gamma*gamma)

                    pts(1,iu+1,iv+1,ir+1) = uu   *r/t
                    pts(2,iu+1,iv+1,ir+1) = vv   *r/t
                    pts(3,iu+1,iv+1,ir+1) = gamma*r/t
                end do
            end do
        end do

        do ir = 0, n_r-1
            do iv = 0, n_v-1
                do iu = 0, n_u-1
#undef  INDEX
#define INDEX(iu,iv,ir) ( (iu) + (n_u+1)*(iv) + (n_u+1)*(n_v+1)*(ir) )
                    cja(1,iu+1,iv+1,ir+1) = INDEX(iu  ,iv  ,ir  )
                    cja(2,iu+1,iv+1,ir+1) = INDEX(iu+1,iv  ,ir  )
                    cja(3,iu+1,iv+1,ir+1) = INDEX(iu+1,iv+1,ir  )
                    cja(4,iu+1,iv+1,ir+1) = INDEX(iu  ,iv+1,ir  )
                    cja(5,iu+1,iv+1,ir+1) = INDEX(iu  ,iv  ,ir+1)
                    cja(6,iu+1,iv+1,ir+1) = INDEX(iu+1,iv  ,ir+1)
                    cja(7,iu+1,iv+1,ir+1) = INDEX(iu+1,iv+1,ir+1)
                    cja(8,iu+1,iv+1,ir+1) = INDEX(iu  ,iv+1,ir+1)
                end do
            end do
        end do

        do i = 1, ncells+1
            cia(i) = 8*(i-1)
        end do
        
        types = PVO_VTU_HEXAHEDRON

        call random_seed( size = n )
        allocate( seed(n) )
        seed = 0
        call random_seed( put = seed )

        do i = 1, nnodes
            call random_number( U(1,i) )
            call random_number( U(2,i) )
            call random_number( U(3,i) )
        end do
        do i = 1, ncells
            call random_number( V(i) )
        end do

        deallocate( seed )
    end subroutine

end program

