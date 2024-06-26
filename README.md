# Archived

To study cpp standard library and implement something I desired to make, I published this repo.
But as time pass by, I found it difficult to achieve it by myself. So it's time to give up.

I remains a thought that, maybe one day I'll pick it up? 
Then I leave this here, and also mark my effort before.

**Actually there are some same (or similar) containers in the famous lib - boost. It implements
all my thought. So for usage, please try [boost](https://www.boost.org/).**

***

# Alternative CPP Containers (ACC)

Some implementations of C++ standard containers,
including `deque`, `vector`, `bitset`, `queue`.

The purpose of the project is to perform better from the mainstream
implementation in some conditions, and try not to do too bad under the others.

So that although most interfaces of ISO C++14 and some of ISO C++17 are 
provided, not all the requirements are fit, such as the iterator invalidation.
That is, they can be used as same as the standard library but perform a bit
different. See the documents in directory `./docs` for more details.

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
