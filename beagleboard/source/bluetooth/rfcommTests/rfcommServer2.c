#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

static const bdaddr_t bt_bdaddr_any   = {{0, 0, 0, 0, 0, 0}};

int main()
{
    printf("starting rfcommServer ...\n");

    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);


    // allocate socket
    printf("allocate socket ...\n");
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    printf("bind socket to first available bluetooth adapter ...\n");
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = bt_bdaddr_any;
    //str2ba("00:1B:DC:05:7E:24", &loc_addr.rc_bdaddr);
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    printf("put socket into listening mode ...\n");
    listen(s, 1);

    // accept one connection
    printf("accept connections ...\n");
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    printf("read data form the client ...\n");
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    printf("close connection ...\n");
    close(client);
    close(s);

    return 0;
}
