# Shogi with Linked list in C
Assignment 2

## Usage

## Compile & Run
```sh
# Compile
cd shogi_withLL
gcc shogi_withLL.c
# Run
To create new game:
./a.out -n filename.txt
To load a old game:
./a.out -l filename.txt
```

## -n(new game)

### Blue goes first

### Blue's move
Enter the coordinates of the piece you want to move.  
Enter the place you want to put the piece onto.

```sh
# Blue
[Blue] Please enter the piece you want to move:
7 7
[Blue] Please enter the place you want to put the piece onto:
7 6
```

### Red's move

Enter the coordinates of the piece you want to move.  
Enter the place you want to put the piece onto.

```sh
# Red
[Red] Please enter the piece you want to move:
7 3
[Red] Please enter the place you want to put the piece onto:
7 4
```

### Enter 0 to undo
```sh
# Undo
[RED] Please enter the piece you want to move:
0
```

## -l(load old game)

### Enter 'f' or 'b' to load next round or previous round.
```sh
#Enter input to test
Enter 'f' to go to the next round.
Enter 'b' to go to the previous round.
Enter 'x' to close the file.
```


