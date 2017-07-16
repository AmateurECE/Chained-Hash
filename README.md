# Chained-Hashtable
<p>This repository contains an implementation of a <i>Chained Hash Table</i> in C. A hash table is a data structure that maps data to elements in an indexable, single dimensional array, by performing a transformation to the data in order to obtain a <i>key</i>. Some hash tables, like in the case of the chained hash table, provide a mechanism for handling <i>collisions</i>, which are edge cases that result from two or more datum being mapped to the same key. In a chained hash table, elements are placed into buckets at each index of the array, which are containers that allow for multiple datum to be placed. In this case, these containers are implemented as a singly linked list.</p>

<p>The goal for this repository is to not only contain the code for implementing a chained hash table, but also a few common hash functions to use with the API. These are not currently included, but will be in a future commit.</p>

<p><b>Building:</b> To build this repository on any system, one follows the traditional Autotools procedure.</p>
<code>
<p>./configure</p>
<p>make</p>
<p>make install</p>
</code>
<p>This repository allows for two different build configurations. The default builds the API assuming statically allocated structures, as in they cannot be resized post-allocation. The repository also supports dynamically-allocated structures, meaning that a size is not specified upon calling chash_init(), and buckets are allocated as needed. This configuration will result in slightly slower operation of the structure, but is useful when the API is employed in environments where efficiency is not key.</p>