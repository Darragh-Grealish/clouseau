# clouseau

[![C++ Build](https://github.com/Darragh-Grealish/clouseau/actions/workflows/build.yml/badge.svg)](https://github.com/Darragh-Grealish/clouseau/actions/workflows/build.yml)

Document repository search engine written in C++.
Using CMake, ctest, C++11 and Project Gutenberg.

## Document repository retrieval

Using the Project Gutenberg library you can use `download_gutenberg.py` to download (web scrape) the top 100 books from Project Gutenberg and store them in a subdirectory of the project root `archive` as plain text files.

```bash
pip install -r requirements.txt

python download_gutenberg.py
```

## Getting Started

```bash
git submodule update --init --recursive
```

## Usage

```bash
mkdir -p build
cd build

# Build
cmake ..
make #Windows: cmake --build .

# Test
make test

# Run
./clouseau index ../archive
./clouseau search ../archive/index.csv

```

