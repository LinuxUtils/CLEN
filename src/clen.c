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


/*
 * Fast strlen implementation using 64-bit memory chunks.
 * It detects null terminators using bitwise operations and
 * is significantly faster than standard strlen for large inputs.
 */
size_t fastStrLen(const char *str) {
    const char *start = str;

    while ((uintptr_t)str % sizeof(uint64_t)) {
        if (*str == '\0') return str - start;
        str++;
    }

    const uint64_t *wordPtr = (const uint64_t *)str;
    while (1) {
        uint64_t chunk = *wordPtr++;
        if ((chunk - 0x0101010101010101ULL) & ~chunk & 0x8080808080808080ULL) {
            str = (const char *)(wordPtr - 1);
            while (*str) str++;
            return str - start;
        }
    }
}



/*
 * Checks whether the given string is a valid file path.
 * Uses access() with F_OK to check if the file exists.
 */
int isFilePath(const char *path) {
    return access(path, F_OK) == 0;
}



/*
 * Gets the total size of the contents of a file in bytes.
 * Opens the file in binary mode and seeks to the end to get size.
 */
size_t getFileContentLength(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fclose(file);
    return size;
}



/*
 * Counts how many letter characters are present in a string.
 * Only standard A-Z and a-z are counted using isalpha().
 */
int countLetters(const char *str) {
    int count = 0;
    while (*str) if (isalpha(*str++)) count++;
    return count;
}



/*
 * Counts how many numeric digit characters (0-9) are present in a string.
 */
int countNumbers(const char *str) {
    int count = 0;
    while (*str) if (isdigit(*str++)) count++;
    return count;
}



/*
 * Counts how many sentences are in a string.
 * A sentence is considered to end with a period ('.').
 */
int countSentences(const char *str) {
    int count = 0;
    while (*str) if (*str++ == '.') count++;
    return count;
}



/*
 * Counts how many special characters are in the string.
 * Only predefined ASCII symbols like !@#$%^ etc. are checked.
 */
int countSpecialSigns(const char *str) {
    int count = 0;
    const char *special = "!@#$%^&*()-_=+[]{}|;:'\",.<>?/\\~`";
    while (*str) {
        if (strchr(special, *str)) count++;
        str++;
    }
    return count;
}



/*
 * Prints a help message showing all available command-line options.
 * Called when --help is passed or no valid input is provided.
 */
void showHelp() {
    printf("© 2025 CLEN - By My Name\n\n");
    printf("CLEN is a high-performance command-line tool that analyzes and reports details\nabout each argument you pass to it. It can count the total length, letters, numbers,\nsentences, and special symbols in each input, and also detect and report file content\nlengths when given a valid path. Designed for speed, CLEN uses low-level memory\noperations to efficiently process even very long inputs in real-time, making it ideal\nfor developers and power users who need fast text inspection from the terminal.\n\n");
    printf("Usage: ./clen [options] arguments...\n\n");
    printf("Options:\n");
    printf("  --count-filecontent    Count length of file content if argument is a file\n");
    printf("  --count-sentences      Count number of '.' sentence endings in argument\n");
    printf("  --count-numbers        Count numerical digits (0–9) in argument\n");
    printf("  --count-letters        Count A-Z and a-z letters in argument\n");
    printf("  --count-special-signs  Count special characters like !@#$%%^&*\n");
    printf("  --help                 Show this help message\n\n");
}



int main(int argc, char *argv[]) {
    if (argc < 2) {
        showHelp();
        return 0;
    }

    // Option flags
    int countLettersFlag = 0;
    int countNumbersFlag = 0;
    int countSentencesFlag = 0;
    int countSpecialFlag = 0;
    int countFileContentFlag = 0;

    int firstArgIndex = 1;

    // Parse options
    for (; firstArgIndex < argc; firstArgIndex++) {
        const char *arg = argv[firstArgIndex];
        if (strncmp(arg, "--", 2) != 0) break;

        if (strcmp(arg, "--count-letters") == 0) countLettersFlag = 1;
        else if (strcmp(arg, "--count-numbers") == 0) countNumbersFlag = 1;
        else if (strcmp(arg, "--count-sentences") == 0) countSentencesFlag = 1;
        else if (strcmp(arg, "--count-special-signs") == 0) countSpecialFlag = 1;
        else if (strcmp(arg, "--count-filecontent") == 0) countFileContentFlag = 1;
        else if (strcmp(arg, "--help") == 0 || strcmp(arg, "--h") == 0) {
            showHelp();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", arg);
            return 1;
        }
    }

    // Process each argument after options
    for (int i = firstArgIndex; i < argc; i++) {
        const char *arg = argv[i];
        size_t length;

        int isFile = isFilePath(arg);

        // Decide length source
        if (isFile && countFileContentFlag) {
            length = getFileContentLength(arg);
        } else {
            length = fastStrLen(arg);
        }

        // Create preview
        char preview[12];
        if (fastStrLen(arg) > 8) {
            snprintf(preview, sizeof(preview), "%.8s...", arg);
        } else {
            snprintf(preview, sizeof(preview), "%s", arg);
        }

        // Output length and preview
        printf("%zu -> %s%s\n", length, preview, isFile && countFileContentFlag ? " (file)" : "");

        // Optional counts
        if (countLettersFlag)
            printf("    - %d Letters\n", countLetters(arg));
        if (countNumbersFlag)
            printf("    - %d Numbers\n", countNumbers(arg));
        if (countSentencesFlag)
            printf("    - %d Sentences\n", countSentences(arg));
        if (countSpecialFlag)
            printf("    - %d Special Signs\n", countSpecialSigns(arg));

        fflush(stdout);
    }

    return 0;
}
