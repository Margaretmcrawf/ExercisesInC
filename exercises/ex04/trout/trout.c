#include "send_probes.h"
#include "recv_dgram.h"
#include "print_report.h"
#include "trout.h"

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

