# Lock Free Programming

This is the first part in a series of programming exercises designed to teach
concepts in lock free programming.

There are two different programs that do the same thing. The first one uses a
low level compare and swap primitive. The second uses a more traditional mutex
to provide thread safety.

## Lock Free

To build the lock free version:

```text
cl /Zi /EHsc /std:c++20 /Fe:lockfree .\lockfree.cpp
```

Output from run:

```text
Thread 23956 processed 30247143 ints in 4.8074186000 seconds.
Thread 28416 processed 36252950 ints in 4.8074490000 seconds.
Thread 32180 processed 36463337 ints in 4.8074095000 seconds.
Thread  5688 processed 31109101 ints in 4.8058779000 seconds.
Thread 27744 processed 32946896 ints in 4.8048257000 seconds.
Thread  6760 processed 32980573 ints in 4.8046970000 seconds.
200000000 operations performed across 6 threads in 4.8084145000 seconds.
```

## Lock Full

To build the lock full version:

```text
cl /Zi /EHsc /std:c++20 /Fe:lockfull .\lockfull.cpp
```

Output from run:

```text
Thread 15844 processed 36155658 ints in 10.4126380000 seconds.
Thread 33604 processed 35073039 ints in 10.4126755000 seconds.
Thread 12188 processed 33852992 ints in 10.4126452000 seconds.
Thread 25948 processed 28774518 ints in 10.4091534000 seconds.
Thread 23796 processed 35612326 ints in 10.4101690000 seconds.
Thread 29856 processed 30531467 ints in 10.4070143000 seconds.
200000000 operations performed across 6 threads in 10.4134749000 seconds.
```
