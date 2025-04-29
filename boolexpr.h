#ifndef BOOL
#define BOOL
#include <string>
#include <vector>
#include <iostream>
class BooleanExpression {
public:
  explicit BooleanExpression(const char* input);

  std::string cnf();
  std::string dnf();
  std::string zhegalkin();
  std::string make_table();
  inline bool SaveZero() const;
  inline bool SaveOne() const;
  inline bool SelfDual() const;
  inline bool Monotonic() const;
  inline bool Linear() const;
  operator std::string() const;

 private:
  std::vector<std::string> queue_;
  std::vector<std::string> vars_;
  std::vector<std::string> MakePrefix(const char* input);
  static inline int  GetPriority(char ch);
  inline void AddVar(std::string var);

  static inline bool GetSmallResult(const std::string& now, bool value, bool value_1);
  bool Result(long long value) const;

  inline bool GetValue(long long value, std::string var_name) const;
};
bool IsFullSystem(const std::vector<BooleanExpression>& funcs);
#endif
