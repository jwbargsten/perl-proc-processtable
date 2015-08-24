use warnings;
use Test::More;
use Data::Dumper;

BEGIN {
  if ( $^O eq 'cygwin' ) {
    plan skip_all => 'Test irrelevant on cygwin';
  }

  use_ok('Proc::ProcessTable');
}

SKIP: {
  $0 = "PROC_PROCESSTABLE_TEST_CMD";
  sleep(1);

  skip 'Cannot set process name', 1
    unless ( `ps hwwp $$` =~ /PROC_PROCESSTABLE_TEST_CMD/ );

  $SIG{CHLD} = 'IGNORE';

  my $pid = fork;
  die "cannot fork" unless defined $pid;

  if ( $pid == 0 ) {
    #child
    $0 = '';
    sleep 10000;
  } else {
    #main
    sleep 1;
    diag "process: " . `ps hwwp $pid`;
    my $t           = Proc::ProcessTable->new;
    my $cmnd_quoted = quotemeta('');
    my ($p) = grep { $_->{pid} == $pid } @{ $t->table };
    like( $p->{cmndline}, qr/$cmnd_quoted/, "odd process commandline bugfix ($cmnd_quoted)" );
    diag "process info: " . Dumper($p);
    kill 9, $pid;
  }
}
done_testing();

