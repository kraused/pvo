
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

program pvo_file_f
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

