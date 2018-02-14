# Model for Topic-sensitive Influential Paper Discovery (MTID)
This is a C++ implementation of `MTID` (Model for Topic-sensitive Influential Paper Discovery).   
It is the submission to PAKDD 2018

*********

# Data Format
    This is an example of AAN dataset with 14305 papers
    
    aan.train.topic: 
    
    This is a file representing the topic distribution of each document. We train the document with Latent Dirichlet
    Allocation and the number of topic is set to 10. Each line is the a paper's topic distribution and each column
    (e.g. colunm i) is the membership of papers in topic (i)   

    aan.train.citation:

     line i represents the reference list (document ID) of document vi. The first column is the id of vi and the rest
     are the ids of refered papers.

    aan.train.year:

     line i represents the published year(document ID) of document vi. The first column is the id of vi and the second 
     is the year.
     
# Compiling

Type "make" in a shell.


# Model Learning

Estimate the model by executing:
```Bash
main [number of document] [citation_file] [topic_file] [year_file] [number of topic] [MaxIter] [alpha] [beta] [STARTING_YEAR] [ENDING_YEAR] [output_file]
```  
     alpha and beta are the parameters for back-tracking problem
     
Example:
```Bash
main 14305 ./data/aan.train.citation ./data/aan.train.topic ./data/aan.train.year 10 100 0.8 0.4 1980 2014 ./data/out 
```
