#include "recv_dgram.h"
#include "print_report.h"
#include "send_probes.h"


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