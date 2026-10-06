#  Weather API — Development & Engineering Journal

A chronologically ordered log of engineering hurdles, toolchain configurations, and design decisions encountered while building this application.

---

##  Entry 1: Toolchain Standardization & CMake Configuration
   2026-10-02

###  Objective
Set up a modern, reproducible C++ build pipeline on Windows using CMake, MinGW-w64, and `libcurl` without hardcoding system paths or relying on legacy configurations.

###  Issues Encountered
1. **Generator & Path Conflicts (`mingw32-make` not found):**
   - *Symptom:* `Get-Command mingw32-make` failed in PowerShell.
   - *Root Cause:* Mixed environment paths between MSYS2 `mingw64` and `ucrt64`. The binary `make.exe` was present in `ucrt64\bin`, but system environment variables contained lingering references to `mingw64`.

2. **Transitive Dependency Resolution (`OpenSSL` & `ZLIB`):**
   - *Symptom:* `find_package(CURL REQUIRED)` threw CMake errors citing missing `OPENSSL_CRYPTO_LIBRARY` and `ZLIB`.
   - *Root Cause:* Standard CMake modules do not automatically scan `C:/msys64/ucrt64` unless explicitly directed.

---

###  Solutions & Key Takeaways

1. **Environment Clean-Up:**
   - Standardized strictly on **UCRT64** (`C:\msys64\ucrt64\bin`) in Windows Environment Variables.
   - Selected UCRT64 over MINGW64 due to its native alignment with Microsoft's Universal C Runtime.

2. **Clean CMake Integration:**
   - Instead of passing cumbersome command-line flags (`-DOPENSSL_ROOT_DIR`), added the following block directly to `CMakeLists.txt`:
     ```cmake
     if(WIN32)
         list(APPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64")
     endif()
     ```
   - This keeps the build command clean (`cmake -G "MinGW Makefiles" -S . -B build`) while ensuring Windows builds find all UCRT64 header files and dynamic libraries.

3. **C++ Best Practices:**
   - Opted for explicit `std::` namespacing (e.g., `std::cout`, `std::cerr`) over `using namespace std;` to avoid namespace pollution and naming collisions as the application grows.

---

###  Result
- Build pipeline configures and generates cleanly without manual command-line overrides.
- Successfully executed a GET request via `libcurl` returning `HTTP 200` response data.

---

###   Next Steps
- Fetch data from a weather API
- Cache API responses with Redis
- Parse JSON responses

---

## Notes
- This was my first C++ project. My previous projects were in JavaScript and Python, where I only needed to ignore `.env` files. I learnt that C++ projects also generate build artifacts (`.exe`, `.o`, and the `build/` directory) that should be added to `.gitignore`.
- I learnt that MSYS2 offers several toolchain environments, and that UCRT64 is generally preferred over MINGW64 because it uses the modern Universal C Runtime.
- Learning CMake, C++, external libraries, dependencies, toolchains, and PATH configuration has been challenging but rewarding.

---

##  Entry 2: Live Weather Server Integration
   2026-10-02

###  Objective
Transition from boilerplate testing to querying a real live weather server (`Open-Meteo`).

###  Implementation
* Updated target endpoint to Open-Meteo's REST API using `CURLOPT_URL`.
* Successfully executed the HTTP GET request and verified a live JSON weather payload response.

---