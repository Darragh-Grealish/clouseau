# Decisions

Tracking choices or trade-offs made during the development of the project with reference to the spec.

## Source material

We need a database of book/documents to search through. To avoid piracy/copyright issues, we will use the Project Gutenberg library. This library contains a large number of books that are in the public domain.
There is a `download_guteberg.py` script in the project root whose environment can be set up with `pip` via the `requirements.txt` file. It scrapes the top 100 books from Project Gutenberg and stores them in a subdirectory of the project root `archive` as plain text files.

## Third party dependencies

- GoogleTest - Using the google test framework for easier test management

## Data Structures

1. ArrayList - our implementation of vector using a fixed array that resizes once full to 2x.
2. HashMap - implemented using Open Adressing, using Double Hashing for collision prevention and Lazy Deletion. Array is rehashed & size is doubled when loadfactor > 0.75.
