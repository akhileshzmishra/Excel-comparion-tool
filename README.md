Excel-comparion-tool
====================

We have created excel comparison tool here. This tool compares two tables of different rows and cols and segregates the matched rows from unmatched rows. There are three different views: Difference only view, Same only view and all view

You can also edit the excel sheets and save them as if you are actually working on them.

_How to build_
---------------
There are two projects inside: 
IMergeProject: Actual project
SetupIMerge: Creation of setup files in case you want to redistribute the files.


You should have Visual Studios 2005 Standard edition with MFC installed in your machine. Download the code and find _IMergeProject.sln_ inside _IMergeProject_ folder. Open it and build it. Start without debugging. Please see that your project configuration is in Release mode.

_Performance_
-------------

Let _m_ and _n_ be the number of rows and cols respectively
and _l_ be the length of string

|Sn |Algorithms Used | Complexity | 
|---|----------------|------------|
|1  |LCS for comparing rows| O(_m_*_n_*_n_) |
|2  |Boyer Moore for string search.| O(_l_) [ O(_l_* _m_ *_n_) for searching entire cell fields (_m_ * _n_) |

|Sn |Data structure| Usage  |
|---|--------------|---------|
|1  |Linked list based upon observer pattern| Even if linked list is getting manipulated, iterator should not become invalid|
|2  |Trie | For finding out closest matched files in the folder |
|3  |Map duals | creating map from and to view and database|




_Detailed Discussion_
-----------------------

This tool helps in comparing excel files and merging them. At present it has following capabilites 
1. Comparison on base of LCS for rows and exact matching for cols. That implies that cols have to be orderered good matching. Rows can be unordered. What I mean by ordering: 
2. If rows are ordered that means that rows with unique identity that is same in both files reside at same row number. If cols are ordered that means that cols with same heading reside at same col number. 
3. In case of unordered, rows with unique identity that is same in both files can reside at any location.

As an example: 

| H1  | H2  | H3  |
|-----|-----|-----|
| a11 | a12 | a13 |
| a21 | a22 | a23 |
| a31 | a32 | a33 |

_Table 1_

| H1  | H2  |  H3  |
|-----|-----|------|
| a21 | a22 |  a23 |
| a11 | a12 |  a13 |
| a31 | a32 |  a33 |
 
_Table 2_

The two rows are at different position relative to each other. So if I choose that _row 1_ of first table is correct relative to _row 2_ then, _row 1_ is at wrong position relative to _row 2_ in second table. This is called row ordering. 

_Row 3_ would be marked as same row as its content and order are same in both tables.

So if rows are not ordererd properly, the tool can find them out and segregate them from rows that have differences

We have tried to give professional look and feel to the application. It has search capabilites presently based upon Boyer Moore string search algorithm.

The tool also has ways of editing/copying/transferring contents from left file to right file.

This tool is build with MFC. I am really sorry about using this old technology. This project started as an intern project in which I acted as a mentor. But with growing demand inside our company, we had to make it more professional by adding various features.

_Finally_
-----------

In coming releases, I would add side bar, tool tip among other things to make this application more professional.

Users are free to use/modify/delete code as their need. In case you are using our code, please mention our names/emails in your application about dialog/main page. We have tried to test it on several test cases. In case you find any bug, please mail it to us at akhileshzmishra@gmail.com along with excel sheets that you were using (if possible).

