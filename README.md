## Fuzzing libc

is a set of fuzzing tests intended for testing standard C library.
By default system C library is used.

### Building

```sh
git clone --jobs $(nproc) --recursive https://github.com/ligurio/libc-tests
CC=clang CXX=clang++ cmake -S . -B build
cmake --build build --parallel
```

or

```
$ cmake --workflow --preset default
```

### Running

```sh
cmake --build build --target test
cd build && ctest -R strptime_test --verbose
```

### License

MIT License, [Sergey Bronnikov](https://bronevichok.ru/)
