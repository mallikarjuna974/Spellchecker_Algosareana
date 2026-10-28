#include "Autocorrect.h"
#include <filesystem>
#include <iostream>
#include <string>

static std::string findDictionaryPath() {
    const std::string paths[] = {"data/dictionary.txt", "../data/dictionary.txt", "../../data/dictionary.txt"};
    for (const auto& path : paths) {
        if (std::filesystem::exists(path)) return path;
    }
    return "data/dictionary.txt";
}

int main(int argc, char* argv[]) {
    std::string dictionaryPath = (argc >= 2) ? argv[1] : findDictionaryPath();
    Autocorrect autocorrect;
    if (!autocorrect.initialize(dictionaryPath)) {
        std::cerr << "Error: could not load dictionary: " << dictionaryPath << "\n"
                  << "Run the program from the project root, or pass the dictionary path.\n";
        return 1;
    }
    std::cout << "====================================\n"
              << "  C++ AUTOCORRECT ENGINE\n"
              << "====================================\n"
              << "Dictionary words : " << autocorrect.dictionarySize() << "\n"
              << "Trie graph nodes  : " << autocorrect.graphNodeCount() << "\n"
              << "Enter a word (or 'exit' to quit).\n\n";
    std::string input;
    while (true) {
        std::cout << "Word> ";
        if (!(std::cin >> input)) break;
        if (input == "exit" || input == "quit") break;
        if (autocorrect.isCorrect(input)) {
            std::cout << "Correct spelling: " << input << "\n\n";
            continue;
        }
        auto suggestions = autocorrect.suggest(input, 5);
        if (suggestions.empty()) {
            std::cout << "No close suggestions found. Try a shorter or differently spelled word.\n\n";
            continue;
        }
        std::cout << "Unknown word: " << input << "\nTop suggestions:\n";
        for (std::size_t i = 0; i < suggestions.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << suggestions[i].word
                      << "  [edit distance: " << suggestions[i].editDistance
                      << ", score: " << static_cast<int>(suggestions[i].score) << "]\n";
        }
        std::cout << "Autocorrect: " << suggestions.front().word << "\n\n";
    }
    std::cout << "Goodbye.\n";
    return 0;
}
