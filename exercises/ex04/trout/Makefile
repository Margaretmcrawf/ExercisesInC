recv_dgram.o: recv_dgram.c recv_dgram.h trout.h
	gcc -c recv_dgram.c

print_report.o: print_report.c print_report.h
	gcc -c print_report.c

send_probes.o: send_probes.c send_probes.h print_report.h recv_dgram.h
	gcc -c send_probes.c

trout.o: trout.c util.c send_probes.h recv_dgram.h print_report.h trout.h
	gcc -c trout trout.c util.c

trout: trout.o send_probes.o print_report.o recv_dgram.o
	gcc trout.o send_probes.o print_report.o recv_dgram.o -o trout
