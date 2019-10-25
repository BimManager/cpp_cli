// Copyright 2019 Samolet LLC
// Author: kkozlov
// bank_unittests.cpp

#include "bank.h"

#using <Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll>

namespace bank {
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

[TestClass]
public ref class AccountTest {
public:
  [TestMethod]
  void	TransferFunds() {
    Account^	source;
    Account^	destination;

    source = gcnew Account();
    source->Deposit(200);
    destination = gcnew Account();
    destination->Deposit(150);
    source->TransferFunds(destination, 100);
    Assert::AreEqual(250, destination->Balance());
    Assert::AreEqual(100, source->Balance());
  }
};
}  // namespace bank
