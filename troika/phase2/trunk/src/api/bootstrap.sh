#!/bin/sh

# Bootstrap the autotools after cleanup.sh stripped the directory to the bare
# minimum. You don't need to run this unless you tinker with the autotools.
# Note: specific autotools versions are used. If you use a different version
# you'll probably also have to change the configuration files.

autoreconf --force --install -I config -I m4
