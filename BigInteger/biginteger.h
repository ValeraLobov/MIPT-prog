#include <iostream>
#include <vector>
#include <string>

enum number_sign { Sign_Negative, Sign_Positive };

class BigInteger;

int max(const int a, const int b) { return a > b ? a : b; }

void reverse(std::string::iterator left, std::string::iterator right) {
  while ((left != right) && (left != --right)) {
    std::swap(*left, *right);
    ++left;
  }
}

bool operator<(const BigInteger&, const BigInteger&);
bool operator>(const BigInteger&, const BigInteger&);
bool operator<=(const BigInteger&, const BigInteger&);
bool operator>=(const BigInteger&, const BigInteger&);
bool operator==(const BigInteger&, const BigInteger&);
bool operator!=(const BigInteger&, const BigInteger&);
const BigInteger operator+(BigInteger, const BigInteger&);
const BigInteger operator-(BigInteger, const BigInteger&);
const BigInteger operator*(BigInteger, const BigInteger&);
const BigInteger operator/(BigInteger, const BigInteger&);
const BigInteger operator%(BigInteger, const BigInteger&);
BigInteger absolute(const BigInteger&);

class BigInteger {
  std::vector<int> array;
  int length;
  number_sign sign;
  int getElement(const int& index) const { return array[index]; }
  void reverse() {
    for (int i = 0; i < length / 2; ++i)
      std::swap(array[i], array[length - i - 1]);
  }
  void deleteZeroes() {
    reverse();
    while (length > 0 && array[length - 1] == 0) array.pop_back(), --length;
    if (!length) array.push_back(0), ++length;
    reverse();
  }
  void pop() {
    array.pop_back();
    --length;
  }
  std::pair<BigInteger, BigInteger> getQuotientAndModulo(BigInteger divisor) {
    if (!divisor) return std::make_pair(0, 0);
    BigInteger dividend = *this;
    number_sign quotient_sign =
        getSign() == divisor.getSign() ? Sign_Positive : Sign_Negative;
    dividend.setSign(Sign_Positive);
    divisor.setSign(Sign_Positive);
    std::vector<int> result_array;
    while (divisor <= dividend) {
      divisor *= 10;
      result_array.push_back(0);
    }
    divisor.pop();
    for (size_t i = 0; i < result_array.size(); ++i) {
      while (divisor <= dividend) {
        dividend -= divisor;
        ++result_array[i];
      }
      divisor.pop();
    }
    BigInteger quotient(result_array);
    quotient.deleteZeroes();
    dividend.deleteZeroes();
    quotient.setSign(quotient_sign);
    dividend.setSign(getSign());
    if (quotient.getElement(0) == 0) quotient.setSign(Sign_Positive);
    if (dividend.getElement(0) == 0) dividend.setSign(Sign_Positive);
    return std::make_pair(quotient, dividend);
  }

 public:
  number_sign getSign() const { return sign; }
  int getSize() const { return length; }
  void setSign(const number_sign& new_sign) { sign = new_sign; }
  BigInteger() {
    array.push_back(0);
    length = 1;
    sign = Sign_Positive;
  }
  BigInteger(const int& n) {
    long long number = n;
    length = 0;
    sign = Sign_Positive;
    if (number < 0) {
      number *= -1;
      sign = Sign_Negative;
    }
    while (number > 0) {
      array.push_back(number % 10);
      ++length;
      number /= 10;
    }
    if (!length) ++length, array.push_back(0);
    reverse();
  }
  BigInteger(const std::vector<int>& array) : array(array) {
    length = array.size();
    sign = Sign_Positive;
  }
  BigInteger(const std::string& string) {
    length = 0;
    sign = Sign_Positive;
    if (string[0] == '-') sign = Sign_Negative;
    for (size_t i = 0; i < string.size(); ++i) {
      if (string[i] != '-') array.push_back(string[i] - '0'), ++length;
    }
    if (array[0] == 0) sign = Sign_Positive;
  }
  const BigInteger operator-() const { return -1 * (*this); }
  BigInteger& operator++() { return * this += 1; }
  const BigInteger operator++(int) {
    BigInteger answer = *this;
    *this += 1;
    return answer;
  }
  BigInteger& operator--() { return * this -= 1; }
  const BigInteger operator--(int) {
    BigInteger answer = *this;
    *this -= 1;
    return answer;
  }

  BigInteger& plus_same_sign(const BigInteger& summand) {
    std::vector<int> sumArray;
    int carry = 0;
    for (int i = 0; i < max(getSize(), summand.getSize()); ++i) {
      int current = carry;
      if (summand.getSize() - i - 1 >= 0)
        current += summand.getElement(summand.getSize() - i - 1);
      if (getSize() - i - 1 >= 0) current += getElement(getSize() - i - 1);
      sumArray.push_back(current % 10);
      carry = current / 10;
    }
    sumArray.push_back(carry);
    *this = BigInteger(sumArray);
    reverse();
    deleteZeroes();
    setSign(summand.getSign());
    return *this;
  }
  BigInteger& plus_other_sign(const BigInteger& summand) {
    bool isSummandGreater = false;
    if (summand.getSize() > getSize()) isSummandGreater = true;
    if (summand.getSize() == getSize()) {
      for (int i = 0; i < getSize(); ++i) {
        if (array[i] != summand.array[i]) {
          isSummandGreater = (array[i] < summand.array[i]);
          break;
        }
      }
    }
    if (isSummandGreater) {
      changnumber_sign();
      BigInteger temp = summand;
      temp.changnumber_sign();
      temp += *this;
      *this = temp;
      changnumber_sign();
    } else {
      int indent = getSize() - summand.getSize();
      std::vector<int> differenceArray;
      int carry = 0;
      for (int i = 0; i < summand.getSize(); ++i) {
        int current = getElement(getSize() - i - 1) -
                      summand.getElement(summand.getSize() - i - 1) - carry;
        (current < 0) ? current += 10, carry = 1 : carry = 0;
        differenceArray.push_back(current);
      }
      for (int i = indent - 1; i >= 0; --i) {
        int current = getElement(i) - carry;
        if (current < 0) {
          current += 10;
          carry = 1;
        } else
          carry = 0;
        differenceArray.push_back(current);
      }
      number_sign signOfAnswer = getSign();
      *this = BigInteger(differenceArray);
      reverse();
      deleteZeroes();
      sign = signOfAnswer;
    }
    return *this;
  }
  BigInteger& operator+=(const BigInteger& summand) {
    if (getSign() == summand.getSign()) {
      return plus_same_sign(summand);
    } else {
      return plus_other_sign(summand);
    }
  }
  BigInteger& operator-=(const BigInteger& subtrahend) {
    changnumber_sign();
    *this += subtrahend;
    changnumber_sign();
    return *this;
  }
  BigInteger& operator*=(const BigInteger& multiplier) {
    std::vector<int> product_array;
    for (int i = 0; i < getSize() + multiplier.getSize(); ++i) {
      product_array.push_back(0);
    }
    for (int i = 0; i < getSize(); ++i) {
      int carry = 0;
      for (int j = 0; j < multiplier.getSize(); ++j) {
        int current = product_array[i + j] + carry +
                      getElement(getSize() - i - 1) *
                          multiplier.getElement(multiplier.getSize() - j - 1);
        product_array[i + j] = current % 10;
        carry = current / 10;
      }
      int index = i + multiplier.getSize();
      while (index < getSize() + multiplier.getSize() && carry != 0) {
        int current = product_array[index] + carry;
        product_array[index++] = current % 10;
        carry = current / 10;
      }
    }
    BigInteger product(product_array);
    product.reverse();
    product.deleteZeroes();
    product.setSign(getSign() == multiplier.getSign() ? Sign_Positive
                                                      : Sign_Negative);
    if (product.getElement(0) == 0) product.setSign(Sign_Positive);
    return * this = product;
  }
  BigInteger& operator/=(const BigInteger& b) {
    return * this = getQuotientAndModulo(b).first;
  }
  BigInteger& operator%=(const BigInteger& b) {
    return * this = getQuotientAndModulo(b).second;
  }
  BigInteger& operator=(const BigInteger& big_integer) {
    length = big_integer.length;
    sign = big_integer.sign;
    array = big_integer.array;
    return *this;
  }
  explicit operator bool() const { return getElement(0) != 0; }
  const std::string toString() const {
    std::string string;
    if (sign == Sign_Negative && array[0] != 0) string.push_back('-');
    for (int i = 0; i < length; ++i) string.push_back('0' + array[i]);
    return string;
  }
  void changnumber_sign() {
    if (sign == Sign_Negative)
      sign = Sign_Positive;
    else
      sign = Sign_Negative;
  }
};

BigInteger absolute(const BigInteger& big_integer) {
  if (big_integer.getSign() == Sign_Positive) return big_integer;
  return -big_integer;
}

const BigInteger operator+(BigInteger left, const BigInteger& right) {
  return left += right;
}

const BigInteger operator-(BigInteger left, const BigInteger& right) {
  return left -= right;
}

bool operator<(const BigInteger& left, const BigInteger& right) {
  std::string left_string = left.toString();
  std::string right_string = right.toString();
  if (left_string[0] == '-') {
    reverse(left_string.begin(), left_string.end());
    left_string.pop_back();
    reverse(left_string.begin(), left_string.end());
  }
  if (right_string[0] == '-') {
    reverse(right_string.begin(), right_string.end());
    right_string.pop_back();
    reverse(right_string.begin(), right_string.end());
  }
  if (left.getSign() == Sign_Negative && right.getSign() == Sign_Positive)
    return true;
  if (left.getSign() == Sign_Positive && right.getSign() == Sign_Negative)
    return false;
  if (left.getSign() == Sign_Negative && right.getSign() == Sign_Negative)
    return BigInteger(right_string) < BigInteger(left_string);
  if (left.getSize() != right.getSize())
    return left.getSize() < right.getSize();
  return left_string < right_string;
}

bool operator>(const BigInteger& left, const BigInteger& right) {
  return right < left;
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
  return left < right + 1;
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
  return right <= left;
}

bool operator==(const BigInteger& left, const BigInteger& right) {
  return !(left > right || left < right);
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
  return !(left == right);
}

const BigInteger operator*(BigInteger left, const BigInteger& right) {
  return left *= right;
}

const BigInteger operator/(BigInteger left, const BigInteger& right) {
  return left /= right;
}

const BigInteger operator%(BigInteger left, const BigInteger& right) {
  return left %= right;
}

std::istream& operator>>(std::istream& in, BigInteger& big_integer) {
  std::string string;
  in >> string;
  big_integer = BigInteger(string);
  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& big_integer) {
  out << big_integer.toString();
  return out;
}