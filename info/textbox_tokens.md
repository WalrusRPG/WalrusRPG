# Textbox token list
## Structure and explanation
Here's how a token is constructed
```
/!\ Big endian.
0xff[AA][BB][CC][DD]
AA : Command ID
BB : Argument 1
CC : Argument 2
DD : Argument 3
```
The command can read its arguments separated or combine them to make bigger values (like 16-bit integers).

# Command list
- `0x01` Changes text cursor's color. Further text will be drawn in red.
    - Arguments : A1-2 (read as big-endian unsigned 16-bit integer)
    - Default value : 0xFFFF (pure white)
- `0x81` Changes textbox's delay between letters.
    - Arguments : A1-2 (read as big-endian unsigned 16-bit integer)
    - Default value : 0x000A (10)