#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <memory>
#include <iostream>


typedef short uint16_t;

int get_host_ip_from_url(char *url, char **p_host, uint16_t *port){
  int pos = 0;
  int host_offset = 0;
  int host_len = 0;
  int port_offset = 0;
  int port_len = 0;
  int has_port = 0;
  char *str_port;

  //http://10.10.10.10
  //http://10.10.10.10/
  //http://10.10.10.10:234
  //http://10.10.10.10:234/
  //http://[ff:ff::ff]  ipv6 format
  for(; url[pos] != 0; pos++){
    if(url[pos] == ':'){
      pos += 3;
      break;
    }
  }

  if(url[pos] == '['){
  //process ipv6 http://[ff:ff::ff] refer to RFC2732
    host_offset = pos+1;
    for(; (url[pos] != ']') && (url[pos] != 0); ++pos){
    }
    if(url[pos] == 0) {
      //TODO error handling, url format error;
    }
    host_len = pos - host_offset;
    
    //process port
    for(; (url[pos] != 0) && (url[pos] != '/'); pos++){
      if(url[pos] == ':'){
        has_port = 1; 
        port_offset = pos + 1;
      }
    }
    if(has_port) {
        port_len = pos - port_offset;
    }
  }
  else {
  //process ipv4 url
    host_offset = pos;
    for(; url[pos] != 0; pos++){         
      //process port
      if(url[pos] == ':'){
        has_port = 1;                    
        port_offset = pos + 1;
        host_len = pos - host_offset;
      }
      if(url[pos] == '/'){               
        break;
      }
    }
    if(has_port) {
        port_len = pos - port_offset;
    }
    if(!has_port) {
        host_len = pos - host_offset;
    }
  }
  
  
  //get port from url[port_offset:port_len]
  if(has_port){
    str_port = (char*)malloc(port_len+ 1);      
    memcpy(str_port, url + port_offset, port_len);
    str_port[port_len] = 0;              
    *(port) = atoi(str_port);            
  
    free(str_port);
  }
  else{
    *(port) = 80;
  }
  
  //get host from url[host_offset:host_len]
  *(p_host) = (char*)malloc(host_len + 1);      
  memcpy(*(p_host), url + host_offset, host_len);
  (*(p_host))[host_len] = 0;             
  
  return 0;
}

namespace mobile {
    static const std::string aaa = "hello";
}

int main(int argc, char* argv[]) {
    std::cout << mobile::aaa << std::endl;

char* url1 = "http://10.10.10.10";
char* url2 = "http://10.10.10.10/";
char* url3 = "http://10.10.10.10:234";
char* url4 = "http://10.10.10.10:234/";
char* urlipv6_1 = "http://[ff:ff::ff]";
char* urlipv6_2 = "http://[ff:ff::ff]/";
char* urlipv6_3 = "http://[ff:ff::ff]:88";
char* urlipv6_4 = "http://[ff:ff::ff]:88/";

uint16_t port = 0;
char* host;

get_host_ip_from_url(url1, &host, &port);
printf("url1: [%s] [%d]\n", host, port);
get_host_ip_from_url(url2, &host, &port);
printf("url2: [%s] [%d]\n", host, port);
get_host_ip_from_url(url3, &host, &port);
printf("url3: [%s] [%d]\n", host, port);
get_host_ip_from_url(url4, &host, &port);
printf("url4: [%s] [%d]\n", host, port);
get_host_ip_from_url(urlipv6_1, &host, &port);
printf("urlipv6_1: [%s] [%d]\n", host, port);
get_host_ip_from_url(urlipv6_2, &host, &port);
printf("urlipv6_2: [%s] [%d]\n", host, port);
get_host_ip_from_url(urlipv6_3, &host, &port);
printf("urlipv6_3: [%s] [%d]\n", host, port);
get_host_ip_from_url(urlipv6_4, &host, &port);
printf("urlipv6_4: [%s] [%d]\n", host, port);
}

