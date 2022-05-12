#include <iostream>
#include "mystring.h"

int main() {
    {
        MyString empty{};
        MyString alpha("alpha");
        MyString alpha2 = alpha;
        alpha2 += '2';
        MyString alpha23four = (alpha2 += "3") + "four";
        std::cout << alpha << " " << alpha2 << " " << alpha23four << std::endl;
    }
    {
        MyString abc123 = "abc123";
        MyString abc124 = abc123;
        abc124[5] = '4';
        abc123[0] = abc124[5];
        abc123[1] = abc123[0];
        std::cout << 'x' + abc123 << " " << abc124 << std::endl;
    }
    {
        MyString s = "mivan";
        for (size_t i = 0; i < s.length(); ++i) {
            std::cout << s[i];
        }
        std::cout << std::endl;
    }
    {
        MyString a = "hello";
        MyString b = a;
        std::cout << b[1];          /* e */

        a[1] = 'a';
        b[1] = b[4];
        std::cout << a << " " << b; /* hallo hollo */
    }
    {
        MyString a = "hello world";
        MyString b = "hello world";
    }
//    {
//        MyString empty;
//        std::cout << "your string: ";
//        std::cin >> empty;
//        std::cout << empty << std::endl;
//        std::cout << (empty == empty + "end") << " " << ((empty += 'a') > MyString()) << std::endl;
//    }
}