### Activate debug mode
```
cmake -DCMAKE_BUILD_TYPE=Debug .


```

### Isolate boost headers (bcp tool)
http://stackoverflow.com/a/20922288/1707931

I couldn't make it work with the version that Debian installed, so I downloaded it from scratch.
```
7z x boost_1_59_0.7z
mkdir foo
bcp --boost=/tmp/boost_1_59_0 shared_ptr /tmp/foo
matthias@rp3deb:/tmp$ du -h -d 2 foo/
24K	foo/libs/smart_ptr
28K	foo/libs
20K	foo/boost/core
20K	foo/boost/detail
572K	foo/boost/type_traits
520K	foo/boost/predef
8.0K	foo/boost/utility
2.4M	foo/boost/preprocessor
224K	foo/boost/mpl
220K	foo/boost/smart_ptr
20K	foo/boost/exception
492K	foo/boost/config
4.5M	foo/boost
4.5M	foo/
```

### clang-check
Couldn't figure out how to make this work for now :/

### oclint
Downloaded 0.8.1 from http://oclint.org/downloads.html.
Generate compile_commands.json
http://docs.oclint.org/en/dev/guide/cmake.html
```
~/oclint-0.8.1/bin/oclint -p . main_catch.cpp
```
gives output that my variable names suck...

http://oclint.org/news/2013/06/22/clang-static-analyzer-integration/
```
~/oclint-0.8.1/bin/oclint -p . main_catch.cpp --enable-clang-static-analyzer
```
yields same result.

