#!/usr/bin/perl
use IPC::System::Simple qw(run);
use warnings;
use strict;

run("perl Makefile.PL");
run("make manifest");
run("make dist");
