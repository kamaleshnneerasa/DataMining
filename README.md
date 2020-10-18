# DataMining
Assignment-1

# How to compile?
./compile.sh

# How to run?
./2017CS10341.sh input X -apriori output
./2017CS10341.sh output X -fptree output  
./2017CS10341.sh input -plot

# List of files submitted
1. fptree.cpp : The source code of FP-Tree Algorithm
2. apriori.cpp : The source code of Apriori Algorithm
3. plot.py : Script to generate the running time plots
4. compile.sh : Commands to compile source code
5. 2017CS10341.sh : Executing the code

# Observations
We observed that the runtime of FP-Tree algorithm at lower threshold values is very better than that of Apriori algorithm. <br />
We also observed that FP-Tree scales well with the size of dataset where as Apriori doesnot scale well with the dataset size. <br />
This is because of two main reasons:
1) Apriori algorithm requires multiple database scans(size of the largest frequent itemset) while FP-Tree algorithm requires only two database scans. <br />
2) Apriori first generates the candidate sets and then prunes the sets(bigger memory requirement) while FP-tree encodes the sets in a tree structure. <br />

Names:
Kamalesh Neerasa 2017CS10341
2019MCS2792
