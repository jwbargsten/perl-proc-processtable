#!/usr/bin/perl
use IPC::System::Simple qw(run);
use warnings;
use strict;

run("cpan-upload-http *.tar.gz");
