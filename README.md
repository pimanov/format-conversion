# format-conversion

A list of directories and files of a certain Linux directory is considered. For example:


    .
    ./download_client.sh
    ./random1000_queries_sport.txt
    ./times.txt
    ./site
    ./site/site_kz_domains_random1000_2011-07-26.txt
    ./site/site_ru_domains_top1000_2011-07-27.txt
    ./site/site_by_domains_top1000_2011-07-27.txt
    ./site/kz
    ./site/kz/random1000
    ./site/kz/random1000/site_kz_random1000_2011-07-16.xml
    ./site/ru
    ./site/ru/random1000


The program converts this list from one format to another. It is implemented on C++.


## Input Format

The first two lines contain the names of the input and output formats. Both strings from the set "find", "python", "acm1", "acm2", "acm3", "xml". The directory tree is described in the subsequent lines. The form of directory tree corresponds to the input format from the first line. The goal is to return the directory tree to output file in the output format.

## Output Format

Output file with the list of files in the output format. 

## Example of run

| Input | Output | 
|-------|--------|
| find<br/> python<br/> 2<br/> site 0<br/> site/news 12 | 2<br/> site 0<br/> news 12 |


## Compilation

c++ pimanov.cpp

## Author 

Vladimir Pimanov (pimanov-vladimir@yandex.ru)


