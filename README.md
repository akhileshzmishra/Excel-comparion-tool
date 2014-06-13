Excel-comparion-tool
====================

Excel compare
We have created excel comparison tool here. 

Algorithms Used:
1. LCS for comparing rows
2. Boyer Moore for string search.

Data structure
1. Linked list based upon observer pattern
2. Trie
3. Map duals

Excel compare This tool helps in comparing excel files and merging them. At present it has following capabilites 1. Comparison on base of LCS for rows and exact matching for cols. That implies that cols have to be orderered good matching. Rows can be unordered. What I mean by ordering: a. If rows are ordered that means that rows with unique identity that is same in both files reside at same row number. If cols are ordered that means that cols with same heading reside at same col number. b. In case of unordered, rows with unique identity that is same in both files can reside at any location.

As an example: 

| H1  | H2  | H3  |
-------------------
| a11 | a12 | a13 |
| a21 | a22 | a23 |

_Table 1_

| H1  | H2  |  H3  |
--------------------
| a21 | a22 |  a23 |
| a11 | a12 |  a13 |

_Table 2_

The two rows are at different position relative to each other. So if I choose that _row 1_ of first table is correct relative to _row 2_ then, _row 1_ is at wrong position relative to _row 2_ in second table. This is called row ordering. 

So if rows are not ordererd properly, the tool can find them out and segregate them from rows that have differences

We have tried to give professional look and feel to the application. It has search capabilites presently based upon Boyer Moore string search algorithm.

The tool also has ways of editing/copying/transferring contents from left file to right file.

This tool is build with MFC. I am really sorry about using this old technology. This project started as an intern project in which I acted as a mentor. But with growing demand inside our company, we had to make it more professional by adding various features.

In coming releases, I would add side bar, tool tip among other things to make this application more professional.

Users are free to use/modify/delete code as their need. In case you are using our code, please mention our names/emails in your application about dialog/main page. We have tried to test it on several test cases. In case you find any bug, please mail it to us at akhileshzmishra@gmail.com along with excel sheets that you were using (if possible).

