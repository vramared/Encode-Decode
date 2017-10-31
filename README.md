# Encode-Decode

Encode/Decode programs which will compress a text file or a PDF to create a condensed version. 
This encoded file can later be decoded for viewing.

# USAGE:

In order to compile the program and create the executables "encode" and "decode", simply type "make"
and these executables will appear in your local directory for usage.

Now, to encode a file, use the following:

$ ./encode -i yourfile.txt

- the -i flag can be used to specify an input file. Type in the name of the file which you would like to encode
- the -o flag can be used to specify an output file. If the output file you specidied does not already exist, a
  new file will be created, and the encryption will be stored here
- the -v flag will print statistics regarding the file you just encoded. This includes things such as the number of 
  bytes which were encoded and the number of bytes outputted, and the ratio of compression
- the -p flag will allow you to print the Hufman Tree which was created as a result of your inputted file

Now that you have encoded a file, decoding is just as simple:

$ ./decode -i yourCompressedFile.zzZ

- -i flag can be used to specify the input file
- -o file can be used to specify the output file to write to
- -v will print some advanced statistics
- -p will print the huffman tree which was used to build the original file

Thank you for using Encode/Decode.


