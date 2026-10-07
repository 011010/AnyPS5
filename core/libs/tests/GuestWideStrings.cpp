#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstddef>
#include <cstdlib>

extern "C" {
const char16_t* APS5_VABI wmemchr_nid_postfix(const char16_t* s, char16_t c, std::size_t n);
int APS5_VABI wmemcmp_nid_postfix(const char16_t* s1, const char16_t* s2, std::size_t n);
char16_t* APS5_VABI wmemcpy_nid_postfix(char16_t* dest, const char16_t* src, std::size_t n);
char16_t* APS5_VABI wmemmove_nid_postfix(char16_t* dest, const char16_t* src, std::size_t n);
std::size_t APS5_VABI wcslen_nid_postfix(const char16_t* s);
int APS5_VABI wcscmp_nid_postfix(const char16_t* s1, const char16_t* s2);
int APS5_VABI wcsncmp_nid_postfix(const char16_t* s1, const char16_t* s2, std::size_t n);
char16_t* APS5_VABI wcscpy_nid_postfix(char16_t* dest, const char16_t* src);
char16_t* APS5_VABI wcsncpy_nid_postfix(char16_t* dest, const char16_t* src, std::size_t n);
const char16_t* APS5_VABI wcschr_nid_postfix(const char16_t* s, char16_t c);
const char16_t* APS5_VABI wcsrchr_nid_postfix(const char16_t* s, char16_t c);
const char16_t* APS5_VABI wcsstr_nid_postfix(const char16_t* haystack, const char16_t* needle);
const char16_t* APS5_VABI wcspbrk_nid_postfix(const char16_t* s, const char16_t* accept);
std::size_t APS5_VABI wcsspn_nid_postfix(const char16_t* s, const char16_t* accept);
char16_t* APS5_VABI wmemset_nid_postfix(char16_t* s, char16_t c, std::size_t n);
}

namespace {

void require(bool condition) {
    if (!condition) std::abort();
}

bool same(const char16_t* left, const char16_t* right, std::size_t count) {
    for (std::size_t index = 0; index < count; ++index) {
        if (left[index] != right[index]) return false;
    }
    return true;
}

}

int main() {
    const char16_t split[] = {u'a', u'b', 0, u'c', 0};
    require(wcslen_nid_postfix(u"") == 0);
    require(wcslen_nid_postfix(u"héllo") == 5);
    require(wcslen_nid_postfix(split) == 2);

    require(wcscmp_nid_postfix(u"abc", u"abc") == 0);
    require(wcscmp_nid_postfix(u"abc", u"abd") < 0);
    require(wcscmp_nid_postfix(u"abd", u"abc") > 0);
    require(wcscmp_nid_postfix(u"ab", u"abc") < 0);
    require(wcscmp_nid_postfix(u"￿", u"a") > 0);
    require(wcscmp_nid_postfix(u"耀", u"翿") > 0);
    require(wcsncmp_nid_postfix(u"abcx", u"abcy", 3) == 0);
    require(wcsncmp_nid_postfix(u"abcx", u"abcy", 4) < 0);
    require(wcsncmp_nid_postfix(u"ab", u"ab", 10) == 0);
    require(wcsncmp_nid_postfix(u"a", u"b", 0) == 0);

    char16_t buffer[8];
    wmemset_nid_postfix(buffer, 0xaaaa, 8);
    require(wcscpy_nid_postfix(buffer, u"wide") == buffer);
    require(same(buffer, u"wide", 5) && buffer[5] == 0xaaaa);

    wmemset_nid_postfix(buffer, 0xaaaa, 8);
    require(wcsncpy_nid_postfix(buffer, u"ab", 5) == buffer);
    const char16_t padded[] = {u'a', u'b', 0, 0, 0, 0xaaaa};
    require(same(buffer, padded, 6));
    wmemset_nid_postfix(buffer, 0xaaaa, 8);
    wcsncpy_nid_postfix(buffer, u"abcdef", 3);
    const char16_t truncated[] = {u'a', u'b', u'c', 0xaaaa};
    require(same(buffer, truncated, 4));

    const char16_t* text = u"a世b世c";
    require(wcschr_nid_postfix(text, u'世') == text + 1);
    require(wcsrchr_nid_postfix(text, u'世') == text + 3);
    require(wcschr_nid_postfix(text, u'z') == nullptr);
    require(wcsrchr_nid_postfix(text, u'z') == nullptr);
    require(wcschr_nid_postfix(text, 0) == text + 5);
    require(wcsrchr_nid_postfix(text, 0) == text + 5);

    const char16_t* haystack = u"one two two";
    require(wcsstr_nid_postfix(haystack, u"two") == haystack + 4);
    require(wcsstr_nid_postfix(haystack, u"") == haystack);
    require(wcsstr_nid_postfix(haystack, u"three") == nullptr);
    require(wcsstr_nid_postfix(u"tw", u"two") == nullptr);

    require(wcspbrk_nid_postfix(haystack, u"wt") == haystack + 4);
    require(wcspbrk_nid_postfix(haystack, u"xyz") == nullptr);
    require(wcsspn_nid_postfix(u"aabbc", u"ab") == 4);
    require(wcsspn_nid_postfix(u"abc", u"") == 0);

    const char16_t units[] = {u'x', 0, u'￿', u'y'};
    require(wmemchr_nid_postfix(units, u'￿', 4) == units + 2);
    require(wmemchr_nid_postfix(units, u'y', 3) == nullptr);
    const char16_t lower[] = {u'x', 0, u'\u0001', u'y'};
    require(wmemcmp_nid_postfix(units, units, 4) == 0);
    require(wmemcmp_nid_postfix(units, lower, 4) > 0);
    require(wmemcmp_nid_postfix(lower, units, 4) < 0);
    require(wmemcmp_nid_postfix(units, lower, 2) == 0);

    char16_t copy[4] = {};
    require(wmemcpy_nid_postfix(copy, units, 4) == copy && same(copy, units, 4));
    char16_t overlap[] = {u'1', u'2', u'3', u'4', u'5'};
    require(wmemmove_nid_postfix(overlap + 1, overlap, 4) == overlap + 1);
    const char16_t shifted[] = {u'1', u'1', u'2', u'3', u'4'};
    require(same(overlap, shifted, 5));
    require(wmemset_nid_postfix(copy, u'世', 3) == copy && copy[0] == u'世' && copy[2] == u'世' && copy[3] == u'y');
    return 0;
}
