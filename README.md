# DataMining
Assignment-1

# How to run fptree.cpp?
Compilation: g++-7 -Wall -O3 -std=c++11 fptree.cpp -o fptree <br />
Running: ./fptree threshold inputFile outputFile <br />
Example: ./fptree 20 transact.txt result.txt <br />

# How to run apriori.cpp?
Compilation: g++-7 -Wall -O3 -std=c++11 apriori.cpp -o apriori <br />
Running: ./apriori threshold inputFile outputFile <br />
Example: ./apriori 20 transact.txt result.txt <br />

# Observations
We observed that the runtime of FP-Tree algorithm at lower threshold values is very better than that of Apriori algorithm. <br />
We also observed that FP-Tree scales well with the size of dataset where as Apriori doesnot scale well with the dataset size. <br />
This is because of two main reasons:
1) Apriori algorithm requires multiple database scans(size of the largest frequent itemset) while FP-Tree algorithm requires only two database scans. <br />
2) Apriori first generates the candidate sets and then prunes the sets(bigger memory requirement) while FP-tree encodes the sets in a tree structure. <br />
