#!/bin/bash

# vi: tabstop=4:expandtab

# Copyright 2010 University of Lugano. All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
# 
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
# 
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of the University of Lugano.

CompilerC=$1
CompilerF=$2

function GetSymbolC
{
    cat << EOF > conftest.c

void pvo_symbol( int *ierr )
{
    *ierr = 0;
}

EOF
    $CompilerC -o conftest.c.o -c conftest.c

    S=$(nm conftest.c.o | awk '$2 == "T" || $2 == "D"' | grep -i pvo_symbol | awk '{print $3}')

    rm conftest.c
    rm conftest.c.o

    echo $S
}

function GetSymbolF
{
    cat << EOF > conftest.F90

subroutine pvo_symbol( ierr )
    integer, intent(out) :: ierr

    ierr = 0
end subroutine

EOF
    $CompilerF -o conftest.F90.o -c conftest.F90

    S=$(nm conftest.F90.o | awk '$2 == "T" || $2 == "D"' | grep -i pvo_symbol | awk '{print $3}')

    rm conftest.F90
    rm conftest.F90.o

    echo $S
}

function GetFortranName
{
    exec perl -e '
        my $SymbolC = $ARGV[0];
        my $SymbolF = $ARGV[1];

        my $Case = "?";
        if($SymbolF =~ m/pvo_symbol/) {
            $Case = "LC";
        }
        if($SymbolF =~ m/PVO_SYMBOL/) {
            $Case = "UC";
        }
        if("?" eq "$Case") {
            exit 1;
        }

        my $i = index lc $SymbolF, $SymbolC;
        my $j = length($SymbolF) - ($i + length($SymbolC));

        my $Rep = "";
        if($i > 0) {
            $Rep = " ## " . $Rep;
        }
        $Rep = $Rep . $Case;
        if($j > 0) {
            $Rep = $Rep . " ## ";
        }

        my $Str = lc $SymbolF;
        $Str =~ s/$SymbolC/$Rep/;

        print "$Str";
    ' $1 $2
}

GetFortranName $(GetSymbolC) $(GetSymbolF)

# For testing
# ( GetFortranName "_pvo_symbol" "_pvo_symbol"    )
# ( GetFortranName "_pvo_symbol" "_PVO_SYMBOL"    )
# ( GetFortranName "_pvo_symbol" "_pvo_symbol_"   )
# ( GetFortranName "_pvo_symbol" "_PVO_SYMBOL_"   )
# ( GetFortranName "_pvo_symbol" "_pvo_symbol__"  )
# ( GetFortranName "_pvo_symbol" "_PVO_SYMBOL__"  )
# ( GetFortranName "_pvo_symbol" "__pvo_symbol_"  )
# ( GetFortranName "_pvo_symbol" "__PVO_SYMBOL_"  )
# ( GetFortranName "_pvo_symbol" "__pvo_symbol__" )
# ( GetFortranName "_pvo_symbol" "__PVO_SYMBOL__" )
# ( GetFortranName "foo" "baz" )

