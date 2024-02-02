# Alternative CPP Containers (ACC)

Some implementations of C++ standard containers,
including `deque`, `vector`, `bitset`, `queue`.

The purpose of the project is to perform better from the mainstream
implementation in some conditions, and try not to do too bad under the others.

So that although most interfaces of ISO C++14 and some of ISO C++17 is 
achieved, not all the requirements are fit, such as the iterator invalidation.
See the documents in directory `./docs` for more details.

_Now we only implement `deque`. The others will come soon._

## Getting Started

The C++ header files are under `./acc` directory. Just move it to your workspace
folder, then include it in your cpp files.

The documents of the ACC project and the containers are under `./docs`
directory.

The sample programs are under `./test`, and these programs test the
reliability of data structures also.

## More Information

+ [License](/LICENSE)
+ [Deque introduction](/docs/deque/introduction.md)
+ [cppreference.com](https://en.cppreference.com/w/)