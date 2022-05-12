#include "mystring.h"

std::map<const char *, MyString::StringValue *, MyString::StringValue::comparer> MyString::StringValue::allValues;

MyString::StringIndexProxy::operator char() const {
    return owner_->value_->data_[index_];
}

MyString::StringIndexProxy &MyString::StringIndexProxy::operator=(char c) {
    if (owner_->value_->refcount_ > 1)
        owner_->replaceValue(StringValue::makeStringValue(owner_->value_->data_));
    owner_->value_->data_[index_] = c;
    return *this;
}


MyString::StringIndexProxy &MyString::StringIndexProxy::operator=(const MyString::StringIndexProxy &rhs) {
    *this = char(rhs);
    return *this;
}

MyString &MyString::operator=(const MyString &rhs) {
    if (this != &rhs) {
        replaceValue(rhs.value_);
        value_->refcount_++;
    }
    return *this;
}

MyString &MyString::operator=(MyString &&rhs) noexcept {
    if (this != &rhs) {
        replaceValue(rhs.value_);
        rhs.value_ = nullptr;
    }
    return *this;
}

MyString MyString::operator+(const MyString &rhs) const {
    char *ret = new char[length() + rhs.length() + 1];
    strcpy(ret, value_->data_);
    strcpy(ret + length(), rhs.value_->data_);
    return MyString(StringValue::makeStringValue(ret, true));
}

MyString &MyString::operator+=(const MyString &rhs) {
    char *ret = new char[length() + rhs.length() + 1];
    strcpy(ret, value_->data_);
    strcpy(ret + length(), rhs.value_->data_);
    replaceValue(StringValue::makeStringValue(ret, true));
    return *this;
}

MyString MyString::operator+(char rhs) const {
    if (rhs == '\0')
        return MyString(*this);

    char *ret = new char[length() + 2];
    strcpy(ret, value_->data_);
    ret[length()] = rhs;
    ret[length() + 1] = '\0';
    return MyString(StringValue::makeStringValue(ret, true));
}

MyString operator+(char lhs, const MyString &rhs) {
    if (lhs == '\0')
        return MyString(rhs);

    char *ret = new char[rhs.length() + 2];
    ret[0] = lhs;
    strcpy(ret + 1, rhs.value_->data_);
    return MyString(MyString::StringValue::makeStringValue(ret, true));
}

MyString &MyString::operator+=(char rhs) {
    if (rhs != '\0') {
        char *ret = new char[length() + 2];
        strcpy(ret, value_->data_);
        ret[length()] = rhs;
        ret[length() + 1] = '\0';
        replaceValue(StringValue::makeStringValue(ret, true));
    }
    return *this;
}

MyString::StringIndexProxy MyString::operator[](size_t i) {
    if (i >= length())
        throw std::out_of_range{"Too high index"};
    return StringIndexProxy(this, i);
}

bool MyString::operator==(const MyString &rhs) const {
    if (this == &rhs || this->value_ == rhs.value_) return true;
    return strcmp(value_->data_, rhs.value_->data_) == 0;
}

bool MyString::operator!=(const MyString &rhs) const {
    return !(*this == rhs);
}

bool MyString::operator<(const MyString &rhs) const {
    return strcmp(value_->data_, rhs.value_->data_) < 0;
}

bool MyString::operator<=(const MyString &rhs) const {
    if (this == &rhs || this->value_ == rhs.value_) return true;
    return strcmp(value_->data_, rhs.value_->data_) <= 0;
}

bool MyString::operator>(const MyString &rhs) const {
    return strcmp(value_->data_, rhs.value_->data_) > 0;
}

bool MyString::operator>=(const MyString &rhs) const {
    if (this == &rhs || this->value_ == rhs.value_) return true;
    return strcmp(value_->data_, rhs.value_->data_) >= 0;
}


void MyString::dropValue() {
    value_->refcount_--;
    if (value_->refcount_ == 0) {
        delete value_;
    }
}

void MyString::replaceValue(MyString::StringValue *new_value) {
    dropValue();
    value_ = new_value;
}


std::ostream &operator<<(std::ostream &os, const MyString &myString) {
    os << myString.value_->data_;
    return os;
}

std::istream &operator>>(std::istream &is, MyString &myString) {
    char c;
    is.get(c);
    while (!std::isspace(c)) {
        myString += c;
        is.get(c);
    }
    return is;
}