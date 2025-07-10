# Sentence Analyzer v3.0 - Advanced C++ Tool

## Overview

Sentence Analyzer is a C++ console application that reads sentences character by character, analyzing their structure and content. It offers powerful features including word counting, vowel counting, palindrome detection, batch processing, and live input mode. Results can be saved as text or JSON files.

---

## Features

- **Character-level analysis:** Counts total characters, words, vowels, consonants, digits, and special characters.
- **Unique word counting:** Detects how many unique words appear in the sentence.
- **Sentence classification:** Identifies if the sentence is a statement, question, or exclamation.
- **Palindrome detection:** Checks if the sentence reads the same forward and backward (ignoring punctuation and case).
- **Sentence length categorization:** Classifies sentences as short, medium, or long.
- **Percentage stats:** Shows the percentage of vowels, digits, consonants, and special characters relative to the total length.
- **Live input mode:** Displays analysis stats while typing character-by-character.
- **Batch processing:** Reads multiple sentences from `input.txt` and analyzes them sequentially.
- **Logging:** Stores all analysis results with timestamps in `log.txt`.
- **Export to JSON:** Save the latest analysis results in JSON format with a custom filename.
- **Color-coded terminal output:** Uses colored text for better readability (supports cross-platform detection).
- **Cross-platform compatibility:** Works on Windows, macOS, and Linux terminals.
- **Unit testing:** Integrated tests using Catch2 framework to ensure reliability.
- **CI/CD pipeline:** GitHub Actions workflow for automatic build and test on push and pull requests.

---

## Getting Started

### Prerequisites

- C++ compiler (g++ recommended)
- (Optional) Qt framework for GUI version

### Building the Project

Compile the main program:

```bash
g++ main.cpp -o analyzer

