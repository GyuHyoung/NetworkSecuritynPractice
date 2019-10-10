#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/rsa.h>      
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
 
#define PORT 9190

int SSU_SEC_SSL_server(int sd, char* msg, char* cert, char* key);

int main(void)
{
    int listen_sd;
    int sd;
    struct sockaddr_in sa_serv;
    struct sockaddr_in sa_cli;
    size_t client_len;
    char* certfile = "servercert.pem";
    char* keyfile = "serverkey.pem";
    char* msg = "Welcome to TLS Server!!";
  
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
   
    memset(&sa_serv, 0, sizeof(sa_serv));
    sa_serv.sin_family = AF_INET;
    sa_serv.sin_addr.s_addr = INADDR_ANY;
    sa_serv.sin_port = htons(PORT);
 
    if(bind(listen_sd, (struct sockaddr*)&sa_serv, sizeof(sa_serv)) == -1){
        return -1;
    }
   
    if(listen(listen_sd, 5) == -1){
        return -1;
    }

    client_len = sizeof(sa_cli);
    sd = accept(listen_sd, (struct sockaddr*)&sa_cli, &client_len);
    close(listen_sd);

    SSU_SEC_SSL_server(sd, msg, certfile, keyfile);

    close(sd);

  
    return(0);
}

int SSU_SEC_SSL_server(int sd, char* msg, char* cert, char* key)
{
    int size;
    SSL_CTX *ctx;
    SSL *ssl;
    X509 *client_cert;
    char buf[4096];
   
    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();
    ctx = SSL_CTX_new(SSLv23_server_method());  
    /* SSLv3_server_method()*/
    /* TLSv1_server_method()*/
    /* TLSv1_1_server_method()*/
    /* TLSv1_2_server_method()*/
    /* SSLv23_server_method(): Negotiate highest available SSL/TLS version */

    if(SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM) <= 0) {      
        return -1;
    }
    if(SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) <= 0) {
        return -1;
    }

    if(!SSL_CTX_check_private_key(ctx)) {
        return -1;
    }
   
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sd);
    if(SSL_accept(ssl) == -1){
        return -1;
    }
   
    printf("\nSSL connection using %s\n", SSL_get_cipher(ssl));
   
    client_cert = SSL_get_peer_certificate(ssl);
    if(client_cert != NULL) {
        printf("\nClient certificate:\n");
        printf("t subject: %s\n", X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0));
        printf("t issuer: %s\n", X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0));
        X509_free(client_cert);
    } else {
        printf("\nClient does not have certificate.\n");
    }
   
    if((size = SSL_read(ssl, buf, sizeof(buf)-1)) == -1){
        return -1;
    }
    
    buf[size] = 0;
    printf("\n%s\n\n", buf);
   
    if(SSL_write(ssl, msg, strlen(msg)) == -1){
        return -1;
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
   
    return(0);
}
 
