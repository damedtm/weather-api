#include <iostream>
#include <curl/curl.h>


int main()
{
    CURL* curl = curl_easy_init();

    if (curl == nullptr)
    {
        std::cerr << "Failed to initialize libcurl.\n";
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        std::cerr << "Request failed: "
                  << curl_easy_strerror(result)
                  << '\n';

        curl_easy_cleanup(curl);
        return 1;
    }

    long statusCode = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &statusCode
    );

    std::cout << "\nHTTP status code: "
              << statusCode
              << '\n';

    curl_easy_cleanup(curl);

    return 0;
}