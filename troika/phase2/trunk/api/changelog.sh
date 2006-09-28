#!/bin/sh

# If this is a Subversion working copy and if (Arthur de Jong's) svn2cl script
# is available, then generate a new ChangeLog.

svn status >/dev/null 2>&1 && which svn2cl
if [[ "0" = $? ]]; then
        svn2cl
fi

