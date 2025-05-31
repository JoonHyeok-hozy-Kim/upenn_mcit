### Concept) = Operator Overloading
- Desc.)
  - `=` : the assignment operator
    - Usage)
      ```cpp
      Point x(1,2);     // ctor
      Point y(x);       // cctor
      y = x;            // assignment
      y.operator=(x);   // equivalent to y=x;
      ```
- Overloading)
  - Why doing this?)
    - Customize the `=` operation.
    - If not, C++ will synthesize one.
      - This is usually wrong whenever a class has dynamically allocated data
  - e.g.) Overloading
    ```cpp
    Point& Point::operator=(const Point& rhs) {
        if (this != &rhs) {
            x_ = rhs.x_;
            y_ = rhs.y_;
        }

        return *this;  // The return type is the reference! Allow chaining.
    }
    ```
    - e.g.) Usage
      ```cpp
      Point a;
      a = b = c;    // Works because = returns *this.
      a.operator=(b.opeartor=(c)); // equiv.
      a = (b = c);  // Works.
      (a = b) = c;  // Works.
      ```



<br>

### Concept) Initialization List
- Syntax)
  ```cpp
  // Constructor with an initialization list
  Point::Point(int x, int y) : x_(x), y_(y) {}
  ```
- Why needed?)
  - It prevents the unnecessary default construction of fields.
    |When used|When not used|
    |:-|:-|
    |<code>Point::Point(int x, int y) {<br> int x_ = x; <br> int y_ = y; <br> int z_ = z; <br>}</code>|<code>Point::Point(int x, int y) {<br> int x_; <br> int y_; <br> int z_;<br> x_ = x; <br> y_ = y; <br> z_ = z; <br>}</code>|
  - A class with a reference data member compile only with the **Initialization List** ctor.
    ```cpp
    class Song {
        public:
            // Initialization List ctor
            Song(string name, int rating) name_(name), rating_(rating) {}; 
        private:
            string name_;
            int& rating_; // This member forces the ctor with the initialization list
    }
    ```
- Props.)
  - Data members in initializer list are initialized in the order they are **defined in the class**
    - cf.) not by the initialization list ordering


<br>

### Concept) Casting
- Desc.)
  - Recall the C-style castings as below.
    - Types)
      - Conversion between pointers of arbitrary type.
        - e.g.)
          ```cpp
          (void*) my_ptr;
          ```
      - Forcibly convert a primitive type to another.
        - e.g.)
          ```cpp
          (double) my_int;
          ```
    - Problem)
      - Intent of the casting is not clear...
        - Sol.) C++ provides alternative casting styles
          - [Static Casting](#concept-static-casting)
          - [Dynamic Casting](#concept-dynamic-casting)
          - [Const Casting](#concept-const-casting)
          - [Reinterpret Casting](#concept-reinterpret-casting)

#### Concept) Static Casting
- Desc.)
  - Eligibility)
    - Casting `void*` to `T*`.
    - Converting pointers to classes of **related** types
      - Compile error if the classes are not related.
        ```cpp
        class A {
            public:
                int x;
        };

        class B {
            public:
                float y;
        };

        class C : public B {
            public:
                char z;
        }
        ```
        ```cpp
        void foo() {
            B b; C c;

            // Compile error : unrelated types
            A* a_ptr = static_cast<A*>(&b);
            // Works, but also works without static_cast.
            B* b_ptr = static_cast<B*>(&c);
            // Compiles but dangerous: What if c_ptr->z?
            C* c_ptr = static_cast<C*>(&b);
        }
        ```
    - Non-pointer conversion.
      - e.g.)
        ```cpp
        void foo() {
            int b = 3;
            float c;
        }

        c = static_cast<float>(b);
        ```
- Prop.)
  - Used in cases where casting is not related to object inheritance.

<br>


#### Concept) Dynamic Casting
- Desc.)
  - Eligibility)
    - Converting...
      - Pointers to classes of related type.
      - References to classes of related type.
  - `dynamic_cast` is checked both at compile time and run time
    - At compile time, error if the classes are unrelated.
    - At run time, casts from the base type to the derived type fails if the pointed-to object is not the derived type.
- e.g.)
  ```cpp
  class Base {
    public:
        virtual void foo() {};
        float x;
  };

  class Der1: public Base {
    public:
        char x;
  };
  ```
  ```cpp
  void bar() {
    Base b; Der1 d;

    // Ok
    Base* b_ptr = dynamic_cast<Base*>(&d);
    assert(b_ptr != nullptr);
    // Ok
    Der1* d_ptr = dynamic_cast<Base*>(b_ptr);
    assert(d_ptr != nullptr);
    // Run-time check fails.
    b_ptr = &b;
    d_ptr = dynamic_cast<Der1*>(b_ptr);
    assert(d_ptr != nullptr);
  }
  ```


<br>


#### Concept) Const Casting
- Desc.)
  - Used to add or strip the `const` property.
- e.g.)
  ```cpp
  void foo(int* x) {
    *x++;
  }

  void bar(const int* x) {
    foo(x);     // compile error!
    foo(const_cast<int*>(x));  // Success!
  }
  ```


<br>


#### Concept) Reinterpret Casting
- Desc.)
  - Casting between incompatible types
    - Low-level reinterpretation of the bit pattern
      - e.g.) Storing a pointer in an `int`.
- e.g.) Server Socket
  ```cpp
  int listen_fd = Listen(port_num, &sock_family); // e.g.) 8080, AF_INET
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    int client_fd = accept(listen_fd,
                           reinterpret_cast<struct sockaddr*>(&caddr),
                           &caddr_len);
    ...
  }
  ```