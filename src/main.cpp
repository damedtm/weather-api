#include <iostream>
#include <curl/curl.h>

// Callback function to handle incoming network data chunks
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    // Calculate total byte size of incoming chunk
    size_t totalSize = size * nmemb;

    // Cast the generic void pointer back to a std::string pointer
    std::string* buffer = static_cast<std::string*>(userp);

    // Append raw char buffer bytes into our C++ heap string
    buffer->append(static_cast<char*>(contents), totalSize);

    // Return exact number of processed bytes so libcurl knows transfer succeeded
    return totalSize;
}

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

    //Set the url to Open Meteo's API key for testing - Configures the target URL inside the session handle's internal state
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&hourly=temperature_2m");

    // String buffer to hold incoming JSON data on the heap
    std::string responseString;
    
    // Register write callback and target payload buffer pointer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

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

    //Print status code, time taken and buffer size
    std::cout << "\nHTTP status code: "
              << statusCode
              << '\n'
              << "\nThe total time taken was: "
              << timeTaken
              << "s";
    std::cout << "Captured buffer size: " << responseString.size() << " bytes\n\n";

    // Print captured string from memory to confirm
    std::cout << "First 150 characters of stored JSON:\n" 
              << responseString.substr(0, 150) << "...\n";
    curl_easy_cleanup(curl);

    return 0;
}