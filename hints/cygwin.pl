symlink "os/Cygwin.c", "OS.c" || die "Could not link os/Cygwin.c to OS.c\n";
delete $self->{LIBS};
$self->{OBJECT} .= ' obstack.o obstack_printf.o';
