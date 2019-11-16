# MLEM2: Rule Induction Implementation

## Project Details

### Usage
The program can be compiled with the command

    make mlem2

and run with the command

    ./mlem2

The program requests an input and output file after it is initialized. The expected format for these files includes any necessary extension, i.e. "test.txt". Please provide different input and output file names.

### Execution Time
For small data sets, results can be expected in less than a few minutes. For larger data sets, allow at least ten minutes for results. The timing of the program's execution, from start to finish, for the given test files have been provided below for reference:
    

| File       | Cases | Attributes | Time      |
|------------|-------|------------|-----------|
| austr.txt  | 690   | 14         | 10m 53s   |
| bowl.txt   | 720   | 4          | -         |
| breast.txt | 277   | 9          | -         |
| iris.txt   | 150   | 4          | 0m 21s    |
| test.txt   | 7     | 3          | 0m 10s    |
| wine.txt   | 178   | 13         | 0m 10s     |

This data was collected on the cycle servers, using the Linux time command.

### Algorithm Implementation
## Ordering
Order influences the execution of the MLEM2 algorithm so it is important to note that my attribute-value blocks are ordered such that all attribute-value blocks for the first attribute (column) are listed, then all of the attribute-value blocks for the second attribute, and so on. Within each attribute, symbolic data is ordered alphabetically and numeric data is ordered by ascending cutpoints to maintain consistency.

## Assignment Overview 

### Logistics
This project was completed for EECS 690 under Dr. Jerzy Grzymala-Busse.

Due: November 20, 2019, 11:59 PM

### Requirements
Implement the MLEM2 algorithm for rule induction. Your project should be implemented on one of the departmental computers, e.g., cycle 1, under Linux. You may use any programming language you wish. Your program should handle symbolic and numerical attributes. All input data sets will be consistent. Potential cutpoints for any numerical attribute should be the averages of consecutive numerical values. Use the all cutpoint approach for numerical attributes.

Input data files will be in the LERS format. Input data files will start from the list of variable declarations. This list starts form "<", then a space, then a sequence of the following symbols: "a", "x", or "d", separated by spaces; then another space, the last symbol of the list is ">". You may ignore this line. The second list of the input data file starts from "["; then a space, then comes a list of attribute and decision names, separated by spaces;
then another space, and then "]". The decision will be always the last variable, all remaining variables are attributes. The following part contains values of the attributes and decisions. The input data file may contain comments. A comment starts from "!"; everything that follows "!", until the end of the line, is the comment. Obviously, comments should be ignored during reading of data. Attribute values are separated by spaces. Spaces should be understood not only as ordinary spaces but also as white space characters, such as the end of line, tab,
etc. Any line of the input data file may start from one or more spaces and one or more spaces may end it. Note that a "line" does not need to be a physical line (it is rather a paragraph). Examples of symbolic values are: medium, 12..14, 1.25..2.37, etc. Examples of numerical values are 42, –12.45, etc.

First your program should ask the user for the name of the input data file. The expected response of the user is the name followed by pressing the <RETURN> key. Then the program should ask for a name of the output data file with rules. Your program should create the output data file with rules, induced by MLEM2, e.g. 

    2, 2, 2
    (Ink-color, blue) & (Body-color, blue) -> (Attitude, plus)
    1, 1, 1
    (Ink-color, red) -> (Attitude, plus)
    1, 3, 3
    (Body-color, black) -> (Attitude, minus)
    1, 3, 3
    (Ink-color, black) -> (Attitude, minus)

Your program should be able to deal with unexpected answers of the user and not crush but rather repeat the question. Use of recursion is not encouraged. You may assume that the input data file does not contain errors. You should expect input data files of any size, with thousands cases and more than a hundred attributes.

### Submission
Include all comments, including instructions about compiling and linking in a single file called read.me. Do not forget to include your name and KUID#. When you are ready to submit the project, send ALL necessary source files, makefile (if any), and the read.me file by e-mail to the TA. Do not send object files, executable files, and test data files. Time stamp of the e-mail will be used to decide late penalty, if applicable. Late projects will be accepted with 10% penalty per day up to five days.