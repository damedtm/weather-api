#include <iostream>
#include <curl/curl.h>


int main()
{
    //Dynamically allocates memory on the Heap, uses a pointer to create a handle named curl
    CURL* curl = curl_easy_init();

    //Error handling for curl not being proprely initalized
    if (curl == nullptr)
    {
        std::cerr << "Failed to initialize libcurl.\n";
        return 1;
    }

    // Configures the target URL inside the session handle's internal state
    //Set the url to Open Meteo's API key for testing.
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&hourly=temperature_2m");

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
    double timeTaken = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &statusCode
    );

    curl_easy_getinfo(
        curl,
        CURLINFO_TOTAL_TIME,
        &timeTaken
    );

    std::cout << "\nHTTP status code: "
              << statusCode
              << '\n'
              << "\nThe total time taken was: "
              << timeTaken
              << "s";

    curl_easy_cleanup(curl);

    return 0;
}