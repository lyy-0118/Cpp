#include "string.h"
namespace lyy {
	void string::reserve(size_t n) {
		if (n > _capacity) {
			//多开一个空间用于存放\0
			char* tmp = new char[n + 1];
			strcpy(tmp, _str);
			_capacity = n;
			_str = tmp;
		}
	}
	void string::push_back(char c) {
		if (_size == _capacity) { //扩容
			reserve(_capacity == 0 ? 4 : 2 * _capacity);
		}
		_str[_size] = c;
		++ _size;
		_str[_size] = '\0';
	}
	string& string::append(const string& s) {
		size_t len = s._size; 
		if (_capacity-_size<len) { //剩余空间不够存放新字符串 进行扩容

			// 大于2倍，需要多少开多少，小于2倍按2倍扩
			reserve(_size + len > 2 * _capacity ? _size + len : 2 * _capacity);
		}
		size_t i = _size, j = 0;
		for (; i < _size + len; i++,j++) {
			_str[i] = s._str[j];
		}
		_str[i] = '\0';
		return *this;
	}
	void string::append(const char* s) {
		size_t len =strlen(s);
		if (_capacity - _size < len) { //剩余空间不够存放新字符串 进行扩容

			// 大于2倍，需要多少开多少，小于2倍按2倍扩
			reserve(_size + len > 2 * _capacity ? _size + len : 2 * _capacity);
		}
		strcpy(_str + _size, s);
		_size += len;
	}
	string& string::operator+=(char c) {
		push_back(c);
		return *this;
	}
	string& string::operator+=(const char* s) {
		append(s);
		return *this;
	}
	void string::resize(size_t n, char c) {
		if (n > _capacity) {
			reserve(n);
		}
		size_t len = _size;
		while (len < n&&c!='\0') {
			_str[len++] = c;
		}
		_str[len] = '\0';
		_size = strlen(_str);
	}
	void string::erase(size_t pos, size_t len) {
		assert(pos >=0 && pos < _size);
		if (len >= _size - pos) {  //长度已经超过剩余字符串长度
			_str[pos] = '\0';
			_size = pos;
		}
		else {
			size_t start = pos, end = pos + len;
			//从后向前覆盖
			while (end < _size) {
				_str[start++] = _str[end++];
			}
			_str[start] = '\0';
			_size-=len;
		}
	}
	void string::insert(char c, size_t pos){
		assert(pos <= _size);
		if (_size == _capacity) { //扩容
			reserve(_capacity == 0 ? 4 : 2 * _capacity);
		}
		//整体后移一位
		size_t end = _size;
		while (end>pos) {
			_str[end] = _str[end - 1];
			--end;
		}
		_str[pos] = c;
		_str[++_size] = '\0';
	}
	void string::insert(const char* s, size_t pos){
		assert(pos <= _size);
		size_t len = strlen(s);
		if (_size+len>_capacity) { //扩容
			reserve(_size+len>2*_capacity?_size+len:2*_capacity);
		}
		//整体后移len位
		size_t end = _size+len;
		while (end >= pos+len) {
			_str[end] = _str[end - len];
			--end;
		}
		//插入字符串
		for (size_t i = 0; i < len; i++)
		{
			_str[pos + i] = s[i];
		}
		_size += len;
		
	}
	size_t string::find(char ch, size_t pos){
		if (pos < _size) {
			return npos;
		}
		size_t i = pos;
		for (; i < _size; i++) {
			if (_str[i] == ch) {
				return i;
			} 
		}
		return npos;
	}
	size_t string::find(const char* s, size_t pos){
		if (pos < _size) {
			return npos;
		}
		const char*ptr = strstr(_str+pos, s);
		if (ptr == nullptr) {
			return npos;
		}
		else {
			return ptr - _str; //减去首地址 得到位置下标
		}
	}
	string string::substr(size_t pos, size_t len){  //返回从pos位置开始 长度为len的字串
		assert(pos < _size);
		if (pos + len > _size) {
			//大于剩余字符长度 更新一下长度
			len = _size - pos;
		}
		string newstring;
		newstring.reserve(len);
		size_t i = pos;
		for (; i < pos + len; i++) {
			newstring += _str[i];
		}
		return newstring;
	}

	bool operator<(const string& s1, const string& s2)
	{
		return strcmp(s1.c_str(), s2.c_str()) < 0;
	}
	bool operator<=(const string& s1, const string& s2)
	{
		return s1 < s2 || s1 == s2;
	}
	bool operator>(const string& s1, const string& s2)
	{
		return !(s1 <= s2);
	}
	bool operator>=(const string& s1, const string& s2)
	{
		return !(s1 < s2);
	}
	bool operator==(const string& s1, const string& s2)
	{
		return strcmp(s1.c_str(), s2.c_str()) == 0;
	}
	bool operator!=(const string& s1, const string& s2)
	{
		return !(s1 == s2);
	}
	ostream& operator<<(ostream& out, const string& s)
	{
		for (auto ch : s)
		{
			out << ch;
		}

		return out;
	}
	istream& operator>>(istream& in, string& s)
	{
		s.clear(); //清除字符串的内容
		const int N = 256;
		char buff[N];  //给一个缓冲字符数组 以免重复reserve空间
		int i = 0;
		char ch;
		//in >> ch;   //直接in>>字符会跳过空格
		ch = in.get();
		while (ch != ' ' && ch != '\n')
		{
			buff[i++] = ch;
			if (i == N - 1)
			{
				buff[i] = '\0';
				s += buff;

				i = 0;
			}
			//in >> ch;
			ch = in.get();
		}
		if (i > 0)
		{
			buff[i] = '\0';
			s += buff;
		}
		return in;
	}
}