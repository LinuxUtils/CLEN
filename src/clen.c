/*
 * MIT License
 * 
 * Copyright (c) 2025 CLEN - By Ibrahim Yousef Alshaibani
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>





/*
 * This function implements a very fast version of strlen using 64-bit memory access.
 * It first aligns the pointer to an 8-byte boundary and then reads the string in 64-bit
 * chunks, using bitwise operations to quickly detect a null terminator. This technique is
 * significantly faster than the standard character-by-character loop, especially when
 * processing very large strings.
 */
size_t fastStrLen(const char *str) {
    const char *start = str;
    while ((uintptr_t)str % sizeof(uint64_t)) {
        if (*str == '\0')
            return str - start;
        str++;
    }
    const uint64_t *wordPtr = (const uint64_t *)str;
    while (1) {
        uint64_t chunk = *wordPtr++;
        if ((chunk - 0x0101010101010101ULL) & ~chunk & 0x8080808080808080ULL) {
            str = (const char *)(wordPtr - 1);
            while (*str)
                str++;
            return str - start;
        }
    }
}





/*
 * This function checks if the given string represents a valid file path on the system.
 * It uses the POSIX access() function with the F_OK flag to determine if the file or directory exists.
 * This check allows the program to decide whether to process the text itself or, when applicable,
 * to inspect the actual file content.
 */
int isFilePath(const char *path) {
    return access(path, F_OK) == 0;
}





/*
 * This function calculates the total size in bytes of the contents of a file.
 * It opens the file in binary mode, moves the file pointer to the end with fseek(),
 * and retrieves the position using ftell(), which gives the file size. The file is then closed.
 * This offers an efficient method for determining file content size when the argument is a valid file path.
 */
size_t getFileContentLength(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file)
        return 0;
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fclose(file);
    return size;
}





/*
 * This function counts the number of alphabetic letter characters (A-Z and a-z) in a string.
 * It iterates through each character in the string and uses the isalpha() function from ctype.h
 * to determine if the character is a letter, incrementing the count when it is.
 */
int countLetters(const char *str) {
    int count = 0;
    while (*str)
        if (isalpha(*str++))
            count++;
    return count;
}





/*
 * This function counts the number of numeric digit characters (0-9) present in a string.
 * It checks each character using isdigit() and increments a counter for every numeric digit found,
 * allowing for a quick assessment of the numerical content within the argument.
 */
int countNumbers(const char *str) {
    int count = 0;
    while (*str)
        if (isdigit(*str++))
            count++;
    return count;
}





/*
 * This function counts the number of sentences in a string.
 * A sentence is defined as a sequence ending with a period ('.'), question mark ('?'), or exclamation mark ('!').
 * Furthermore, if one of these punctuation marks is immediately followed by a single or double quote,
 * that quote is considered part of the same sentence-ending sequence.
 */
int countSentences(const char *str) {
    int count = 0;
    while (*str) {
        if (*str == '.' || *str == '?' || *str == '!') {
            count++;
            if (*(str + 1) == '\'' || *(str + 1) == '\"')
                str++;
        }
        str++;
    }
    return count;
}





/*
 * This function counts the number of special characters in the string.
 * It uses a predefined list of special symbols (such as !, @, #, $, etc.) and iterates through
 * the string, increasing the counter whenever one of those symbols is found. This provides insight
 * into the non-alphanumeric composition of the text.
 */
int countSpecialSigns(const char *str) {
    int count = 0;
    const char *special = "!@#$%^&*()-_=+[]{}|;:'\",.<>?/\\~`";
    while (*str) {
        if (strchr(special, *str))
            count++;
        str++;
    }
    return count;
}





/*
 * This function counts the number of words in a string.
 * A word is defined as a continuous sequence of non-whitespace characters.
 * The function keeps track of transitions from whitespace to a non-whitespace character,
 * incrementing the word count each time a new word is detected.
 */
int countWords(const char *str) {
    int count = 0, inWord = 0;
    while (*str) {
        if (!isspace(*str)) {
            if (!inWord) {
                count++;
                inWord = 1;
            }
        } else {
            inWord = 0;
        }
        str++;
    }
    return count;
}





/*
 * This function counts the number of quoted substrings in the string.
 * A quoted substring is defined as a segment of text that starts and ends with the same quote character
 * (either a double quote (") or a single quote (')). The function searches for a starting quote and then
 * looks for the corresponding closing quote, counting each complete pair.
 */
int countQuotes(const char *str) {
    int count = 0;
    while (*str) {
        if (*str == '\"' || *str == '\'') {
            char quote = *str;
            const char *temp = str + 1;
            while (*temp && *temp != quote)
                temp++;
            if (*temp == quote) {
                count++;
                str = temp; // Skip to the closing quote
            }
        }
        str++;
    }
    return count;
}




/*
 * This function calculates the number of uppercase and lowercase letters in a given string.
 * It takes a pointer to the input string and two integer pointers for storing the count of uppercase
 * and lowercase characters. The function iterates over each character in the string and uses the 
 * standard C library functions isupper() and islower() to determine the case of alphabetic characters.
 * If a character is uppercase (A–Z), the uppercase counter is incremented; if it's lowercase (a–z),
 * the lowercase counter is incremented. This separation of case types allows users to analyze the
 * casing distribution within their input, which can be particularly useful for checking formatting,
 * analyzing data entry patterns, or enforcing style rules in text input.
 */
void countCases(const char *str, int *upper, int *lower) {
    *upper = 0;
    *lower = 0;
    while (*str) {
        if (isupper((unsigned char)*str)) (*upper)++;
        else if (islower((unsigned char)*str)) (*lower)++;
        str++;
    }
}





/*
 * This function prints a comprehensive help message that explains all the available command-line
 * options of CLEN. It provides a full summary of the tool's functionality, including the newly added
 * options, such as counting bytes and quotes. This help message is essential for new users who need guidance
 * on how to leverage the full features of CLEN.
 */
void showHelp() {
    printf("CLEN is a high-performance command-line tool that analyzes and reports details\n");
    printf("about each argument you pass to it. It can count the total length, letters, numbers,\n");
    printf("sentences, special symbols, words, bytes, and quoted segments in each input, and can also\n");
    printf("detect and report file content lengths when a valid file path is provided. Designed for speed,\n");
    printf("CLEN uses low-level memory operations to efficiently process even very long inputs in\n");
    printf("real-time, making it ideal for developers and power users who need quick and robust\n");
    printf("text inspection from the terminal.\n\n");
    printf("Usage: ./clen [options] arguments...\n\n");
    printf("Options:\n");
    printf("  --count-filecontent    Count the length of file content if the argument is a file\n");
    printf("  --count-sentences      Count sentence endings (., ?, or !, optionally followed by a quote)\n");
    printf("  --count-numbers        Count numerical digits (0–9) in the argument\n");
    printf("  --count-letters        Count alphabetic letters (A–Z and a–z) in the argument\n");
    printf("  --count-cases          Count uppercase and lowercase letters (requires --count-letters)\n");
    printf("  --count-special-signs  Count special characters like !@#$%%^&*\n");
    printf("  --count-words          Count the number of words in the argument\n");
    printf("  --count-bytes          Count the number of bytes in the argument or file content\n");
    printf("  --count-quotes         Count quoted segments delimited by ' or \"\n");
    printf("  --help                 Show this help message\n\n");
}





int main(int argc, char *argv[]) {
    // --> DISPLAY HEADER
    printf("© 2025 CLEN - By Ibrahim Yousef Alshaibani\n\n");



    if (argc < 2) {
        showHelp();
        return 0;
    }



    /*
     * Here we declare flag variables for every command-line option. These flags are set based on
     * the options provided by the user and determine which analyses will be performed on each argument.
     * New flags such as --count-bytes and --count-quotes have been added to extend the tool's capabilities,
     * while existing flags allow for counts of letters, numbers, sentences, special signs, words, and an option
     * to display the argument in uppercase.
     */
    int countLettersFlag     = 0;
    int countCasesFlag       = 0;
    int countNumbersFlag     = 0;
    int countSentencesFlag   = 0;
    int countSpecialFlag     = 0;
    int countFileContentFlag = 0;
    int countWordsFlag       = 0;
    int countBytesFlag       = 0;
    int countQuotesFlag      = 0;
    int firstArgIndex        = 1;



    /*
     * This loop parses the command-line options. It checks each argument that starts with "--" and,
     * if it matches a known option, the corresponding flag is set. The --help option prints this help
     * message and terminates the program. Any unknown options result in an error message. This parsing
     * step allows the tool to be highly flexible and perform only the requested analyses.
     */
    for (; firstArgIndex < argc; firstArgIndex++) {
        const char *arg = argv[firstArgIndex];
        if (strncmp(arg, "--", 2) != 0)
            break;

        if (strcmp(arg, "--count-letters") == 0)
            countLettersFlag = 1;
        else if (strcmp(arg, "--count-cases") == 0)
            countCasesFlag = 1;
        else if (strcmp(arg, "--count-numbers") == 0)
            countNumbersFlag = 1;
        else if (strcmp(arg, "--count-sentences") == 0)
            countSentencesFlag = 1;
        else if (strcmp(arg, "--count-special-signs") == 0)
            countSpecialFlag = 1;
        else if (strcmp(arg, "--count-filecontent") == 0)
            countFileContentFlag = 1;
        else if (strcmp(arg, "--count-words") == 0)
            countWordsFlag = 1;
        else if (strcmp(arg, "--count-bytes") == 0)
            countBytesFlag = 1;
        else if (strcmp(arg, "--count-quotes") == 0)
            countQuotesFlag = 1;
        else if (strcmp(arg, "--help") == 0 || strcmp(arg, "--h") == 0) {
            showHelp();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", arg);
            return 1;
        }
    }



    /*
     * Before processing the individual arguments, we display the total number of non-option arguments.
     * This informs the user how many arguments will be processed, for example, "1 Argument given" for a single
     * argument or "8 Arguments given" if there are multiple.
     */
    int numArgs = argc - firstArgIndex;
    if (numArgs == 1)
        printf("1 Argument given\n\n");
    else
        printf("%d Arguments given\n\n", numArgs);



    /*
     * In this loop, each argument (after the options) is processed one by one.
     * For each argument, we record the processing start time, determine if the argument is a file,
     * and choose to calculate the length either by reading the file content or by using our fast string length method.
     * A short preview (first 8 characters plus "..." if needed) is then generated.
     * After processing, the time taken is computed and displayed alongside the preview.
     */
    for (int i = firstArgIndex; i < argc; i++) {
        const char *arg = argv[i];
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        size_t length;
        int isFile = isFilePath(arg);
        if (isFile && countFileContentFlag)
            length = getFileContentLength(arg);
        else
            length = fastStrLen(arg);
            

        char preview[20];
        if (fastStrLen(arg) > 8)
            snprintf(preview, sizeof(preview), "%.8s...", arg);
        else
            snprintf(preview, sizeof(preview), "%s", arg);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double processTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;


        // --> PRINT THE ARGUMENT INDEX, PREVIEW, AND PROCESSING TIME
        printf("%d -> %s (%.8fs)%s\n",
            i - firstArgIndex + 1,
            preview,
            processTime,
            isFile ? " (File)" : ""
        );
        printf("    - %zu (Length)\n", length);



        /*
         * Now we print additional counts for this argument based on the flags that were set earlier.
         * Each requested metric (letters, numbers, sentences, special signs, words, bytes, quotes, and the uppercase version)
         * is computed and printed on its own indented line.
         */
        if (countLettersFlag)
            printf("    - %d Letters\n", countLetters(arg));
        if (countLettersFlag && countCasesFlag) {
            int upper = 0, lower = 0;
            countCases(arg, &upper, &lower);
            printf("        - %d Uppercase\n", upper);
            printf("        - %d Lowercase\n", lower);
        }
        if (countNumbersFlag)
            printf("    - %d Numbers\n", countNumbers(arg));
        if (countSentencesFlag)
            printf("    - %d Sentences\n", countSentences(arg));
        if (countSpecialFlag)
            printf("    - %d Special Signs\n", countSpecialSigns(arg));
        if (countWordsFlag)
            printf("    - %d Words\n", countWords(arg));
        if (countBytesFlag)
            printf("    - %zu Bytes\n", length);
        if (countQuotesFlag)
            printf("    - %d Quotes\n", countQuotes(arg));

        printf("\n");
        fflush(stdout);
    }

    return 0;

}
