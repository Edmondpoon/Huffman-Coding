# Disclaimer
This repo contains a school project for <strong> cse13s </strong> and all current students should not look at the source code.

# Huffman Coding
The decode and encode programs aim to decode and/or encode any file type, ideally to reduce the size of the file. The
encode program will work for any file, but the decode program will only work with files that were encoded with a
specific magic number.

Note that if the '-v' flag is specified for either programs, the program will output the umcompressed and compressed
file sizes and the amount of space saved.

## Building 

Both programs (encode/decode) can be built at once via either commands below:
```
$ make all
```
Or 
```
$ make 
```

To build a specific program, you can simply run
```
$ make <encode/decode>
```


## Running

To run any of the two executables after compiling them, you can run the command:
```
$ ./<decode/encode> [-flag(s)]
```
OR
```
$ ./<decode/encode> [-flag] [-flag] ...
```
with any (can be multiple) flag (comand-line argument). All valid flags can be found using the '-h' flag with any executable from above.

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
