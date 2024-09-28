# clouseau

Document repository search engine written in C++.

## Document repository retrieval

Using the Project Gutenberg library you can use `download_gutenberg.py` to download (web scrape) the top 100 books from Project Gutenberg and store them in a subdirectory of the project root `archive` as plain text files.

```bash
pip install -r requirements.txt

python download_gutenberg.py
```

## Build

```bash
mkdir build
cd build

cmake ..
make
```

## Run

```bash
./clouseau
```

