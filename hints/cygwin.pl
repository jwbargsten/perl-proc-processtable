my @files = ( "Cygwin.c", "obstack.c", "obstack.h", "obstack_printf.c" );

for my $f (@files) {
  symlink "os/$f", $f || die "Could not link os/$f to $f\n";
}

delete $self->{LIBS};
$self->{OBJECT} .= ' obstack.o obstack_printf.o';
