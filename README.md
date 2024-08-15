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

## Lock Full

To build the lock full version:

```text
cl /Zi /EHsc /std:c++20 /Fe:lockfull .\lockfull.cpp
```
