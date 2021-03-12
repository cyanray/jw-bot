#pragma once
#ifndef CJKB__keywords_parser_h_H_
#define CJKB__keywords_parser_h_H_
#include <string>
#include <vector>
#include <algorithm>
using std::string;
using std::vector;

namespace Cyan
{
	typedef vector<string> AlterKeywords;
	class KeywordParser
	{
	public:
		KeywordParser(const string& words) :words_(words), result_(true), pos_(0)
		{
			Trim(words_);
		}
		bool GetResult() const { return result_; }
		KeywordParser& With(const string& keyword)
		{
			if (result_ == false) return *this;
			With(keyword, pos_);
			return *this;
		}
		KeywordParser& StartWith(const string& keyword)
		{
			if (result_ == false) return *this;
			if (words_.rfind(keyword, 0) != string::npos)
			{
				result_ = true;
				pos_ = keyword.size();
			}
			else result_ = false;
			return *this;
		}
		KeywordParser& AndWith(const string& keyword)
		{
			if (result_ == false) return *this;
			With(keyword, pos_);
			return *this;
		}
		KeywordParser& AndWith(const vector<string>& keywords)
		{
			if (result_ == false) return *this;
			for (const string& keyword : keywords)
			{
				With(keyword, pos_);
				if (result_ == true) break;
			}
			return *this;
		}
		KeywordParser& Equals(const string& str)
		{
			if (result_ == false) return *this;
			result_ = (words_ == str);
			return *this;
		}
		KeywordParser& Equals(const vector<string>& strs)
		{
			if (result_ == false) return *this;
			for (const string& s : strs)
			{
				result_ = (words_ == s);
				if (result_) break;
			}
			return *this;
		}
		KeywordParser& EndWith(const string& str)
		{
			if (result_ == false) return *this;
			WordsEndWith(str);
			return *this;
		}
		KeywordParser& EndWith(const vector<string>& strs)
		{
			if (result_ == false) return *this;
			for (const string& s : strs)
			{
				WordsEndWith(s);
				if (result_) break;
			}
			return *this;
		}
		~KeywordParser() = default;
	private:
		size_t pos_;
		string words_;
		bool result_;

		KeywordParser& With(const string& keyword, size_t pos = 0)
		{
			if (words_.find(keyword, pos) != string::npos)
			{
				result_ = true;
				pos_ += pos + keyword.size();
			}
			else result_ = false;
			return *this;
		}

		KeywordParser& WordsEndWith(const string& str)
		{
			if (str.size() > words_.size()) result_ = false;
			result_ = std::equal(str.rbegin(), str.rend(), words_.rbegin());
			return *this;
		}

		void Trim(string& s)
		{
			if (s.empty()) return;
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
		}

	};

} // namespace Cyan

#endif // !CJKB__keywords_parser_h_H_