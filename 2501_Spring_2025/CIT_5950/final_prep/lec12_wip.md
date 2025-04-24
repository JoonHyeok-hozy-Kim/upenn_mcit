# Assignment Operator (=) Overloading
- Syntax)
  ```cpp
  Point& Point::operator=(const Point& rhs){
    if (this != &rhs) { // Always check this!
        this->x_ = rhs.x_;
        this->y_ = rhs.y_;
    }
    return *this; // Always return *this!
  }
  ```
- Usage)
  ```cpp
  void assignment_op_test() {
          Point p;
          Point q(1,2);
          Point r(3,4);
  
          p = q = r;      // All (3,4)
          p = (q = r);    // All (3,4)
          (p = q) = r;    // (3,4), (1,2), (3,4)
  }
  ```

<br><br>

# Initialization List
- Syntax)
  ```cpp
  class Point3D {
    public:
        Point3D(int x, int y, int z) : y_(y), x_(x) {
            z_ = z; // After x, y are assigned
        }
    private:
        int x_, y_, z_;
  }; 
  ```
- Props.)
  - Data members get initialized in the order defined in the class.
    - Not the initialization listing order.
  - Members not on the lists are default initialized before the body is executed.
  - Not using it takes up more resource.
    - Separately declaring and then initializing
      ```cpp
      int x_;
      x_ = x;
      ```
      - If a member allocates memory, it can be costly.
        - e.g) String, vector
    - When used, it will be something like
      ```cpp
      int x_ = x;
      ```
  - If a member is a reference variable, initialization list is required!!!