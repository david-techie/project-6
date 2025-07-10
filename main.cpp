#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <map>

using namespace std;

// ANSI color codes for colorized output (optional)
const string COLOR_RESET = "\033[0m";
const string COLOR_GREEN = "\033[32m";
const string COLOR_YELLOW = "\033[33m";
const string COLOR_RED = "\033[31m";
const string COLOR_CYAN = "\033[36m";

// Trim leading/trailing whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Convert string to lowercase
string toLower(const string& s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

// Split sentence into words by spaces
vector<string> splitWords(const string& sentence) {
    vector<string> words;
    istringstream iss(sentence);
    string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

// Check if a character is vowel
bool isVowel(char c) {
    char lower = tolower(c);
    return lower=='a' || lower=='e' || lower=='i' || lower=='o' || lower=='u';
}

// Remove punctuation and spaces for palindrome check
string normalizeForPalindrome(const string& s) {
    string filtered;
    for (char ch : s) {
        if (isalnum(ch)) {
            filtered += tolower(ch);
        }
    }
    return filtered;
}

// Check palindrome
bool isPalindrome(const string& s) {
    string filtered = normalizeForPalindrome(s);
    int left = 0, right = (int)filtered.size() - 1;
    while (left < right) {
        if (filtered[left++] != filtered[right--]) return false;
    }
    return true;
}

// Detect sentence type (question, exclamation, statement)
string sentenceType(const string& s) {
    if (s.empty()) return "Unknown";
    char lastChar = s.back();
    if (lastChar == '?') return "Question";
    if (lastChar == '!') return "Exclamation";
    if (lastChar == '.') return "Statement";
    return "Unknown";
}

// Categorize sentence length
string lengthCategory(int charCount) {
    if (charCount < 30) return "Short";
    if (charCount < 80) return "Medium";
    return "Long";
}

// Percentage helper
double percent(int part, int total) {
    return total == 0 ? 0.0 : (100.0 * part) / total;
}

// Count unique words (case-insensitive)
int countUniqueWords(const vector<string>& words) {
    unordered_set<string> uniqueSet;
    for (auto& w : words) uniqueSet.insert(toLower(w));
    return (int)uniqueSet.size();
}

// Save analysis summary to JSON file
void saveToJSON(const map<string, string>& data, const string& filename) {
    ofstream jsonFile(filename);
    if (!jsonFile.is_open()) {
        cout << COLOR_RED << "[Error] Could not open " << filename << " for writing." << COLOR_RESET << endl;
        return;
    }
    jsonFile << "{\n";
    int count = 0;
    for (const auto& [key, val] : data) {
        jsonFile << "  \"" << key << "\": \"" << val << "\"";
        if (++count < (int)data.size()) jsonFile << ",";
        jsonFile << "\n";
    }
    jsonFile << "}\n";
    jsonFile.close();
    cout << COLOR_GREEN << "[Saved JSON to " << filename << "]" << COLOR_RESET << endl;
}

// Live char-by-char analysis (feature 1)
void liveMode() {
    cout << COLOR_CYAN << "Live input mode (press Enter to finish):\n" << COLOR_RESET;
    string input;
    char ch;
    int char_count=0, vowel_count=0, consonant_count=0, uppercase_count=0, lowercase_count=0, digit_count=0, special_count=0;
    while (true) {
        ch = cin.get();
        if (ch == '\n') break;
        cout << ch; // echo character
        char_count++;
        if (isalpha(ch)) {
            if (isVowel(ch)) vowel_count++; else consonant_count++;
            if (isupper(ch)) uppercase_count++; else lowercase_count++;
        } else if (isdigit(ch)) digit_count++;
        else if (!isspace(ch)) special_count++;
    }
    cout << "\n" << COLOR_YELLOW << "Live input stats:" << COLOR_RESET << endl;
    cout << "Chars: " << char_count << ", Vowels: " << vowel_count << ", Consonants: " << consonant_count << "\n";
    cout << "Uppercase: " << uppercase_count << ", Lowercase: " << lowercase_count << ", Digits: " << digit_count << ", Special: " << special_count << endl;
}

// Analyze the full sentence (main analysis)
map<string,string> analyze(const string& sentence) {
    map<string,string> data;
    int char_count=0, vowel_count=0, consonant_count=0, uppercase_count=0, lowercase_count=0, digit_count=0, special_count=0;

    for (char ch : sentence) {
        char_count++;
        if (isalpha(ch)) {
            if (isVowel(ch)) vowel_count++;
            else consonant_count++;
            if (isupper(ch)) uppercase_count++;
            else lowercase_count++;
        } else if (isdigit(ch)) digit_count++;
        else if (!isspace(ch)) special_count++;
    }
    vector<string> words = splitWords(sentence);
    int word_count = (int)words.size();
    int unique_word_count = countUniqueWords(words);

    data["Sentence"] = sentence;
    data["Total Characters"] = to_string(char_count);
    data["Words"] = to_string(word_count);
    data["Unique Words"] = to_string(unique_word_count);
    data["Vowels"] = to_string(vowel_count);
    data["Consonants"] = to_string(consonant_count);
    data["Uppercase Letters"] = to_string(uppercase_count);
    data["Lowercase Letters"] = to_string(lowercase_count);
    data["Digits"] = to_string(digit_count);
    data["Special Characters"] = to_string(special_count);
    data["Sentence Type"] = sentenceType(sentence);
    data["Length Category"] = lengthCategory(char_count);
    data["Is Palindrome"] = isPalindrome(sentence) ? "Yes" : "No";

    // Percentages
    data["% Vowels"] = to_string(percent(vowel_count,char_count)) + "%";
    data["% Consonants"] = to_string(percent(consonant_count,char_count)) + "%";
    data["% Digits"] = to_string(percent(digit_count,char_count)) + "%";
    data["% Special"] = to_string(percent(special_count,char_count)) + "%";

    return data;
}

// Print analysis in colorized format
void printAnalysis(const map<string,string>& data) {
    cout << COLOR_CYAN << "\n=== Analysis Summary ===" << COLOR_RESET << endl;
    for (const auto& [key,val] : data) {
        cout << COLOR_YELLOW << key << ": " << COLOR_GREEN << val << COLOR_RESET << endl;
    }
}

// Save analysis to text and log files
void saveAnalysis(const map<string,string>& data, const string& filename = "analysis.txt", const string& logname = "log.txt") {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& [key,val] : data) {
            outFile << key << ": " << val << "\n";
        }
        outFile.close();
        cout << COLOR_GREEN << "[Saved analysis to " << filename << "]" << COLOR_RESET << endl;
    } else {
        cout << COLOR_RED << "[Error] Could not write to " << filename << COLOR_RESET << endl;
    }
    ofstream logFile(logname, ios::app);
    if (logFile.is_open()) {
        for (const auto& [key,val] : data) {
            logFile << key << ": " << val << "\n";
        }
        logFile << "-----------------------------\n";
        logFile.close();
    } else {
        cout << COLOR_RED << "[Error] Could not write to " << logname << COLOR_RESET << endl;
    }
}

// Batch mode: analyze all sentences from input.txt (each line one sentence)
void batchMode() {
    ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        cout << COLOR_RED << "[Error] Could not open input.txt for batch processing.\n" << COLOR_RESET;
        return;
    }
    string line;
    int count = 0;
    while (getline(inFile, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto analysis = analyze(line);
        cout << COLOR_CYAN << "\n--- Sentence " << (++count) << " ---" << COLOR_RESET << endl;
        printAnalysis(analysis);
        saveAnalysis(analysis, "analysis.txt", "log.txt");
    }
    inFile.close();
    if(count==0) cout << COLOR_YELLOW << "[input.txt is empty or no valid sentences]" << COLOR_RESET << endl;
}

// Main menu interface
void menu() {
    while (true) {
        cout << COLOR_CYAN << "\n=== Sentence Analyzer v3.0 ===" << COLOR_RESET << endl;
        cout << "1. Analyze a sentence" << endl;
        cout << "2. Live char-by-char input mode" << endl;
        cout << "3. Batch analyze from input.txt" << endl;
        cout << "4. View log history" << endl;
        cout << "5. Export last analysis to JSON" << endl;
        cout << "6. Help / Instructions" << endl;
        cout << "7. Exit" << endl;

        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        static map<string,string> lastAnalysis;

        switch (choice) {
            case 1: {
                cout << "Enter a sentence ending with a period ('.'), question mark ('?'), or exclamation ('!'): ";
                string sentence;
                getline(cin, sentence);
                sentence = trim(sentence);
                if (sentence.empty() || (sentence.back() != '.' && sentence.back() != '?' && sentence.back() != '!')) {
                    cout << COLOR_RED << "[Error] Sentence must end with '.', '?' or '!'." << COLOR_RESET << endl;
                    break;
                }
                lastAnalysis = analyze(sentence);
                printAnalysis(lastAnalysis);
                saveAnalysis(lastAnalysis);
                break;
            }
            case 2:
                liveMode();
                break;
            case 3:
                batchMode();
                break;
            case 4: {
                ifstream logFile("log.txt");
                if (!logFile.is_open()) {
                    cout << COLOR_RED << "[Error] No log.txt file found." << COLOR_RESET << endl;
                    break;
                }
                cout << COLOR_CYAN << "\n=== Log History ===" << COLOR_RESET << endl;
                string line;
                while (getline(logFile, line)) cout << line << endl;
                logFile.close();
                break;
            }
            case 5:
                if (lastAnalysis.empty()) {
                    cout << COLOR_YELLOW << "[No analysis to export. Run an analysis first.]" << COLOR_RESET << endl;
                } else {
                    string filename;
                    cout << "Enter JSON filename to save (default results.json): ";
                    getline(cin, filename);
                    if (filename.empty()) filename = "results.json";
                    saveToJSON(lastAnalysis, filename);
                }
                break;
            case 6:
                cout << COLOR_CYAN << "\nHelp - Usage Instructions:\n" << COLOR_RESET;
                cout << "- Choose option 1 to enter and analyze a sentence.\n";
                cout << "- Option 2: Type characters live, see stats update.\n";
                cout << "- Option 3: Batch analyze multiple sentences in 'input.txt'.\n";
                cout << "- Option 4: View previous analyses from log.\n";
                cout << "- Option 5: Export last analysis to JSON.\n";
                cout << "- Option 7: Exit the program.\n";
                break;
            case 7:
                cout << COLOR_GREEN << "Goodbye!" << COLOR_RESET << endl;
                return;
            default:
                cout << COLOR_RED << "[Invalid choice, try again.]" << COLOR_RESET << endl;
        }
    }
}

#include <iostream>
#include <string>
#include <cctype>

// Suppose you have helper functions like these (simplified):

bool isVowel(char c) {
    c = std::tolower(c);
    return c=='a' || c=='e' || c=='i' || c=='o' || c=='u';
}

bool checkPalindrome(const std::string& s) {
    std::string filtered;
    for (char ch : s) {
        if (std::isalnum(ch)) filtered += std::tolower(ch);
    }
    std::string rev = filtered;
    std::reverse(rev.begin(), rev.end());
    return filtered == rev;
}

std::string classifySentenceType(const std::string& sentence) {
    if (!sentence.empty()) {
        char lastChar = sentence.back();
        if (lastChar == '?') return "Question";
        if (lastChar == '!') return "Exclamation";
    }
    return "Statement";
}

int main() {
    std::string sentence;
    std::cout << "Enter a sentence ending with a period:\n";
    std::getline(std::cin, sentence);

    int length = sentence.length();
    int words = 0, vowels = 0, consonants = 0, digits = 0, specialChars = 0;
    bool inWord = false;

    for (char ch : sentence) {
        if (std::isspace(ch)) {
            if (inWord) {
                words++;
                inWord = false;
            }
        } else {
            inWord = true;
        }

        if (std::isalpha(ch)) {
            if (isVowel(ch)) vowels++;
            else consonants++;
        } else if (std::isdigit(ch)) {
            digits++;
        } else if (!std::isspace(ch)) {
            specialChars++;
        }
    }
    if (inWord) words++;

    bool isPal = checkPalindrome(sentence);
    std::string sentenceType = classifySentenceType(sentence);

    writeAnalysisToFile(sentence, length, words, vowels, consonants, digits, specialChars, sentenceType, isPal);

    std::cout << "Analysis saved to analysis.txt\n";
    return 0;
}

#include <fstream>
#include <string>

void exportAnalysisToJSON(const std::string& sentence, int length, int words, int vowels,
                          int consonants, int digits, int specialChars,
                          const std::string& sentenceType, bool isPalindrome,
                          const std::string& filename = "results.json") {
    std::ofstream jsonFile(filename);
    if (!jsonFile) {
        std::cerr << "Error opening " << filename << " for writing.\n";
        return;
    }

    jsonFile << "{\n";
    jsonFile << "  \"sentence\": \"" << sentence << "\",\n";
    jsonFile << "  \"length\": " << length << ",\n";
    jsonFile << "  \"words\": " << words << ",\n";
    jsonFile << "  \"vowels\": " << vowels << ",\n";
    jsonFile << "  \"consonants\": " << consonants << ",\n";
    jsonFile << "  \"digits\": " << digits << ",\n";
    jsonFile << "  \"special_characters\": " << specialChars << ",\n";
    jsonFile << "  \"sentence_type\": \"" << sentenceType << "\",\n";
    jsonFile << "  \"palindrome\": " << (isPalindrome ? "true" : "false") << "\n";
    jsonFile << "}\n";

    jsonFile.close();
    std::cout << "Analysis exported to " << filename << "\n";
}

exportAnalysisToJSON(sentence, length, words, vowels, consonants, digits, specialChars, sentenceType, isPal);

std::string jsonFilename;
std::cout << "result.json";
std::getline(std::cin, jsonFilename);

exportAnalysisToJSON(sentence, length, words, vowels, consonants, digits, specialChars, sentenceType, isPal, jsonFilename);

char exportChoice;
std::cout << "Would you like to export this analysis as JSON? (y/n): ";
std::cin >> exportChoice;
std::cin.ignore();  // Clear newline

if (exportChoice == 'y' || exportChoice == 'Y') {
    std::string filename;
    std::cout << "Enter filename to save JSON (e.g., results.json): ";
    std::getline(std::cin, filename);

    exportAnalysisToJSON(sentence, length, words, vowels, consonants,
                         digits, specialChars, sentenceType, isPal, filename);
}


int main() {
    menu();
    return 0;
}
