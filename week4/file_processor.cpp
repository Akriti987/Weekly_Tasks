#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <regex>
#include <sstream>
#include <vector>
#include <mutex>

std::atomic<int> word_count(0);        
std::atomic<int> punctuation_count(0);  
std::atomic<int> date_count(0);         
std::mutex mtx;                        

std::regex date_pattern(R"(\b\d{4}-\d{2}-\d{2}\b)");

// Function to check if a word is a date
bool isDate(const std::string &word) {
    return std::regex_match(word, date_pattern);
}

void countWords(const std::string &line) {
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        if(ispunct(word.at(word.length()-1)))
            word = word.substr(0,word.length()-1);
        if (!isDate(word)) {  // Only count words that are not dates
            word_count++;  // Increment word count for each word found
            //std::cout<<word<<" "<<word_count<<std::endl;
        }
    }
}

// Function to count punctuation marks in a line
void countPunctuation(const std::string &line) {
    //std::cout<<"Punctuation:\n"<<std::endl;
    for (char c : line) {
        if (ispunct(c)) {  
            //std::cout<<c<<std::endl;
            punctuation_count++; 
        }
    }
}

void countDates(const std::string &line) {
    std::smatch matches;  
    std::string::const_iterator search_start(line.cbegin());
    
    // Iterate over the line and match all date patterns
    while (std::regex_search(search_start, line.cend(), matches, date_pattern)) {
        date_count++;  
        search_start = matches.suffix().first;  
    }
}

void processFile(const std::string &filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        throw std::runtime_error("Error: Failed to open input file.");
    }

    std::string line;
    std::vector<std::thread> threads;  // Vector to store threads for each line

    while (std::getline(input_file, line)) {
        // For each line, launch a separate thread for each task
        threads.push_back(std::thread(countWords, std::ref(line)));
        threads.push_back(std::thread(countPunctuation, std::ref(line)));
        threads.push_back(std::thread(countDates, std::ref(line)));
    }

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}


void writeResults(const std::string &output_filename) {
    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        throw std::runtime_error("Error: Failed to open output file.");
    }

    // Write the results to the file
    output_file << "Word Count: " << word_count.load() << std::endl;
    output_file << "Punctuation Count: " << punctuation_count.load() << std::endl;
    output_file << "Date Pattern Count: " << date_count.load() << std::endl;

    output_file.close(); 
}

int main() {
    std::string input_filename, output_filename;
    input_filename="input.txt";
    output_filename = "output.txt";

    try {
        processFile(input_filename);
        writeResults(output_filename);
        std::cout << "Processing completed successfully. Results written to " << output_filename << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

