# ascii-to-bf-codes

Generate brainfxxk codes that print the inputs. (Still a prototype.)

## Compile

```bash
gcc ./src/ascii2bf.c -Wall -o ./ascii2bf.out
```

## Run

```bash
./ascii2bf.out
```

## An Example

```bash
./ascii2bf.out <<< "lky_is_god"
```

## Known Issues

### Encoding

Listed below are proved wrong.

```bash
./ascii2bf.out <<< "lkynb"
```

```bash
./ascii2bf.out <<< "1234abcd"
```

These are caused by the implementation of ```fit_into_addition_and_multiplication()```.

### Others

Currently, it can't directly encode words seperated by whitespaces.

```bash
./ascii2bf.out <<< "lky is god"
```

## To-do

Fix known issues, and a CXX version.
