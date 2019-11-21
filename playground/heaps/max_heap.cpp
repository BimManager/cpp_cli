// Copyright 2019 Samolet LLC
// Author: kkozlov
// max_heap.cpp

#using <System.dll>

ref class Node {
 public:
  Node(int n_neighbours) {
    _neighbours = gcnew array<Node^>(n_neighbours);
  }
  Node(System::IComparable^ data, int n_neighbours)
      : _data{data} {
    _neighbours = gcnew array<Node^>(n_neighbours);
  }

  System::IComparable^ Data(void) {
    return (_data);
  }
  void SetData(System::IComparable^ data) {
    _data = data;
  }
 protected:
  System::IComparable^	_data;
  array<Node^>^		_neighbours;
};

ref class HeapNode : Node {
 public:  
  HeapNode(System::IComparable^ data)
      :Node(data, 3) {}

  HeapNode^ Left(void) {
    return (static_cast<HeapNode^>(_neighbours[0]));
  }
  HeapNode^ Right(void) {
    return (static_cast<HeapNode^>(_neighbours[1]));
  }
  HeapNode^ Parent(void) {
    return (static_cast<HeapNode^>(_neighbours[2]));
  }
  void SetLeft(Node^ left) {
    _neighbours[0] = left;
  }
  void SetRight(Node^ right) {
    _neighbours[1] = right;
  }
  void SetParent(Node^ parent) {
    _neighbours[2] = parent;
  }
};

ref class MaxHeap {
 public:
  MaxHeap(void) {
    Initialize();
  }
  void Insert(HeapNode^ node) {

  }
  HeapNode^ Maximum(void) {
    return (nullptr);
  }
  HeapNode^ ExtractMaximum(void) {
    return (nullptr);
  }
  void IncreaseKey(HeapNode^ node,
                   System::IComparable^ new_data) {

  }
 private:
  void Initialize(void) {
    _count = 0;
  }
  int	_count;
};

void SwapNodes(Node^ node1, Node^ node2) {
  System::IComparable^	tmp;

  tmp = node1->Data();
  node1->SetData(node2->Data());
  node2->SetData(tmp);
}

void MaxHeapify(HeapNode^ node) {
  HeapNode^	max;

  max = node;
  if (max->Left() != nullptr
      && max->Data()->CompareTo(max->Left()->Data()) == -1)
    max = node->Left();
  if (max->Right() != nullptr
      && max->Data()->CompareTo(max->Right()->Data()) == -1)
    max = node->Right();
  if (max != node) {
    SwapNodes(node, max);
    MaxHeapify(max);
  }
}

void BuildMaxHeap(HeapNode^ root) {
  
}

void PreorderTraversal(HeapNode^ node) {
  if (node == nullptr)
    return ;
  System::Console::WriteLine(
      static_cast<System::String^>(node->Data()));
  PreorderTraversal(node->Left());
  PreorderTraversal(node->Right());
}

void SetupHeap(void) {
  HeapNode^	node;

  node = gcnew HeapNode("Aoo");
  node->SetLeft(gcnew HeapNode("Bar"));
  node->Left()->SetLeft(gcnew HeapNode("Zoo"));
  node->SetRight(gcnew HeapNode("Baz"));
  node->SetParent(nullptr);
  PreorderTraversal(node);
  System::Console::WriteLine("===after===");
  MaxHeapify(node);
  PreorderTraversal(node);
}

#using <System.Configuration.dll>

int main(void) {
  SetupHeap();
  return (0);
}
