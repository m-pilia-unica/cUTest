cUTest - C Unit Test Framweork
==============================
cUTest is a simple implementation of a unit test framework in C.

Build and install
=================
To automatically build the library in a Unix-like environment (requires 
[make](http://www.gnu.org/software/make/) and 
[gcc](https://gcc.gnu.org/)):

```bash
make
```

To build and run the library unit test (requires
[check](http://check.sourceforge.net/)):

```bash
make test
```

To build the library documentation (requires [Doxygen](http://www.doxygen.org)):

```bash
make doc
```

To install the library, launch:

```bash
sudo make install
```

After the installation, you can include the header <code>cutest.h</code> 
in your C source files and compile your code with the linker 
flags <tt>-lcutest</tt> and <tt>-lm</tt>.

To uninstall the library, launch:

```bash
sudo make uninstall
```
License
===================
The project is licensed under GPL 3. See [LICENSE](./LICENSE)
file for the full license.
