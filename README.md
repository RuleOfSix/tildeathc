Tildeathc is a compiler for my version of the ~ATH programming language, written in C. It compiles to assembly language targeting amd64 Linux, and uses GCC to assemble and link the asm into a native executable binary.

# What is ~ATH?
~ATH is a fictional esoteric programming language from the Homestuck webcomic. In its original form, it is an impossible language that manipulates and checks the state of various living or dead entities in the actual universe (and, in fact, across universes as well). This has not stopped several people from creating their own spins on it, each focusing on a different aspect of the original language's concept to preserve. My own version of ~ATH (ruleofsix ~ATH? RoS ~ATH?) is based heavily upon drocta ~ATH, although with several major differences that make it an altogether different language. Unlike every other implementation of ~ATH that I know of, tildeathc is a compiler (though a simple, limited one), not an interpreter. 

# How does RoS ~ATH work?
Much like Homestuck's ~ATH, RoS ~ATH consists entirely of loops which are predicated upon the life of certain entities (in otherwords, they loop "til death of" those entities). Unlike Homestuck's ~ATH, these entities are abstract objects created by the program that can also be killed by the program arbitrarily in order to control the execution of those loops. Every variable in RoS ~ATH is a reference to one of these objects, and can be overwritten with a reference to a different object (overwriting a varaible to refer to something that is _already dead_ is a common way to make a loop terminate). Objects are alive upon creation. Every object in RoS ~ATH has a left and a right "half", each of which is its own object that is alive upon creation. You can "bifurcate" an object to receive references to its left and right halves and assign variables to them. There is no way to "un-bifurcate" an object, or get a reference to it from its halves: this is because, in my view, ~ATH is a language concerned with death and inevitability and breaking things down, not with putting things back together. 

If this all seems a bit abstract and complicated, just try to read the explanations of the language syntax with this in mind as best you can, and hopefully seeing it in practice will help it make a bit more sense. If it still doesn't, feel free to email me at github@ruleofsix.ca: I will always be happy to talk about this very silly project. 

There are four main types of statements, or _graves,_ in ~ATH:

## Imports
The primary purpose of imports is to declare new variables. The syntax for imports is:
```
import <import_type> <var_name>;
```
The import types are `abstract`, `universe`, `input`, and `library`, the last of which is unique in that it does not create a new variable (more on that later). All variable names in RoS ~ATH can only consist of capital letters, digits, and underscores. Unlike in other programming languages, a variable name _can_ start with a digit or consist entirely of digits, because there is no built-in concept of numbers or math in RoS ~ATH. So imports can be used like this:
```
import abstract LAMB;
import universe U1;
import input IN;
import library NUMBERS256;
```
Abstract objects are the default, basic RoS ~ATH object, and what is used for the majority of program logic.  

Universe objects are alive, cannot be killed, and do not have left or right halves (in other words, they cannot be bifurcated).  

Input objects are used to take in input from users on the command line: when an input object is imported, the user will be prompted for input. This input can ONLY take the form of a string of 0 and 1 characters (eg. the text "011101" is valid input, but "hello" will cause a runtime error). This "binary" input is bound to the newly-created input object, and can be "read out" via successive bifurcation. 

Library import statements don't create objects, they effectively insert another ~ATH file in their own place. The name used for the import is the filename the compiler will search for to insert, but without the file extension: for instance, `import library NUMBERS256` will cause the compiler to insert the contents of NUMBERS256.~ATH into the file at that location. The only real utility of importing a library is to have your program come pre-loaded with certain objects already created. The NUMBERS256.~ATH library that is installed alongside the compiler is an example of the utility of this: it creates for you references to 256 objects, each the left half of the next, named N256 to N0. Only N0 is dead. This is a very simplistic way to encode numbers in RoS ~ATH (and also how numbers are encoded in drocta ~ATH), but writing out the about 260 lines of code to create these objects at the top of every program where you want to use numbers would be a chore.

## Loops
Loops are structured like this:
```
~ATH(<var_name>) {
    <graves>
} EXECUTE(<grave> | <print> | NULL);
```
The term "grave" simply refers to a statement. A loop executes while the variable it is dependent on is alive. Once that variable no longer refers to a living object, the loop stops and the singular grave inside the parentheses after EXECUTE is executed once. **The EXECUTE is never run if the variable was already dead (in other words, if the loop does not execute at least once)**. Also, two things besides a grave can go between the parentheses after EXECUTE: 
- A print statement looks like `PRINT "<string>";` It prints the given text out to the command line. A newline is automatically inserted after the given text. **Print statements are NOT graves and can ONLY go in EXECUTEs.**
- The keyword NULL in an EXECUTE means that you don't want to execute anything once the loop has terminated.

For example, to print the string "Hello, world!" once the variable LAMB is dead, you would do the following:
```
~ATH(LAMB) {
    # Additional graves...
} EXECUTE(PRINT "Hello, world!");
```
Of course, if there's nothing within the main loop body that would cause LAMB to be dead, then it will loop forever and "Hello, world!" will never be printed. Also, notice that print statements don't need semicolons. This is also true for the NULL keyword, but **graves need semicolons even when inside an EXECUTE.** 

## Deaths
Deaths are a grave that allow us to actually have loops terminate at some point such that we can actually do something. This is a big departure from Homestuck's ~ATH but I think you will like it. Deaths are either written as
```
<var_name>.DIE();
```
or as
```
[<var_name>, <var_name>, ... <var_name>].DIE();
```
where the second form can have any number of variable names, such that even `[<var_name>].DIE();` is valid. "Killing" a variable causes the object it refers to to become dead, and causes both of its halves to become dead, and both of the halves' halves, etc. Notably, this means that killing a living input variable completely destroys your ability to access any data it may have been containing. Killing an object that is already dead has no effect, and universe objects cannot be killed. 

**IMPORTANT: Every RoS ~ATH program MUST end with `THIS.DIE();` to close the program itself; furthermore, `THIS.DIE();` can be used elsewhere in the program in order to terminate early.**

We can use a death and an import to complete the loop example above into a functional program:
```
import abstract LAMB;
~ATH(LAMB) {
    LAMB.DIE();
} EXECUTE(PRINT "Hello, world!");
THIS.DIE();
```
The above program is the canonical example of Hello, World in RoS ~ATH.

## Bifurcations
Bifurcations are the main vehicle by which complex logic can be created, because they allow you to traverse the binary tree structure that RoS ~ATH objects form, albeit only in one direction (by going from an object to its halves). The syntax for a bifurcation looks like this:
```
bifurcate <var_name>[<left_half_name>, <right_half_name>];
```
So to take get the two halves of LAMB and assign them to the variables LAMB_LEFT and LAMB_RIGHT, you would do:
```
bifurcate LAMB[LAMB_LEFT, LAMB_RIGHT];
```
It is important to remember that, as far as RoS ~ATH is concerned, objects all already have halves upon creation (and their halves have halves, and so on). When you bifurcate an object, you simply are getting a reference to that object's left and right half and assigning them to variables. This means that you can bifurcate an object twice, each time giving the left half a different variable name, and both of those variables would refer to the same object. Also, if you overwrite one of those variables with a new value (by bifurcating something _else_ into it, for instance) it would not change what the original object's left half is. **You cannot change what halves an object has. The only way to alter state in ~ATH is by killing an object.** 

With the idea of overwriting variables in mind, you can replace an object OBJ with its left half by doing 
```
bifurcate OBJ[OBJ, NUL];
```
or with its right half by doing 
```
bifurcate OBJ[NUL, OBJ];
```
(Note: NUL is not a special variable name, I just use it to signify that I don't need that particular reference. It has no relation to the NULL keyword, and you could use any variable name you want for this purpose.)

This pattern allows you to repeatedly "descend" down a chain of halves until you reach a dead object. This is a concept used by the NUMBERS256 library:
```
import library NUMBERS256;
bifurcate N11[OBJ, NUL]; # Set OBJ to N10, which is the left half of N11
~ATH(OBJ) { # N0 is dead, so the loop will end when OBJ becomes a reference to N0
    bifurcate OBJ[OBJ, NUL]; # "Decrement" OBJ
} EXECUTE(PRINT "Done!");
THIS.DIE();
```
The above code will loop 10 times and then print "Done!". 

## Functions/Code Re-Use
There is none. There's not any syntax suggested for this in Homestuck, and ~ATH is pretty much designed to be an unusable nightmare, so I've kept that spirit. I may add some form of functions (called "graveyards") as a future optional compiler flag, but I'm undecided.

# Installation
You'll need gcc and glibc both to compile tildeathc, and also to compile and run tildeathc programs (although tildeathc will use them behind-the-scenes for you; they just need to be installed). You also need to be using amd64/x86_64 Linux: anything else and tildeathc will not be able to produce working code for your device. I have yet to test it on WSL. With that out of the way, it's as simple as 
```
git clone github.com/RuleOfSix/tildeathc
mkdir build
cd build
cmake ..
sudo make install
```
Run `tildeathc -h` to get an overview of compiler flags, and good luck!

# Contact
If you have any issues or find a bug you'd like to report, you can create an issue on this repo or email me at github@ruleofsix.ca. Or email me anyway if you just want to talk about the project!
