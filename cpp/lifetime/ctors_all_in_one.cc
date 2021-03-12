#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

// https://stackoverflow.com/questions/18290523/is-a-default-move-constructor-equivalent-to-a-member-wise-move-constructor

// a simple memory block that shows the idea of RAII with copy and move
// constructors.

class Block {
  size_t length_;
  int *data_;

public:
  // simple constructors to initialize the resourece.
  explicit Block(size_t length) : length_(length), data_(new int[length]) {
    std::cout << "In Block(sizet), length = " << length_ << "." << std::endl;
    for (int i = 0; i < length_; ++i) {
      data_[i] = i;
    }
  }

  std::string to_string() {
    std::stringstream ss;
    for (int i = 0; i < length_; ++i) {
      ss << data_[i];
    }

    return std::string("<Block, length") + std::to_string(length_) + ">";
  }

  // destructor
  // we manually write it, but the default one generated by the compiler should
  // do something similar
  ~Block() {
    std::cout << "In ~Block(), length = " << length_ << "." << std::endl;
    if (data_ != nullptr) {
      std::cout << "Deleting data_";
      delete[] data_;
    }

    std::cout << std::endl;
  }

  // copy constructor
  Block(const Block &other)
      : length_(other.length_), data_(new int[other.length_]) {

    std::cout << "In Block(const Block&), length = " << length_ << "."
              << std::endl;

    std::copy(other.data_, other.data_ + length_, data_);
  }

  Block &operator=(const Block &other) {
    std::cout << "In Block& operator= (const Block&), length = " << length_
              << "." << std::endl;

    // free existing resource
    if (this != &other) {
      delete[] data_;

      length_ = other.length_;
      data_ = new int[length_];
      std::copy(other.data_, other.data_ + length_, data_);
    }
    return *this;
  }

  size_t length() { return length_; }

  Block(Block &&other) noexcept : data_(nullptr), length_(0) {
    std::cout << "In Block(Block&&), length = " << length_ << "." << std::endl;

    // the diference is you don't allocate new data_, instead you just grab it's
    // data.
    data_ = other.data_;
    length_ = other.length_;

    // we don't care the state of the other code, just make it null.
    other.data_ = nullptr;
    other.length_ = 0;
  }

  Block &operator=(Block &&other) {
    delete[] data_;

    data_ = other.data_;
    length_ = other.length_;

    other.data_ = nullptr;
    other.length_ = 0;
    return *this;
  }
};

void use_move_improve_performance_1() {
  // rvalue refernce.
  // 1. construct Block in place (a rvalue).
  // 2. pass the rvalue as rvalue reference.
  // 3. v get rvalue and steal it's data.
  //
  // Note, because Block(25) rvalue, we don't care about how it is
  // after it's constructed.
  std::vector<Block> v;
  v.reserve(3);
  v.push_back(Block(2));
  v.push_back(Block(3));
  v.push_back(Block(5));
  v.push_back(Block(10)); // from these one vector needs to resize.
  v.push_back(Block(12)); // move will be called twice. (not copy thank god.)
  v.push_back(Block(13));
  std::cout << "hitting }" << std::endl;
}

void what_happens_to_source_of_a_move_1_lval() {
  static_assert(std::is_move_constructible_v<Block>);
  Block b(20);

  // cast block& to block&&;
  Block d(std::move(b));

  // notice, because of our move constructor's implemnetation, b its moved and
  // we should not use it afterward.
  std::cout << b.to_string() << std::endl;
}

struct Cube {
  int data_[10];
  Cube() {
    for (int i = 0; i < 10; ++i) {
      data_[i] = i;
    }
  }
  std::string to_string() {
    std::stringstream ss;
    for (int i = 0; i < 10; ++i) {
      ss << data_[i] << ",";
    }
    return ss.str();
  }
};

void what_happens_to_source_of_a_move_2_default_move() {

  Cube b;
  Cube d(std::move(b));

  // notice, int[10] is not move constructible
  static_assert(!std::is_move_constructible_v<int[10]>);

  static_assert(std::is_move_constructible_v<Cube>);
  static_assert(std::is_copy_constructible_v<Cube>);

  // using default move constructor nothing happen.
  std::cout << "d: " << d.to_string() << std::endl;
  std::cout << "b: " << b.to_string() << std::endl;

  for (int i = 0; i < 10; ++i) {
    d.data_[i] = 1;
  }

  // move actually copied...
  std::cout << "d: " << d.to_string() << std::endl;
  std::cout << "b: " << b.to_string() << std::endl;
}

int main(void) {
  std::cout << "use_move_improve_performance_1" << std::endl;
  use_move_improve_performance_1();
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

  std::cout << "what_happens_to_source_of_a_move_1_lval" << std::endl;
  what_happens_to_source_of_a_move_1_lval();
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

  std::cout << "what_happens_to_source_of_a_move_2_default_move" << std::endl;
  what_happens_to_source_of_a_move_2_default_move();
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

  return 0;
}