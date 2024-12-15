**This is a convention that all developers of this project must respect. It will make working together easier, more efficient and more consistent.**

- The entire project is in English. Whether it's the git, comments in the code, doxygen or even variable names.

- Variable name

	- Variables names are in camelCase

		- Begin with a lowercase letter

		- Words are in sequence

			- The next words begin with a capital letter

	- Prefix

		- Some variable types must have a prefix. If your variable follows more than one of these instructions, do them all in order.

			- Member variables must begin with the prefix m_

			- QStringList -> sList

			- string, QStrings (and other derivatives) -> s

			- int -> n

			- double -> d

			- bool -> b

			- Pointer -> p

			- Object name (without the q for qt objects)

	- Some variable types must have a suffix. If your variable follows more than one of these instructions, do them all in order.

		- function parameters -> in

		- value returned by a function -> out

- The first letter of the first word of a function is capitalized.

- There must be a space before and after these characters: '&&', '||', '==', '!=', '<=', '=>', '<<', '>>', '?', ':', '+' and '='.

- There must be a space after these characters: ','.

- Acolades must be on a single line.

- Functions must not exceed 60 lines in length.

- For header files, their extensions must be .h and not .hpp.

- First the Qt includes and others between '<' '>' then a skip line and the project includes between '"' '"'.

- Includes in alphabetical order. 

- Inclusions must be made in the .cpp file. Inclusions in the .h file should only be those necessary.

- Using pragma once instead of ifdef.

- A comment at the beginning of each file must be present, containing several versions such as the file creation date, its relative path (to the source repo), its author, its copyright and a summary.