# Unix Utilities Implementation

This repository contains simple implementations of Unix utilities in C:
- `pwd`: Prints the current working directory.
- `echo`: Prints a user input string to standard output.
- `cp`: Copies a file from a source to a destination.
- `mv`: Moves a file from one location to another.

## Compilation
To compile the programs, use the following commands:
```sh
gcc -o pwd pwd.c
gcc -o echo echo.c
gcc -o cp cp.c
gcc -o mv mv.c
```

## Usage and Example Output
### 1. `pwd` - Print Working Directory
```sh
$ ./pwd
/home/user
```
Prints the current directory.

### 2. `echo` - Print Input String
```sh
$ ./echo Hello World
Hello World
```
Prints the provided string.

### 3. `cp` - Copy File
```sh
$ ./cp source.txt destination.txt
$ cat destination.txt
(Contents of source.txt displayed)
```
Copies `source.txt` to `destination.txt`.

### 4. `mv` - Move File
```sh
$ ./mv oldname.txt newname.txt
$ ls | grep newname.txt
newname.txt
```
Moves (or renames) `oldname.txt` to `newname.txt`.

## Error Handling
Each program checks for errors such as missing arguments, invalid file paths, or system call failures, and prints appropriate error messages.

## Repository Structure
```
/Linux
│── pwd.c
│── echo.c
│── cp.c
│── mv.c
│── README.md
```

## Contribution
Feel free to fork this repository and improve the utilities!

## License
This project is open-source and available under the MIT License.
