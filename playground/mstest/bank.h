// Copyright Samolet LLC
// Author: kkozlov
// bank.h

namespace bank {
public ref class Account {
public:
  void	Deposit(double amount);
  void	Withdraw(double amount);
  double Balance(void);
  void TransferFunds(Account^ destination, double amount);
private:  
  double	balance_;
};
}  // namespace bank
