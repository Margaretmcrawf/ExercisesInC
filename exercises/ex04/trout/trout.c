#include "recv_dgram.c"

/* NOTES: system calls beginning with a capital letter are Stevens's
   wrapper functions.  Each one invokes the method and checks the
   return value.  If the call fails, it invokes err_sys, which prints
   the error message and quits.

   Types that begin with a capital letter are usually typedefs
   that I added because (1) I hate seeing the word struct all over
   the place, and (2) it lets me pretend I am writing Java. */

/* sig_alrm: alarm handler sends a message to the process through
   a pipe, causing select to return */

void sig_alrm (int signo)
{
  Write (pipefd[1], "", 1);  /* write 1 null byte to pipe */
  return;
}

/* sub_tv: subtract minus from plus and put the result in res */

void sub_tv (Timeval *plus, Timeval *minus, Timeval *res)
{
  res->tv_sec = plus->tv_sec - minus->tv_sec;
  res->tv_usec = plus->tv_usec - minus->tv_usec;

  if (res->tv_usec < 0) {
    res->tv_sec--;
    res->tv_usec += 1000000;
  }
}

/* time_to_double: convert a Timeval to a double.  This only
   works with Timevals that are small (like the difference between
   two real Timevals) */

double time_to_double (Timeval *time)
{
  return time->tv_sec * 1000.0 + time->tv_usec / 1000.0;
}

/* print_report: prints all the information about a successful round trip */

void print_report ()
{
  int stat;
  double rtt, krtt;
  char str[NI_MAXHOST];
      
  stat = sock_cmp_addr (sarecv, salast, salen);

  /* if this reply comes from source different from the previous
     one, print the full host information */

  if (stat != 0) {
    stat = getnameinfo (sarecv, salen, str, sizeof(str), NULL, 0, 0);
    if (stat == 0) {
      printf (" %s (%s)", str, Sock_ntop_host (sarecv, salen));
    } else {
      printf (" %s", Sock_ntop_host (sarecv, salen));
    }
    memcpy (salast, sarecv, salen);
  }

  /* calculate and print the round trip time using user-level timestamps */

  sub_tv (recvtv, sendtv, difftv);
  rtt = time_to_double (difftv);

  printf ("  %.3f", time_to_double (difftv));
}

/* send_dgram: generate an outgoing UDP packet */

    /* the second effort send is a kludge to handle a funny
       thing, which is that the last host seems to refuse the
       second or third connection consistently, which might
       might mean that something breaks when we get the
       ICMP_DEST_UNREACH error.  The second attempt seems
       to succeed consistently. */

void send_dgram (int ttl)
{
  int n;

  rec->seq = seq++;
  sock_set_port (sasend, salen, htons(dport+seq));

  Gettimeofday (sendtv, NULL);
  n = sendto(sendfd, sendbuf, datalen, 0, sasend, salen);

  if (n==-1 && errno == ECONNREFUSED) {
    Gettimeofday (sendtv, NULL);
    n = sendto(sendfd, sendbuf, datalen, 0, sasend, salen);
  }

  if (n != datalen) {
    err_sys("sendto error");
  }
}

/* send_probes: sends a set of probes with the given ttl and
   then waits for the replies.  The weird TOS options are there
   as a signal to the kernel to identify clink packets so it can
   fill in the timestamps.  I am assuming that they don't have
   any actual effect. */

int send_probes (int ttl) 
{
  int i, probe, code, done;

  Setsockopt (sendfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
  bzero (salast, salen);

  printf ("%2d  ", ttl);
  fflush (stdout);

  done = 0; /* count the number of probes that generate an ICMP_DEST_UNREACH */

  for (probe = 0; probe < nprobes; probe++) {
    send_dgram (ttl);
    code = recv_dgram ();

    if (code == -3) {
      printf (" *");
    } else {
      print_report ();
    }

    if (code == -1) done++;
    fflush (stdout);
  }
  printf ("ms\n");
  return done;
}

/* loop_ttl: starting with ttl=1, gradually increase ttl until
   we start getting ICMP_DEST_UNREACH instead of ICMP_TIME_EXCEEDED */

void loop_ttl ()
{
  int ttl, done;

  Pipe (pipefd);     /* the pipe for the alarm handler */

  recvfd = socket (sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
  if (recvfd == -1) {
    if (errno == EPERM) {
      printf ("\nclink was unable to open a raw socket.  The most\n");
      printf ("likely cause is that you are not running it as root.\n");
      exit (1);
    } else {
      err_sys ("opening raw socket in clink");
    }
  }

  fcntl (recvfd, F_SETFL, O_NONBLOCK);
  setuid (getuid ());

  sendfd = socket (sasend->sa_family, SOCK_DGRAM, 0);

  sabind->sa_family = sasend->sa_family;
  sport = (getpid() & 0xffff) | 0x8000;       /* source UDP port # */
  sock_set_port (sabind, salen, htons(sport));
  Bind (sendfd, sabind, salen);

  Signal (SIGALRM, sig_alrm);

  for (ttl = 1; ttl <= max_ttl; ttl++) {
    done = send_probes (ttl);
    if (done > 0) break;
  }
}

int main (int argc, char **argv)
{
  int c;
  struct addrinfo *ai;
  char *host;

  opterr = 0;
  while ( (c = getopt (argc, argv, "m:")) != -1) {
    switch (c) {
    case 'm':
      if ( (max_ttl = atoi(optarg)) <= 1) {
	err_quit ("invalid -m value");
      }
      break;
    default:
      err_quit ("unrecognizd option: %c", c);
    }
  }

  if (optind != argc - 1) {
    err_quit ("usage: trout [ -m <maxttl>] <hostname>");
  }
  host = argv[optind];
  ai = Host_serv (host, NULL, 0, 0);

  printf ("trout to %s (%s): %d hops max, %d data bytes\n",
	  ai->ai_canonname,
	  Sock_ntop_host (ai->ai_addr, ai->ai_addrlen),
	  max_ttl, datalen);

  if (ai->ai_family != AF_INET) {
    err_quit ("unknown address family %d", ai->ai_family);
  }

  sasend = ai->ai_addr;
  salen = ai->ai_addrlen;
  sarecv = Calloc (1, salen);
  salast = Calloc (1, salen);
  sabind = Calloc (1, salen);

  loop_ttl ();
  exit (0);
}

