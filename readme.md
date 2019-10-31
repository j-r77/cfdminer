# Mining Constant Conditional Functional Dependencies

> Based on the paper "Discovering Conditional Functional Dependencies", by W. Fan, F. Geerts, J. Li, and M. Xiong , published in IEEE Transactions on Knowledge and Data Engineering. The original code of the paper is not freely available, this version is my understanding of the CFDMiner algorithm for discovering constant CFDs, as explained in the paper.

## Quick Start Guide
The algorithm takes 3 mandatory arguments as input:
1. A dataset in csv format
2. The minimum support threshold, specifying how often the discovered rules should occur in the data
3. The maximum size of the discovered rules, i.e., how many attributes are allowed to occur in any rule

Note that the algorithm only discovers rules without violations


## Output format
The rules are output in the notation described in the paper: First, the attributes are listed as [A,B] => C, followed by the values those attributes take in the rule, (a,b || c). This notation corresponds to a rule (A=a, B=b) => C=c. All rules contain a single consequent.