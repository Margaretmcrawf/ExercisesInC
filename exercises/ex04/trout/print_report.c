
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