Compilation
------------

1) UDP
-------
 Server: gcc rscmnd.c -o rscmnd_udp
 Client: gcc rccmnd.c -o rccmnd_udp

2) TCP
-------
 Server: gcc rscmnd.c -o rscmnd_tcp
 Client: gcc rccmnd.c -o rccmnd_tcp


Execution
----------

1) UDP
-------
 Server: ./rscmnd_udp -p <port_number>
         ./rscmnd_udp -h ==> to get the help 

 Client: ./rccmnd_udp -s <server_ip> -p <port_number> -c <command> -n <execution time> -d <delay>
         ./rccmnd_udp -h ==> to get the help 



2) TCP
-------
 Server: ./rscmnd_tcp -p <port_number>
         ./rscmnd_tcp -h ==> to get the help 

 Client: ./rccmnd_tcp -s <server_ip> -p <port_number> -c <command> -n <execution time> -d <delay>
         ./rccmnd_tcp -h ==> to get the help 
