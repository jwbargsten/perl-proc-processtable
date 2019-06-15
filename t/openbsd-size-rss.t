# Test size & RSS attribute on OpenBSD
# Added by Joelle Maslak <jmaslak@antelope.net>, used bugfix-51470 for
# outline.

use warnings;
use Test::More;
use Data::Dumper;

BEGIN {
  if ( $^O ne 'openbsd' ) {
    plan skip_all => 'OpenBSD-specific tests';
  }

  use_ok('Proc::ProcessTable');
}

SKIP: {
  $0 = "PROC_PROCESSTABLE_TEST_CMD";
  sleep(1);

  my ($ps) = grep {/^$$\s+/} map { chomp; s/^\s*//; $_ } `ps ww`;
  skip 'Cannot set process name', 1
    unless ($ps && $ps =~ /PROC_PROCESSTABLE_TEST_CMD/);

  $SIG{CHLD} = 'IGNORE';

  my $pid = fork;
  die "cannot fork" unless defined $pid;

  if ( $pid == 0 ) {
    #child
    sleep 10000;
  } else {
    #main
    sleep 1;
    my ($ps) = grep {/^$pid\s+/} map { chomp; s/^\s*//; $_ } `ps ww`;
    diag "process ($pid): $ps";

    my $pstmp = `ps -p $pid -o vsz,rss`;
    my (@lines) = split '\n', $pstmp;
    my (@parts) = split /\s+/, $lines[1];
    my $ps_vsize = $parts[0] * 1024;
    my $ps_rss   = $parts[1] * 1024;

    my $t   = Proc::ProcessTable->new;
    my ($p) = grep { $_->{pid} == $pid } @{ $t->table };
    is( $p->{size}, $ps_vsize, "Process size = ps vsz" );
    is( $p->{rss},  $ps_rss,   "Process rss  = ps rss" );
    diag "process info: " . Dumper($p);
    kill 9, $pid;
  }
}
done_testing();

