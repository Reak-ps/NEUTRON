// ============================================================================
// INCLUDES: Stuff we need because C++ doesn't have batteries included
// ============================================================================

#include <iostream>     // cout, cerr - printing shit to console
#include <string>       // Because char arrays are a nightmare
#include <filesystem>   // File stuff that finally works (C++17, took them 40 years)
#include <map>          // Dictionary / HashMap / whatever your fav language calls it
#include <fstream>      // Reading files without wanting to die
#include <vector>       // Dynamic array that isn't broken like raw arrays
#include <chrono>       // Time. Mostly for pretending we measure performance.

// ============================================================================
// HELPER FUNCTIONS: The ugly plumbing you don't want to see
// ============================================================================

/**
 * trim()
 * ------
 * Removes spaces from start and end of strings.
 *
 * Why does this not exist in std::string?
 * No fucking idea. Every other language has it.
 */
std::string trim(const std::string& str) {
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start])) {
        start++;  // Skip spaces like you skip gym
    }

    size_t end = str.length();
    while (end > start && std::isspace(str[end - 1])) {
        end--;  // More skipping. We're professional skippers now.
    }

    return str.substr(start, end - start);
}

/**
 * parseConfig()
 * -------------
 * Reads neutron.conf. If it doesn't exist, we cry and exit.
 *
 * Format: key = value
 * Spaces around '=' are ignored because we're not monsters.
 */
void parseConfig(const std::string& filename,
                 std::string& sourcePath,
                 std::map<std::string, std::string>& rules) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open " << filename << "!\n";
        std::cerr << "Did you create it? No? There's your problem.\n";
        throw std::runtime_error("Config file missing - skill issue");
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty()) continue;        // Empty lines? We don't care.
        if (line[0] == '#') continue;      // Comments? Also don't care.

        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;  // No '='? Invalid. Skip.

        std::string left = trim(line.substr(0, eqPos));
        std::string right = trim(line.substr(eqPos + 1));

        if (left == "source") {
            sourcePath = right;
        }
        else if (!left.empty() && left[0] == '.') {
            for (auto& c : left) c = std::tolower(c);  // Force lowercase. We're bullies.
            rules[left] = right;
        }
        // Unknown stuff? Ignored. We only handle what we like.
    }
    // File closes itself because RAII is the only good thing C++ has
}

// ============================================================================
// MAIN: Where the magic (and occasional disaster) happens
// ============================================================================

int main() {
    // ------------------------------------------------------------------------
    // CONFIG LOADING
    // ------------------------------------------------------------------------

    std::string sourcePath;
    std::map<std::string, std::string> rules;

    try {
        parseConfig("neutron.conf", sourcePath, rules);
    }
    catch (const std::exception& e) {
        std::cerr << "Abort: " << e.what() << "\n";
        std::cerr << "Go fix your config. I'll wait. (Actually I won't, I'm exiting)\n";
        return 1;
    }

    std::cout << "=== NEUTRON File Organizer ===\n";
    std::cout << "Source: " << sourcePath << "\n";
    std::cout << "Rules: " << rules.size() << " (hopefully enough)\n\n";

    // ------------------------------------------------------------------------
    // COUNTERS: For statistics that make us look professional
    // ------------------------------------------------------------------------

    int totalFiles = 0;
    int organizedFiles = 0;
    int skippedFiles = 0;
    int errorFiles = 0;

    std::map<std::string, int> stats;  // "We moved X things to Y" - bragging rights

    // ------------------------------------------------------------------------
    // THE ACTUAL WORK
    // ------------------------------------------------------------------------

    if (!std::filesystem::exists(sourcePath)) {
        std::cerr << "ERROR: Source folder doesn't exist: " << sourcePath << "\n";
        std::cerr << "Are you sure you typed it right? Or did it run away?\n";
        return 1;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(sourcePath)) {

        if (!entry.is_regular_file()) continue;  // Folders? Not our problem.

        totalFiles++;

        std::string ext = entry.path().extension().string();
        for (auto& c : ext) c = std::tolower(c);  // Lowercase everything. Consistency is key.

        std::string targetFolder = "Misc";  // The "I have no idea what this is" folder

        if (rules.count(ext) > 0) {
            targetFolder = rules[ext];  // Found a home for this lost file
        }

        // Build destination path
        std::filesystem::path destDir = std::filesystem::path(sourcePath) / targetFolder;

        // Create folder. If it exists, nothing happens. Magic.
        std::filesystem::create_directories(destDir);

        std::filesystem::path destFile = destDir / entry.path().filename();

        // Name conflict? Skip. We don't do overwrites here. Too dangerous.
        if (std::filesystem::exists(destFile)) {
            std::cout << "[SKIP] " << entry.path().filename() << " (already there, not touching it)\n";
            skippedFiles++;
            continue;
        }

        // THE MOVE
        try {
            std::filesystem::rename(entry.path(), destFile);  // Fast. Efficient. No copy+delete BS.

            std::cout << "[OK] " << entry.path().filename() << " → " << targetFolder << "/\n";
            organizedFiles++;
            stats[targetFolder]++;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "[ERR] " << entry.path().filename() << ": " << e.what() << "\n";
            std::cerr << "      (Probably in use or you don't have permissions. Or Mercury is in retrograde.)\n";
            errorFiles++;
        }
    }

    // ------------------------------------------------------------------------
    // SUMMARY: Time to flex our numbers
    // ------------------------------------------------------------------------

    std::cout << "\n=== SUMMARY ===\n";
    std::cout << "Total:     " << totalFiles << "\n";
    std::cout << "Organized:  " << organizedFiles << "\n";
    std::cout << "Skipped:    " << skippedFiles << "\n";
    std::cout << "Errors:     " << errorFiles << "\n";

    if (errorFiles > 0) {
        std::cout << "\nYeah, not perfect. But hey, " << organizedFiles << " files are happy now.\n";
    }

    std::cout << "\nBy folder:\n";
    for (const auto& [folder, count] : stats) {
        std::cout << "  " << folder << ": " << count << "\n";
    }

    std::cout << "\nNEUTRON out. Files organized. Chaos defeated.\n";

    return 0;
}