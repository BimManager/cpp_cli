#ifndef DATA_STRUCTURES_BINARY_TREE_H_
#define DATA_STRUCTURES_BINARY_TREE_H_

namespace data_structures {
namespace trees {
public ref class Node {
public:
  Node(int n_neighbours) {
    neighbours_ = gcnew array<Object^>(n_neighbours);
  }
  Node(System::Object^ data, int n_neighbours)
      :data_{data} {
    neighbours_ = gcnew array<Object^>(n_neighbours);
  }  
protected:
  System::Object^		data_;
  array<System::Object^>^	neighbours_;
};

public ref class BinaryTreeNode : Node {
public:
  BinaryTreeNode(void)
      :Node(2) {}
  BinaryTreeNode(System::Object^ data)
      :Node(data, 2) {}

  void	SetLeft(Node^ left) {
    neighbours_[0] = left;
  }
  void	SetRight(Node^ right) {
    neighbours_[1] = right;
  }
  Node^	Left(void) {
    return (neighbours_[0]);
  }
  Node^	Right(void) {
    return (neigbours_[1]);
  }
};
}  // trees
}  // data_structures
#endif
