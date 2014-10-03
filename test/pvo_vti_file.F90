
! vi: tabstop=4:expandtab:hlsearch

! Copyright 2010 University of Lugano. All rights reserved.
!
! Redistribution and use in source and binary forms, with or without modification, are
! permitted provided that the following conditions are met:
!
!    1. Redistributions of source code must retain the above copyright notice, this list of
!       conditions and the following disclaimer.
!
!    2. Redistributions in binary form must reproduce the above copyright notice, this list
!       of conditions and the following disclaimer in the documentation and/or other materials
!       provided with the distribution.
!
! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
! OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
! AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
! CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
! SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
! ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
! NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
! ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
!
! The views and conclusions contained in the software and documentation are those of the
! authors and should not be interpreted as representing official policies, either expressed
! or implied, of the University of Lugano.

#include "pvo_configf.h"

program pvo_vti_file_f
    use pvo
#if 1 == PVO_INCLUDE_MPIF_H
    implicit none
#include "mpif.h"
#else
    use mpi
    implicit none
#endif

    integer :: ierr, num
    integer(kind = PVO_COOKIE_KIND) :: cookie
    integer(kind = PVO_VTU_FILE_KIND) :: fh
    integer, dimension(6) :: whole_extent, local_extent
    integer, dimension(3,3,3) :: X
    real   , dimension(2,2,2) :: Y
    integer :: i, j, k

    call mpi_init( ierr )
    call pvo_init( MPI_COMM_WORLD, ierr )

    do k = 1, 3
        do j = 1, 3
            do i = 1, 3
                X(i,j,k) = i + 3*(j + 3*k)
            end do
        end do
    end do

    Y = pvo_world_rank()

    whole_extent = [ 0, 2*pvo_world_size(), 0, 2, 0, 2 ]
    local_extent = [ 2*pvo_world_rank(),        &
                     2*pvo_world_rank() + 2,    &
                     0, 2, 0, 2 ]


    ! Write one file per processig element.
    call pvo_cookie_create( pvo_world_rank(), cookie, ierr )
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

    call pvo_vti_file_open( "test14"//char(0), cookie, whole_extent, 0, &
                            [ 0.0, 0.0, 0.0 ], [ 0.1, 0.1, 0.1 ],       &
                            local_extent, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vti_file_open failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_file_write( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_write failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_vti_file_close( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vti_file_close failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_cookie_delete( cookie, ierr )
    if( 0 .ne. ierr ) then
        write(*,*) 'pvo_cookie_delete failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    ! Write one file in total.
    call pvo_cookie_create( 0, cookie, ierr )
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

    call pvo_vti_file_open( "test15"//char(0), cookie, whole_extent, 0, &
                            [ 0.0, 0.0, 0.0 ], [ 0.1, 0.1, 0.1 ],       &
                            local_extent, fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vti_file_open failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_file_write( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_file_write failed'
        call mpi_abort( MPI_COMM_WORLD, 1, ierr )
    endif

    call pvo_vti_file_close( fh, ierr )
    if(0 .ne. ierr) then
        write(*,*) 'pvo_vti_file_close failed'
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

