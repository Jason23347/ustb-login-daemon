#include <stdint.h>
#include <curl/curl.h>
#include <string.h>

#include "http_request.h"
#include "debug.h"
#include "log.h"

#define CURL_BUFFER_MAX_SIZE 0xffff

size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream)
{
    if ( strlen ((char *) stream) + strlen ((char *) buffer)
            > CURL_BUFFER_MAX_SIZE )
        return 0;

    strcat(stream, (char *) buffer);
    return size * nmemb;
}

/** return empty string if curl failed. */
char *get_ipv6_addr()
{
    char buf[CURL_BUFFER_MAX_SIZE] = "";
    char ip_addr[0x0030];

    CURL *curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://cippv6.ustb.edu.cn/get_ip.php");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_COULDNT_CONNECT) {
        return "";
    }else if (res != CURLE_OK) {
        perror(curl_easy_strerror(res));
        write_log("get ipv6 address failed: %s", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);

    debug("response: %s", buf);
    
    return strncpy(ip_addr, buf+13, 39);
}

int post_login(char *data)
{
    char buf[CURL_BUFFER_MAX_SIZE] = "";
    CURL *curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://202.204.48.82");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        perror(curl_easy_strerror(res));
        write_log("curl to login failed: %s", curl_easy_strerror(res));
        return -1;
    }
    curl_easy_cleanup(curl);

    /* TODO: check whether login  successed */

    return 0;
}