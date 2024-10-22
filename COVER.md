## Clouseau - Group 2
1. Group Members:
    - Adam Byrne 22338004
    - Darragh Grealish 22347666
    - Desirèe Charles 21188076

## Contributions 
1. Adam Byrne
    - Data Structure: ArrayList & HashMap
    - Test: ArrayList, CLI, Indexer
    - Document Indexing
    - CLI
    - Build Setup 

2. Darragh Grealish
    - Data Structure: HashMap & Trie
    - Test: HashMaps & Trie
    - Keyword Autocomplete 
    

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

Throughout the development of our project, several key decisions and trade-offs were made in alignment with the project specifications. First, the selection of a reliable book/document database was essential for ensuring both functionality and compliance. To address the potential risks of piracy and copyright violations, we chose to utilize the Project Gutenberg library, which offers a vast collection of public domain works. This decision ensured our project remained compliant with legal standards while still providing a sufficient volume of text data for the search engine.

We implemented a 'download_gutenberg.py' script, located at the project root, that automates the retrieval of the top 100 books from Project Gutenberg. The script is compatible with pip for easy environment setup using the requirements.txt file. The downloaded books are stored in the archive subdirectory as plain text files, making them accessible for indexing.

The indexing process is managed by the index command, which scans all documents in the archive directory and stores the index in a CSV file, also within the archive directory. The index is structured with rows that begin with a word, followed by pairs of file names and their respective word counts (e.g., word, file1 count1, file2 count2, ..., fileN countN).

In terms of testing and data structures, several important design choices were made:

- GoogleTest Framework: We incorporated the GoogleTest framework for streamlined and efficient management of unit tests, ensuring robust verification of functionality.

- ArrayList: Instead of using the standard std::vector, we developed a custom ArrayList, which resizes by doubling the array size when full. This gave us more control over memory management.

- HashMap: Our custom HashMap was built using open addressing with double hashing for collision resolution, along with lazy deletion. The array automatically rehashes and doubles in size when the load factor exceeds 0.75, improving performance during high-volume operations.

- Set: To handle collections of unique elements effectively, we built a custom Set structure. This ensures no duplicates and provides critical features like intersections, insertions, and existence checks, optimizing operations that involve handling unique data sets.

Each of these choices was made to optimize performance, ensure scalability, and improve overall efficiency while staying true to the project’s requirements.

## References