## Clouseau - Group 2
1. Group Members:
    - Adam Byrne 22338004
    - Darragh Grealish 22347666
    - Desirèe Charles 21188076

## Contributions 
1. Adam Byrne

2. Darragh Grealish

3. Desirèe Charles
    - Data Structure: Trie & Set
    - Test: Trie & Set
    - keyword search
    - Display Results

## Number of Git Commits
1. Adam Byrne

2. Darragh Grealish

3. Desirèe Charles

## Line of code
1. Adam Byrne

2. Darragh Grealish

3. Desirèe Charles

## Description 

Tracking choices or trade-offs made during the development of the project with reference to the spec.

We need a database of book/documents to search through. To avoid piracy/copyright issues, we will use the Project Gutenberg library. This library contains a large number of books that are in the public domain.
There is a `download_guteberg.py` script in the project root whose environment can be set up with `pip` via the `requirements.txt` file. It scrapes the top 100 books from Project Gutenberg and stores them in a subdirectory of the project root `archive` as plain text files.

Using the `index` command, the user can index the documents in the `archive` directory. The index is stored in a CSV file in the `archive` directory. The index contains rows of word total file1 count1 file2 count2 ... fileN countN. The word is the first column and the subsequent columns are the file name and the count of the word in that file.

GoogleTest - Using the google test framework for easier test management

ArrayList - our implementation of vector using a fixed array that resizes once full to 2x.

HashMap - implemented using Open Adressing, using Double Hashing for collision prevention and Lazy Deletion. Array is rehashed & size is doubled when loadfactor > 0.75.

Set - We decided to implement a custom Set to efficiently handle collections of unique elements, making sure there are no duplicates. It also offers useful features like intersections, insertions, and checking if an element exists.

## References