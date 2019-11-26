// Copyright Samolet LLC
// Author: kkozlov
// bank.cpp

#include "bank.h"

namespace bank {
void	Account::Deposit(double amount) {
  balance_ += amount;
}
void	Account::Withdraw(double amount) {
    balance_ -= amount;
}
double Account::Balance(void) {
  return (balance_);
}
void Account::TransferFunds(Account^ destination, double amount) {
}
}  // namespace bank
