# DISCLAIMER
This repo a school project for <strong> cse13s </strong> and all current students should not look at the source code.

# Huffman Coding
The decode and encode programs aim to decode and/or encode any file type, ideally to reduce the size of the file. The
encode program will work for any file, but the decode program will only work with files that were encoded with a
specific magic number.

Note that if the '-v' flag is specified for either programs, the program will output the umcompressed and compressed
file sizes and the amount of space saved.

## Citations
The structs for the header, node adt, Code adt and stack adt are given by Professor Long in the assignment
documentation and are used in this assignment. Additionally, all the functions are built off of the ideas given by the
assignment documentation with some aspects given from sections.

## Building 

### Decode:

To build the decode program, you can run the command:
```
$ make decode
```
Or 
```
$ make 
```
\
Although there are other methods present in the Makefile that will allow to build the program, shown below, but the other methods are more intuitive.
```
$ make all 
```

### Encode:

To build the encode program, you can run the command:
```
$ make encode
```
Or 
```
$ make 
```
\
Although there are other methods present in the Makefile that will allow to build the program, shown below, but the other methods are more intuitive.
```
$ make all 
```


## Running

### Decode:

To run the decode program after compiling it, you can run the command:
```
$ ./decode -<flag(s)>
```
OR
```
$ ./decode -<flag> -<flag> ...
```
with any (can be multiple) flag (comand-line argument) below:


|Flag                  |Output                                                                                      | 
|:--------------------:| ------------------------------------------------------------------------------------------ |
|-v                    |Prints statistics that represent the space saved, the decoded file size, and the encoded file size.   |
|-i <em> infile </em>  |Specifies an input file path representing the file to decode. Default: stdin  |
|-o <em> outfile </em> |Specifies an outfile file path to print the decoded file to. Default: stdout                      |
|-h                    |Displays a help message detailing how to use the program.                                   |

Note that to enter an argument for the flags that require arguments, you can simply add the argument after the flag with
a space in between, as shown below:
```
$ ./decode -o ~/path/to/outfile
```


### Encode:

To run the encode program after compiling it, you can run the command:
```
$ ./encode -<flag(s)>
```
OR
```
$ ./encode -<flag> -<flag> ...
```
with any (can be multiple) flag (comand-line argument) below:


|Flag                  |Output                                                                                      | 
|:--------------------:| ------------------------------------------------------------------------------------------ |
|-v                    |Prints statistics that represent the space saved, the original file size, and the encoded file size.   |
|-i <em> infile </em>  |Specifies an input file path representing the file to encode. Default: stdin  |
|-o <em> outfile </em> |Specifies an outfile file path to print the encoded file to. Default: stdout                      |
|-h                    |Displays a help message detailing how to use the program.                                   |

Note that to enter an argument for the flags that require arguments, you can simply add the argument after the flag with
a space in between, as shown below:
```
$ ./encode -o ~/path/to/outfile
```

### Example:
Running the command below after building the program will encode and \
decode a text file.
```
$ cat file.txt
$ 
$ ./encode -i file.txt -o output
$ cat output
$
$ ./decode -i output
$
```
\
Result from the example above:
```
> Hello World!!!
>
> ?ﾤ?#L!LoLeLHIIILrL
ILL?IILdL LWIILlIIIn??V|?
>
> Hello World!!!
```
