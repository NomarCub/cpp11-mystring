#ifndef NHF1_MYSTRING_H
#define NHF1_MYSTRING_H

#include <cstring>
#include <iostream>
#include <map>

class MyString final {
    class StringValue final {
        struct comparer {
            bool operator()(const char *lhs, const char *rhs) const {
                return strcmp(lhs, rhs) < 0;
            }
        };

        explicit StringValue(const char *chars, bool takeOwnership)
                : size_(strlen(chars)) {
            if (takeOwnership) {
                data_ = const_cast<char *>(chars);
            } else {
                data_ = new char[size_ + 1];
                strcpy(data_, chars);
            }
            allValues[data_] = this;
        }

    public:
        size_t size_;
        size_t refcount_ = 1;
        char *data_;

        static std::map<const char *, StringValue *, comparer> allValues;

        static StringValue *makeStringValue(const char *chars, bool takeOwnership = false) {
            if (allValues.count(chars) == 0) {
                return new StringValue(chars, takeOwnership);
            } else {
                auto foundValue = allValues[chars];
                foundValue->refcount_++;
                return foundValue;
            }
        }

        ~StringValue() {
            allValues.erase(data_);
            delete[] data_;
        }

        StringValue &operator=(const StringValue &) = delete;

        StringValue(const StringValue &) = delete;
    };

    class StringIndexProxy final {
        MyString *owner_;
        size_t index_;
    public:
        StringIndexProxy(MyString *owner, size_t i) : owner_(owner), index_(i) {}

        operator char() const;

        StringIndexProxy &operator=(char c);

        StringIndexProxy &operator=(const StringIndexProxy &rhs);
    };

public:
    MyString(const char *chars = "") :
            value_(StringValue::makeStringValue(chars)) {}

    MyString(const MyString &other) {
        value_ = other.value_;
        value_->refcount_++;
    }

    MyString(MyString &&other) noexcept {
        value_ = other.value_;
        other.value_ = nullptr;
    }

    ~MyString() noexcept {
        dropValue();
    }

    MyString &operator=(const MyString &rhs);

    MyString &operator=(MyString &&rhs) noexcept;

    MyString operator+(const MyString &rhs) const;

    MyString &operator+=(const MyString &rhs);

    MyString &operator+=(char rhs);

    MyString operator+(char rhs) const;

    StringIndexProxy operator[](size_t i);

    bool operator==(const MyString &rhs) const;

    bool operator!=(const MyString &rhs) const;

    bool operator<(const MyString &rhs) const;

    bool operator<=(const MyString &rhs) const;

    bool operator>(const MyString &rhs) const;

    bool operator>=(const MyString &rhs) const;

    size_t length() const { return value_->size_; }

    friend std::ostream &operator<<(std::ostream &, const MyString &);

    friend MyString operator+(char, const MyString &);

private :
    StringValue *value_;

    explicit MyString(StringValue *value) : value_(value) {}

    void dropValue();

    void replaceValue(StringValue *new_ref);
};

std::istream &operator>>(std::istream &, MyString &);


#endif //NHF1_MYSTRING_H