These solutions are built on the given skeleton with an implementation for the function cacheSimDM().
It implemented by detecting index and tag parts in the given address based on cache size and line size.
Simply, it detect if the tag in the given index is valid and with the same tag or not.
If the tag is valid and the tag is the same it will return “HIT” else it will replace the old tag with
the requested one (as we get it from main memory), and return a "MISS".

For experiment 1 (exp1.cpp) cache size is fixed at 64 kbytes and the line size is changed manually for each size.
For experiment 2 (exp2.cpp) line size is fixed at 16 bytes and the cache size is changed manually for each size.

Each time we run an experiment it will display the result for all address generators from 1 to 6. (Hit ratio, miss ratio, 
number of hits, & number of misses)
Results for each address line is skipped because of time consumed in printing a million line for each experiment.

It starts for each generator by initializing cache table with zeros then start generate addresses and calculate the result at the end. 
For experiment 2, the same code is used but each time cache size is changed manully.