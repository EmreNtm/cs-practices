Finds whether a word from the given list can be transformed into another word from the list by navigating through neighboring words that differ by only 1 letter.
Algorithm:
- The world list is taken into a graph
- The connection matrix is built from the graph
- Breadth-first-search is used to find if the given 2 words have a path
- Transition steps are calculated by backtracking breadth-first-search results