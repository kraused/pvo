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


# Write the test file
cat << EOF > conftest.F90

subroutine pvo_symbol( ierr )
    integer, intent(out) :: ierr

    ierr = 0
end subroutine

EOF
$1 -o conftest.o -c conftest.F90

# Find the symbol
SYMBOL=$(nm conftest.o | awk '$2 == "T"' | grep -i pvo_symbol | awk '{print $3}')

# Underscores
USCORES=$(echo $SYMBOL | sed "s+pvo_symbol++g" | sed "s+PVO_SYMBOL++g")

# lower or uppercase
if [ "pvo_symbol${USCORES}" == "$SYMBOL" ]; then
    VAR="LC"
else
    VAR="UC"
fi

echo "${VAR} ## ${USCORES}"

