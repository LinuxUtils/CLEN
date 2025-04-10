![CLEN BANNER](https://github.com/g7gg/CLEN/blob/main/graphics/CLEN_BANNER.png?raw=true)

# What is CLEN?
#### CLEN is a high-performance command-line tool that analyzes and reports details about each argument you pass to it. It can count the total length, letters, numbers, sentences, and special symbols in each input, and also detect and report file content lengths when given a valid path. Designed for speed, CLEN uses low-level memory operations to efficiently process even very long inputs in real-time, making it ideal for developers and power users who need fast text inspection from the terminal.

# Installation
### 1. Clone repository
`git clone https://github.com/g7gg/CLEN.git`

### 2. Build
`cd CLEN && gcc -O3 -o clen clen.c`

### 2. Move to bins
`install -m 755 clen /usr/local/bin/clen`
