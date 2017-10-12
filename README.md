# terark basic library

## Prerequisite
  * [boost-1.41 or newer](http://sourceforge.net/projects/boost/)
  * `terark-base` use boost as header-only library

## Compile
```bash
$ cd /path/to/terark-base
$ make
```

## Components
* [valvec](#valvec)
* [Lexical Cast](#lexical-cast)
* [Read text lines](#read-text-lines)

<h3 id="valvec">valvec</h3>
`terark::valvec` is a alternative to `std::vector`, it use `malloc`/`realloc`/`free`
to manage memory. `Type` of `terark::valvec<Type>` must be [memmove-able](#memmove-able).

valvec support terark serialization(DataIO).

#### Fast expansion
`valvec` is much faster than `std::vector` when expanding memory, when expanding,
`std::vector` first allocate a larger memory, then copy-cons(move-cons in c++11) old
objects, this is slow. But `valvec` just calling `realloc' to expand the memory, in
virtual addressing system, for example:
```c++
   void* p = malloc(oldsize); // `p` is a virtual address
   /// do something
   void* q = realloc(p, newsize); // `q` is another virtual address
```
When available virtual address range pointed by `p` is not big enough,
it is posible to just [remap](http://linux.die.net/man/2/mremap) the physical address of `p` into a larger
free virtual address range. Most popular `malloc` implementations have used [remap](http://linux.die.net/man/2/mremap)
to get this advantage.

#### low level functions
  * risk_set_data, risk_set_size, risk_set_capacity
  * risk_release_ownership

[terarkdb](https://github.com/Terark/terarkdb) use these functions for
supporting DFA memory map in an graceful way.

#### utility functions
```c++
   terark::valvec<int> vec;
   vec.push(1); // `push` is an alias of `push_back`
   vec.push(2); // expand when full
   //...
   vec.reserve(vec.size() + 100);
   vec.unchecked_push(123); // assert when full
   int val = vec.pop_val(); // throws when empty
   //...
   while (!vec.empty()) {
      int top = vec.unchecked_pop_val();
      //...
   }
```

<h4 id="memmove-able">memmove-able</h4>
When memmove an object `x` to another memory block `y` without destruct `x`,
if `y` can be used as it is `x`, then `x` is memmove-able.

For example, gnu `std::list,map,set`... are not memmove-able, because they
have self-references.

<h3 id="lexical-cast">Lexical Cast</h3>
Both compiling and running are much fast than `boost::lexical_cast`.
```c++
#include <terark/lcast.hpp>

//...
	int    i = terark::lcast("123");
	long   l = terark::lcase(std::string("1") + "23");
	float  f = terark::lcast("1.23");
	double d = terark::lcast("1.23");
```

<h3 id="read-text-lines">Read text lines</h3>
```c++
#include <terark/util/linebuf.hpp>

//...
	terark::LineBuf line;
	terark::valvec<fstring> fields; // fstring didn't own memory
	while (line.getline(stdin) > 0) {
		line.chomp(); // like perl chomp
		line.split(&fields);
		if (fields.size()) {
			double dval = terark::lcast(fields[0]);
			//...
		}
	}
```
