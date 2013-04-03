use warnings;
use Test::More;
use Data::Dumper;

BEGIN { 
  if( $^O eq 'cygwin' ) {
      plan skip_all => 'Test irrelevant on cygwin';
  }

  use_ok('Proc::ProcessTable'); 
}


$SIG{CHLD} = 'IGNORE';

my $pid = fork;
die "cannot fork" unless defined $pid;

if ( $pid == 0 ) {
  #child
  $0 = '01234567890123456789';
  sleep 10000;
} else {
  #main
  sleep 1;
  my $t = Proc::ProcessTable->new;
  my ($p) = grep { $_->{pid} == $pid } @{ $t->table };
  like( $p->{cmndline}, qr/01234567890123456789/, "modulo 20 commandline bugfix" );
  kill 9, $pid;
}
done_testing();

