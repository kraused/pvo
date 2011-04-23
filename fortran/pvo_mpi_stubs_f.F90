
! vi: tabstop=4:expandtab

module mpi

    integer, parameter :: MPI_INTEGER8 = 20

    integer, parameter :: MPI_SUM    = 0

    integer, parameter :: MPI_COMM_WORLD = 0
    integer, parameter :: MPI_COMM_NULL  = 0
   
    common /pvo_mpi_f_in_place/ MPI_IN_PLACE

    interface
        real*8 function mpi_wtime()
        end function
    end interface

contains

    subroutine mpi_finalize( ierr )
        integer, intent(out) :: ierr
        ierr = 0
    end subroutine

    subroutine mpi_abort( comm, retval, ierr )
        integer, intent(in) :: comm, retval
        integer, intent(out) :: ierr

        stop
    end subroutine

    subroutine mpi_barrier( comm, ierr )
        integer, intent(in) :: comm
        integer, intent(out) :: ierr
        ierr = 0
    end subroutine

end module

