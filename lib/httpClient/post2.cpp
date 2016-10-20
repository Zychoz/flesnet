#include <curl/curl.h>

int main(int argc, char *argv[])
{
    CURLcode ret;
    CURL *hnd;
    
    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, "http://localhost:8086/write?db=mydb");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_HEADER, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "cpu_load_short,host=server02,region=us-west value=0.88");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)54);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    
    /* Here is a list of options the curl code used that cannot get generated
     as source easily. You may select to either not use them or implement
     them yourself.
     
     CURLOPT_WRITEDATA set to a objectpointer
     CURLOPT_WRITEFUNCTION set to a functionpointer
     CURLOPT_READDATA set to a objectpointer
     CURLOPT_READFUNCTION set to a functionpointer
     CURLOPT_SEEKDATA set to a objectpointer
     CURLOPT_SEEKFUNCTION set to a functionpointer
     CURLOPT_ERRORBUFFER set to a objectpointer
     CURLOPT_STDERR set to a objectpointer
     CURLOPT_HEADERFUNCTION set to a functionpointer
     CURLOPT_HEADERDATA set to a objectpointer
     
     */
    
    ret = curl_easy_perform(hnd);
    
    curl_easy_cleanup(hnd);
    hnd = NULL;
    
    return (int)ret;
}

char* add_data_item_to_request(
