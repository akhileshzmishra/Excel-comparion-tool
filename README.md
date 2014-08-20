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
|1  |B Tree based storage tree | For fast retrieval, deletion and insertion and importantly preservation of sequence as that happens in linked list.
|2  |Linked list based upon observer pattern| Even if linked list is getting manipulated, iterator should not become invalid|
|3  |Trie | For finding out closest matched files in the folder |
|4  |Map duals | creating map from and to view and database|




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

_Data structure_ used for table has been upgraded to modified B tree now:

|Sn | Description |
|---|-------------|
|1. | _Insertion takes at  most log n time_.|
|2. |_Lookup takes at most log n time_|
|3. |_Deletion takes at most log n time_|
|4. |_After any insertion/deletion, the index rearranges itself so that index is consecutive_.|

As an example
There are 10 elements indexed from 1 - to - 10.


|-----|
|1(A) 2(B) 3(C) 4(D) 5(E) 6(F) 7(G) 8(H) 9(I) 10(J)|
|-----|
After an insertion at 4th place the elements should be
|-----|
|1(A) 2(B) 3(C) 4(Insert) 5(D) 6(E) 7(F) 8(G) 9(H) 10(I) 11(J)|
|-----|

This can be achieved by a list but every other operation would be terribly slow. With any combination of primitive data structure available in stl, it was not possible to have a fast look up, fast insertion/ deletion while still preserving index.

_My concept is like this_
Store every thing in a leaf node. 
When leaf node is full, bifurcate it and create a new parent element. Add the bifurcated parts to the parent node.
Now parent here is not a leaf node but its children are. So there are two types of storage capacity
Storage capacity of the leaf(_actual data also called Pocket Size in the code_)
Storage capacity of every other node.(_also called Order in the code_)
When there is any insertion, keep the data in the appropriate storage leaf node. if it is full, bifurcate it and 
rearrange the whole node tree.


_Finally_
-----------

In coming releases, I would add side bar, tool tip among other things to make this application more professional.

Users are free to use/modify/delete code as their need. In case you are using our code, please mention our names/emails in your application about dialog/main page. We have tried to test it on several test cases. In case you find any bug, please mail it to us at akhileshzmishra@gmail.com along with excel sheets that you were using (if possible).

