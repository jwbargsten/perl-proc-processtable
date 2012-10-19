#!/usr/bin/perl
use IPC::System::Simple qw(run);
use warnings;
use strict;

system("make distclean");
run("rm -fR Proc-ProcessTable-* Proc-ProcessTable-*.tar.gz MANIFEST.bak");
