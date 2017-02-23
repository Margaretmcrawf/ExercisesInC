#include "trout.h"

int max_ttl = 30;
int nprobes = 2;

/* other global variables */

int seq = 0;

char recvbuf[BUFSIZE];
char sendbuf[BUFSIZE];
Rec *rec = (Rec *) sendbuf;

int sendfd, recvfd;
int pipefd[2];              /* the pipe for the alarm handler */

Sockaddr *sasend;    /* socket addresses for various purposes */
Sockaddr *sarecv;
Sockaddr *salast;
Sockaddr *sabind;

socklen_t salen;                    /* length of a socket address */
int datalen = sizeof (Rec);         /* length of the data in a datagram */

u_short sport;                      /* source UDP port # */
u_short dport = 32768 + 668;        /* destination port -- hopefully unused */
                                    /* 668 = the neighbor of the beast */
Timeval sendtv[1];
Timeval recvtv[1];
Timeval difftv[1];


/* process_ip: extracts all the info from an incoming ICMP packet

     Just for kicks, I changed the BSD-style names of the ICMP
     errors to Linux-style names, mostly so that they will be
     consistent with the changes I made in the kernel and so my
     head won't explode. */

int process_ip (struct ip *ip, int len)
{
  int hlen1, hlen2, icmplen;
  struct icmp *icmp;
  struct ip *hip;
  struct udphdr *udp;

  hlen1 = ip->ip_hl << 2;                        /* length of IP header */
  icmp = (struct icmp *) (recvbuf + hlen1);
  icmplen = len - hlen1;

  if (icmplen < 8 + 20 + 8) return 0;

  if (icmp->icmp_type != ICMP_TIME_EXCEEDED &&
      icmp->icmp_type != ICMP_DEST_UNREACH)
    return 0;

  /* hip is the header of the enclosed IP packets, supposedly
     the header of the packet that caused the error */

  hip = (struct ip *) (recvbuf + hlen1 + 8);
  if (hip->ip_p != IPPROTO_UDP) return 0;

  hlen2 = hip->ip_hl << 2;
  udp = (struct udphdr *) (recvbuf + hlen1 + 8 + hlen2);

  if (udp->source != htons (sport)) return 0;
  if (udp->dest != htons (dport + seq)) return 0;

  /* now we know it's an ICMP packet caused by a UDP
     datagram sent by us and sent to the port we happen to
     be sending to.  It's probably one of ours. */

  if (icmp->icmp_type == ICMP_TIME_EXCEEDED) {
    if (icmp->icmp_code == ICMP_EXC_TTL) {
      return -2;
    } else {
      return 0;
    }
  }

  if (icmp->icmp_type == ICMP_DEST_UNREACH) {
    if (icmp->icmp_code == ICMP_PORT_UNREACH) {
      return -1;
    } else {
      return 0;
    }
  }
}

/* recv_dgram: reads all incoming datagrams and checks for
   returning ICMP packets.
   returns -3 on timeout,
           -2 on ICMP time exceeded in transit (we reached a router)
	   -1 on ICMP port unreachable (we reached the destination)
	    0 on ICMP that has nothing to do with us  */

  /* as Stevens points out in Section 18.5 of Unix Network Programming,
     many programs with alarms have a race condition, which is that
     the alarm might go off before we get to the recvfrom, in which
     case it does nothing and the recvfrom might wait indefinitely.

     In earlier versions of this code, this problem seemed to pop
     up occasionally (although I am not positive about that).

     The use of select here solves that problem.  When the alarm
     goes off, the alarm handler sends a message through the pipe,
     which is one of the things select waits for.

     When select returns, we know that we have received a datagram
     OR the alarm has gone off OR both.  We then use rset to find
     out which, and deal with it.

     According to the specification of select, it should not be possible
     to get to the recvfrom unless there is a datagram waiting, and
     therefore the recvfrom should never block.  Nevertheless, it sometimes
     does, which is why, when we opened it, we set the NONBLOCK flag
     and why, if it fails (errno = EAGAIN) we just go on. */

int recv_dgram ()
{
  int err;
  socklen_t len;
  ssize_t n;
  struct ip *ip;
  int maxfdp1 = max (recvfd, pipefd[0]) + 1;
  fd_set rset[1];  
  FD_ZERO (rset);

  alarm(3);       /* set the timeout alarm to handle dropped packets */

  while (1) {
    FD_SET (recvfd, rset);
    FD_SET (pipefd[0], rset);

    n = select (maxfdp1, rset, NULL, NULL, NULL);
    if (n < 0 && errno != EINTR) {
      err_sys ("select error");
    }

    if (FD_ISSET (recvfd, rset)) {
      len = salen;
      n = recvfrom (recvfd, recvbuf, sizeof(recvbuf), 0, sarecv, &len);
      err = errno;
      Gettimeofday (recvtv, NULL);   /* get time of packet arrival */
      if (n < 0 && err != EAGAIN) {
	err_sys ("recvfrom error");
      }
    }

    if (FD_ISSET (pipefd[0], rset)) {
      Read (pipefd[0], &n, 1);
      return -3;                 /* timeout */
    }

    ip = (struct ip *) recvbuf;
    return process_ip (ip, n);
  }
}
