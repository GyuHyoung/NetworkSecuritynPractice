#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 9190

int SSU_SEC_SSL_client(int sd, char* msg);

int main(void)
{
    int sd;
    struct sockaddr_in sa;
    char* serv_ip = "192.168.152.180";
    char* msg = "Hello World!!";
  
    sd = socket(AF_INET, SOCK_STREAM, 0);
   
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(serv_ip);  
    sa.sin_port = htons(PORT);                
   
    if(connect(sd, (struct sockaddr*)&sa, sizeof(sa)) == -1){
        return -1;
    }

    SSU_SEC_SSL_client(sd, msg);
   
    close(sd);
   
    return 0;
}


int SSU_SEC_SSL_client(int sd, char* msg)
{
    int size;
    SSL_CTX *ctx;
    SSL *ssl;
    X509 *server_cert;
    char buf[4096];
   
    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();

    ctx = SSL_CTX_new(TLSv1_2_client_method());
    /* SSLv3_client_method()*/
    /* TLSv1_client_method()*/
    /* TLSv1_1_client_method()*/
    /* TLSv1_2_client_method()*/
    /* SSLv23_client_method(): Negotiate highest available SSL/TLS version */

    ssl = SSL_new(ctx); 
   
    SSL_set_fd(ssl, sd);
    if(SSL_connect(ssl) == -1){
        return -1;
    }
   
    printf("\nSSL connection using %s\n", SSL_get_cipher(ssl));
   
    server_cert = SSL_get_peer_certificate(ssl);
    printf("\nServer certificate:\n");
    printf("subject: %s\n", X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0));
    printf("issuer: %s\n", X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0));
    X509_free(server_cert);
   
    if(SSL_write(ssl, msg, strlen(msg)) == -1){
        return -1;
    }
   
    if((size = SSL_read(ssl, buf, sizeof(buf) - 1)) == -1){
        return -1;
    }

    buf[size] = 0;
    printf("\n%s\n\n", buf);
    
    SSL_shutdown(ssl);
   
    SSL_free(ssl);
    SSL_CTX_free(ctx);
   
    return 0;
}
