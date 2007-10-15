#!/bin/bash

HEADER=/tmp/ripple_header
FILE=/tmp/ripple_tempfile

echo "/*
 * \$URL: \$
 * \$Revision: \$
 * \$Author: \$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */

" > $HEADER

for file in `find -name "*.java"`
do
    svn propset svn:keywords "LastChangedDate LastChangedRevision LastChangedBy HeadURL Id" ${file}

    if grep Revision\: ${file} > /dev/null; then
        echo -n
    else
        cp ${file} $FILE && cat $HEADER > ${file} && cat $FILE >> ${file}
    fi
done
