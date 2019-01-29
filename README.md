# format-conversion

A list of directories and files of a certain directory in Linux is considered, for example:

|------------------|
|.  
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
./site/ru/random1000 |
|-------------------------|

The program converts this list from one format to another. 

The program is implemented on C++ programming language. 

The program solves an examination task to Yandex company.

## Input Format

The first two lines contain the names of the input and output formats. Both strings from the set "find", "python", "acm1", "acm2", "acm3", "xml". In the subsequent lines in the form corresponding to the input format from the first line, the directory tree is described. Our task is to output this directory tree in the output format to the output file.

## Output Format

Print a list of files in the output format to the output file.

## Example of run

| Input | Output | 
|-------|--------|
| find  
python  
2  
site 0  
site/news 12 | 
2  
site 0  
news 12 |


## Compilation

c++ pimanov.cpp

## Author 

Vladimir Pimanov (pimanov-vladimir@yandex.ru)


