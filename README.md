# Variable resolution (resvars)

## Description

Resvars is a tool that can be used to parse a string and resolves variables contained in them in a similar fashion as shell. 

## Example

Input:
```
varA=Hello
varB=World
varC=${varA} ${varB}
```
Output:
```
varA=Hello
varB=World
varC=Hello World
```
Resvars is able to parse a text file containing empty lines and comments for example:

Input:
```
# Comments in file

# Comment line
# Another comment line
abc=def
hello World!
abg=edw

aaa=${abg}
```
Output:
```
aaa=edw
abc=def
abg=edw
```
## How does it work?

### Adding variables

Resvars relies upon a hashmap data structure built for this use case. Every line that does not start with a `#` and contains an `=` is considered a key/value pair.

`varA=Hello` Key=`varA`, value=`Hello`. 

Each key/value pair is added to the hashtable. 

### Resolving variables

Whenever a value has a `${`, resvars will parse until the first encountered `}` on the line. Everything between the `${` and `}` is then used as a key to look for in the hashmap.

`varA=${varB}`. Resvars resolves `varB` et searches for the `varB` key in the hashmap. If found, varB is replaced with the value of the `varB` key. 


## Add to an existing project

Adding resvars to a project is really simple. You can simply import the C files into your C project and make sure you include either `hashmap.h` or `resvars.h`. 
