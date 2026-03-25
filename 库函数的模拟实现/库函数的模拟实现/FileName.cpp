#include <cstddef>

void* my_memcpy(void* dest, const void* src, std::size_t n) {
    if (dest == nullptr || src == nullptr) return dest;
    auto* d = static_cast<unsigned char*>(dest);
    auto* s = static_cast<const unsigned char*>(src);
    for (std::size_t i = 0; i < n; ++i) d[i] = s[i];
    return dest;
}

void* my_memmove(void* dest, const void* src, std::size_t n) {
    if (dest == nullptr || src == nullptr) return dest;
    auto* d = static_cast<unsigned char*>(dest);
    auto* s = static_cast<const unsigned char*>(src);
    if (d == s || n == 0) return dest;

    if (d < s || d >= s + n) {
        for (std::size_t i = 0; i < n; ++i) d[i] = s[i];
    }
    else {
        //因为要“从后往前”复制，但 size_t 是无符号，不能写 i >= 0。所以用 i = n 开始，实际访问 i - 1。
        for (std::size_t i = n; i != 0; --i) d[i - 1] = s[i - 1];
    }
    return dest;
}

std::size_t my_strlen(const char* str) {
    if (str == nullptr) return 0;
    const char* p = str;
    while (*p != '\0') ++p;
    return static_cast<std::size_t>(p - str);
}

char* my_strcpy(char* dest, const char* src) {
    if (dest == nullptr || src == nullptr) return dest;
    char* ret = dest;

    /*src++ 先用旧值再自增。
    复制到 '\0' 时，src 最终指向字符串尾后位置；尾后指针可存在但不可解引用。
    本代码退出循环后不再访问它，因此合法*/
    while ((*dest++ = *src++) != '\0') {}

    return ret;
}

int my_strcmp(const char* s1, const char* s2) {
    // 面试里最好说明：标准库传空指针不合法，这里仅做防御处理
    if (s1 == nullptr || s2 == nullptr) {
        if (s1 == s2) return 0;
        return s1 ? 1 : -1;
    }
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

// 这是 strstr 的朴素匹配。
// 外层枚举主串起点，内层逐字符比较。
// 若模式串全部匹配完则返回当前起点。
// 空模式串返回主串，空指针返回 nullptr。
// 时间复杂度 O(n* m)，空间复杂度 O(1)。”
const char* my_strstr(const char* text, const char* pattern) {
    if (text == nullptr || pattern == nullptr) return nullptr;
    if (*pattern == '\0') return text;

    for (const char* start = text; *start != '\0'; ++start) {
        const char* text_cur = start;
        const char* pattern_cur = pattern;

        while (*text_cur != '\0' && *pattern_cur != '\0' && *text_cur == *pattern_cur) {
            ++text_cur;
            ++pattern_cur;
        }

        if (*pattern_cur == '\0') return start;
    }
    return nullptr;
}

int main() {
    // 这里可以添加一些测试代码来验证上述函数的正确性
	char dest[20];
	char src[] = "Hello, World!";
	my_strcpy(dest, src);
    return 0;
}