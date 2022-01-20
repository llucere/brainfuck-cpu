# brainfuck cpu

A fully functional computer that only likes brainfuck.

## Brainfuck for goofy goobers

`+` add 1 to the current cell

`-` subtract 1 from the current cell

`>` go right one cell

`<` go left one cell

`,` get a users input. `,,,,,` will get a string of 5 characters, no individual char getting needed.

`.` print the current cells ascii value

`[` begin a loop. does not begin the loop if the current cells value is 0

`]` end a loop. only ends if the current cells value is 0

`{` executes the code block only if the current cell is 0

`}` ends the condition code block

`~` flips the bits of the current cell (excluding sign)

`%` sleeps for 100ms

`*` clears the screen

`@` sets the current cell to 'a'

`@@` sets the current cell to 'A'

`/` outputs a newline character (linefeed character)

`$` exits the program

- any characters other than these are ignored when a brainfuck program is reading your brainfuck code. have fun typing comments bozo

- there are 30,000 cells in a brainfuck program, all of which are 0

- you start at cell 0

## Ideas

'Hide' command:

- Hides the cells curent value during the next 2 cycles, e.g.

```c
mem[0] = 65;
hide(&mem[0]);
putchar(mem[0]); // does not print A
putchar(mem[0]); // does not print A
putchar(mem[0]); // prints A
```

- Gets refreshed once the hide command is called upon again

Multi-threading:

- () creates multiple 'threads' (concurrency), with ! being the command to break out the current thread e.g.

```bf
(%@.)@@.
output:
Aa
```

## OPTIMIZATION LIST

`[]` -> EXIT

`[-]` -> CLRS

`[+]` -> CLRS

`@@` -> STDA

`~~` -> SLP

`+-` -> SLP

`-+` -> SLP

`{}` -> SLP

### OPTIMIZATION LIST DESCRIPTIONS

`ADD` - `+`

`SUB` - `-`

`SHL` - `<`

`SHR` - `>`

`LOP` - `[`

`ELP` - ']`

`CHR` - `,`

`PUT` - `.`

`COND` - `{`

`ECND` - `}`

`DRAW` - `#`

`SLP` - `%`

`BFLP` - `~`

`CLS` - `*`

`SETA` - `@`

`STDA` - `@@`

`CLRS` - `blank`, clears the current cell by setting it to 0 instantly

`LNFD` - `/`

`EXIT` - `$`
