
! vi: tabstop=4:expandtab

#include "pvof.h"

! FIXME On the long run pvo_consts should be
!       created by cmake
module pvo_consts

    integer, parameter :: PVO_COOKIE_KIND = 8
    integer, parameter :: PVO_FILE_KIND   = 8

    integer, parameter :: PVO_VAR_NODEDATA = 0
    integer, parameter :: PVO_VAR_CELLDATA = 1

    integer, parameter :: PVO_FILE_BYTE_ORDER_MACHINE = -1
    integer, parameter :: PVO_FILE_BYTE_ORDER_LE      =  0
    integer, parameter :: PVO_FILE_BYTE_ORDER_BE      =  1

    integer, parameter :: PVO_VTU_VERTEX        = 1
    integer, parameter :: PVO_VTU_LINE          = 3
    integer, parameter :: PVO_VTU_TRIANGLE      = 5
    integer, parameter :: PVO_VTU_QUADRILATERAL = 9
    integer, parameter :: PVO_VTU_TETRAHEDRON   = 10
    integer, parameter :: PVO_VTU_HEXAHEDRON    = 12
    integer, parameter :: PVO_VTU_WEDGE         = 13
    integer, parameter :: PVO_VTU_PYRAMID       = 14

    integer, parameter :: PVO_VTU_FILE_KIND = 8
    integer, parameter :: PVO_VTP_FILE_KIND = 8

end module

module pvo

    ! Types must be in external module so that we can
    ! use them in the interface. See
    ! http://software.intel.com/en-us/blogs/2006/10/05/domestic-or-imported/
    use pvo_consts

    interface
        integer function pvo_world_size
        end function

        integer function pvo_world_rank
        end function

        subroutine pvo_init( comm, ierr )
            integer, intent(in)  :: comm
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_quit( ierr )
            integer, intent(in) :: ierr
        end subroutine

        subroutine pvo_physical_node_uid( uid )
            integer, intent(out) :: uid
        end subroutine

        subroutine pvo_cookie_create( color, cookie, ierr )
            use pvo_consts

            integer, intent(in) :: color
            integer(kind = PVO_COOKIE_KIND), intent(out) :: cookie
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_cookie_delete( cookie, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(inout) :: cookie
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_cookie_insert_var_int32( cookie, grp, ncomps, name, ptr, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer, intent(in) :: grp, ncomps
            character(len = *), intent(in) :: name
            integer, dimension(*), intent(in) :: ptr
            integer, intent(out) :: ierr
        end subroutine
    
        subroutine pvo_cookie_insert_var_int64( cookie, grp, ncomps, name, ptr, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer, intent(in) :: grp, ncomps
            character(len = *), intent(in) :: name
            integer*8, dimension(*), intent(in) :: ptr
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_cookie_insert_var_float32( cookie, grp, ncomps, name, ptr, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer, intent(in) :: grp, ncomps
            character(len = *), intent(in) :: name
            real, dimension(*), intent(in) :: ptr
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_cookie_insert_var_float64( cookie, grp, ncomps, name, ptr, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer, intent(in) :: grp, ncomps
            character(len = *), intent(in) :: name
            real*8, dimension(*), intent(in) :: ptr
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_cookie_remove_var( cookie, name, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            character(len = *), intent(in) :: name
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_cookie_number_var( cookie, num, ierr )
            use pvo_consts

            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer, intent(out) :: num, ierr
        end subroutine

        subroutine pvo_file_create( filename, bo, cookie, fh, ierr )
            use pvo_consts

            character(len = *), intent(in) :: filename
            integer, intent(in) :: bo
            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer(kind = PVO_FILE_KIND), intent(out) :: fh
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_file_write( fh, ierr )
            use pvo_consts
            
            integer(kind = PVO_FILE_KIND), intent(in) :: fh
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_file_delete( fh, ierr )
            use pvo_consts

            integer(kind = PVO_FILE_KIND), intent(inout) :: fh
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_vtu_file_open( filename, cookie, nnodes, pts, ncells, cia, cja, types, fh, ierr )
            use pvo_consts
            
            character(len = *), intent(in) :: filename
            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer*8, intent(in) :: nnodes, ncells
            real, dimension(*), intent(in) :: pts
            integer, dimension(*), intent(in) :: cia, cja, types
            integer(kind = PVO_VTU_FILE_KIND), intent(out) :: fh
            integer, intent(out) :: ierr 
        end subroutine

        subroutine pvo_vtu_file_close( fh, ierr )
            use pvo_consts

            integer(kind = PVO_VTU_FILE_KIND), intent(inout) :: fh
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_vtp_file_open( filename, cookie, npoints, pts, fh, ierr )
            use pvo_consts

            character(len = *), intent(in) :: filename
            integer(kind = PVO_COOKIE_KIND), intent(in) :: cookie
            integer*8, intent(in) :: npoints
            real, dimension(*), intent(in) :: pts
            integer(kind = PVO_VTP_FILE_KIND), intent(out) :: fh
            integer, intent(out) :: ierr
        end subroutine

        subroutine pvo_vtp_file_close( fh, ierr )
            use pvo_consts

            integer(kind = PVO_VTP_FILE_KIND), intent(inout) :: fh
            integer, intent(out) :: ierr
        end subroutine

    end interface

end module

