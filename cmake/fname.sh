#!/bin/bash
# vi: tabstop=4:expandtab

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

