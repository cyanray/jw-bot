#pragma once

#include <string>
#include <stack>
#include <regex>
#include <list>
#include <iostream>
#include <vector>
#include <exception>
#include <functional>
#include <queue>
#include <unordered_map>
using std::unordered_map;
using std::queue;
using std::string;
using std::function;
using std::exception;
using std::vector;
using std::list;
using std::stack;
using std::make_pair;
using std::ostream;


// 这里是自定义异常的实现
namespace Cyan
{
	class CantFindTag : public exception
	{
	public:
		CantFindTag(const string& tagName) :msg("")
		{
			msg += "Can't find tag '";
			msg += tagName;
			msg += "' .";
		}
		const char* what() const throw ()
		{
			return msg.data();
		}
	private:
		string msg;
	};
	class CantFindAttribute : public exception
	{
	public:
		CantFindAttribute(const string& AttributeName) :msg("")
		{
			msg += "Can't find Attribute '";
			msg += AttributeName;
			msg += "' .";
		}
		const char* what() const throw ()
		{
			return msg.data();
		}
	private:
		string msg;
	};
}

// 这里放一些全局函数
namespace Cyan
{
	inline void str2lwr(string& m)
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			m[i] = (char)tolower(m[i]);
		}
	}
}

// 这里是 TokenScanner 类的实现
namespace Cyan
{
	enum class TokenType
	{
		Default,
		LeftAngleBracket,
		EndTag,
		AttributeName,
		AttributeValue,
		RightAngleBracket
	};
	struct Token
	{
		TokenType type = TokenType::Default;
		size_t offset = 0;
		string value = "";
		Token* next = nullptr;
	};
	class TokenScanner
	{
	public:
		TokenScanner(const string& content) :raw(content), root(new Token), now(nullptr) {}
		~TokenScanner()
		{
			Token* now = root, * then;
			while (now)
			{
				then = now->next;
				delete now;
				now = then;
			}
		}
		void Scan()
		{
			size_t offset = 0;
			Token* lastToken = root;
			size_t len = raw.size();
			while (true)
			{
				if (offset >= len) break;
				if (raw[offset] == '<')
				{
					if (raw[offset + 1] == '/')
					{
						lastToken->next = new Token;
						lastToken = lastToken->next;
						lastToken->type = TokenType::EndTag;
						lastToken->offset = offset;
						offset += 2;//跳过'<'和'/'这两个字符，使得raw[offset]=tagName的第一位字符
						lastToken->value = gstrExcept('>', offset);
						str2lwr(lastToken->value);
						continue;
					}
					lastToken->next = new Token;
					lastToken = lastToken->next;
					lastToken->type = TokenType::LeftAngleBracket;
					lastToken->offset = offset;
					offset += 1;//使得raw[offset]=tagName的第一位字符
					lastToken->value = gstrExcept(' ', '/', '>', offset);
					str2lwr(lastToken->value);
					while (raw[offset] != '>')
					{
						skipAll(' ', offset);
						lastToken->next = new Token;
						lastToken = lastToken->next;
						lastToken->type = TokenType::AttributeName;
						lastToken->value = gstrExcept('=', ' ', '>', offset);
						skipAll(' ', '=', offset);
						//skipAll('=', offset);

						//用单、双引号括起来的字符串里可能出现空格，为了正确分割
						//应该再再次遇到单、双引号的时候停止，而不是一刀切的在 空格、单双引号 处直接停止
						//也就是实现 将字符串视为一个整体来分割，即使字符串内包含空格（或单双引号）
						char flag = raw[offset];//当前字符是等号(=)右边的第一个非空格字符  name = ["]value"
						if (flag == '"' || flag == '\'') ++offset;//如果现在是引号，那么跳过，指向内容区
						//html code:
						///<text style="aaa" autofocus></text>
						//The attribute "autofocus" will cause a error
						//
						//the next judge is to fix this bug.
						if (flag == '>') break;

						lastToken->next = new Token;
						lastToken = lastToken->next;
						lastToken->type = TokenType::AttributeValue;
						if (flag == '"' || flag == '\'')
							lastToken->value = gstrExcept(flag, offset);
						else
							lastToken->value = gstrExcept(' ', '>', offset);
						skipAll('"', '\'', ' ', offset);
						skipAll('/', offset);
					}
				}
				if (raw[offset] == '>')
				{
					lastToken->next = new Token;
					lastToken = lastToken->next;
					lastToken->type = TokenType::RightAngleBracket;
					lastToken->offset = offset;
				}
				++offset;
			}
			now = root;
		}
		inline Token* next()
		{
			now = now->next;
			return now;
		}
	private:
		const string& raw;
		Token* root;
		Token* now;
		inline void skipExcept(char exception, size_t& offset)
		{
			while (raw[offset] != exception && raw[offset] != '\0') ++offset;
		}
		inline void skipExcept(char exceptionA, char exceptionB, size_t& offset)
		{
			while (raw[offset] != exceptionA && raw[offset] != exceptionB && raw[offset] != '\0') ++offset;
		}
		inline void skipExcept(char exceptionA, char exceptionB, char exceptionC, size_t& offset)
		{
			while (raw[offset] != exceptionA && raw[offset] != exceptionB && raw[offset] != exceptionC && raw[offset] != '\0') ++offset;
		}
		inline void skipExcept(char exceptionA, char exceptionB, char exceptionC, char exceptionD, size_t& offset)
		{
			while (raw[offset] != exceptionA && raw[offset] != exceptionB && raw[offset] != exceptionC && raw[offset] != exceptionD && raw[offset] != '\0') ++offset;
		}
		inline void skipAll(char exception, size_t& offset)
		{
			while (raw[offset] == exception && raw[offset] != '\0') ++offset;
		}
		inline void skipAll(char exceptionA, char exceptionB, size_t& offset)
		{
			while ((raw[offset] == exceptionA || raw[offset] == exceptionB) && raw[offset] != '\0') ++offset;
		}
		inline void skipAll(char exceptionA, char exceptionB, char exceptionC, size_t& offset)
		{
			while ((raw[offset] == exceptionA || raw[offset] == exceptionB || raw[offset] == exceptionC) && raw[offset] != '\0') ++offset;
		}
		inline string gstrExcept(char exception, size_t& offset)
		{
			size_t a = offset, count = 0;
			skipExcept(exception, offset);
			count = offset - a;
			return raw.substr(a, count);
			//return substr(raw + a, count);
		}
		inline string gstrExcept(char exceptionA, char exceptionB, size_t& offset)
		{
			size_t a = offset, count = 0;
			skipExcept(exceptionA, exceptionB, offset);
			count = offset - a;
			return raw.substr(a, count);
			//return substr(raw + a, count);
		}
		inline string gstrExcept(char exceptionA, char exceptionB, char exceptionC, size_t& offset)
		{
			size_t a = offset, count = 0;
			skipExcept(exceptionA, exceptionB, exceptionC, offset);
			count = offset - a;
			return raw.substr(a, count);
			//return substr(raw + a, count);
		}
		inline string gstrExcept(char exceptionA, char exceptionB, char exceptionC, char exceptionD, size_t& offset)
		{
			size_t a = offset, count = 0;
			skipExcept(exceptionA, exceptionB, exceptionC, exceptionD, offset);
			count = offset - a;
			return raw.substr(a, count);
			//return substr(raw + a, count);
		}
	};

}

// 这里是 Node 类的实现
namespace Cyan
{
	class HTMLDoc;
	class Node;
	class Attributes
	{
		friend class Node;
		friend class HTMLDoc;
	private:
		unordered_map<string, string> attr;
		Attributes() { attr.reserve(4); }
		~Attributes() = default;
		void Update(const string& name, const string& value)
		{
			attr[name] = value;
		}
	public:
		bool Exist(const string& name) const
		{
			return (attr.find(name) != attr.end());
		}
		bool Exist(const string& name, const string& value) const
		{
			if (!Exist(name)) return false;
			return (value == attr.at(name));
		}
		string GetValue(const string& name) const
		{
			if (Exist(name)) return attr.at(name);
			throw CantFindAttribute(name);
		}
	};
	class Node
	{
		friend class HTMLDoc;
	private:
		typedef function<bool(const Node*)> FilterFunc;
		Node* parent;
		Node* siblings;
		Node* child;
		string tagName;
		Attributes attributes;
		size_t txtOffset;// "[<]html>"
		size_t count;// the number of characters counted from "[<]html>" to " </html[>] "
		//创建一个子节点，返回它的指针
		Node* CreateChild()
		{
			if (child == nullptr)
			{
				child = new Node(this);
				return child;
			}
			else
			{
				Node* tNode = child;
				while (tNode->siblings != nullptr)
				{
					tNode = tNode->siblings;
				}
				tNode->siblings = new Node(this);
				return tNode->siblings;
			}
		}
		//将某节点设置为本节点的子节点
		void AddChild(Node* Child_)
		{
			if (child == nullptr)
			{
				child = Child_;
			}
			else
			{
				Node* tNode = child;
				while (tNode->siblings != nullptr)
				{
					tNode = tNode->siblings;
				}
				tNode->siblings = Child_;
			}
		}
		//移动本节点，成为目标节点的子节点
		void MoveTo(Node* dst)
		{
			//我的父节点还以为我是它的子节点，先修正
			parent->child = nullptr;
			//让dst指向自己
			dst->AddChild(this);
			//让自己指向dst
			this->parent = dst;
			// sibling节点是通过“我”这个节点访问的，就不需要重复move过去了
			// 但是需要修正 parent 的指向
			Node* tNode = siblings;
			while (tNode != nullptr)
			{
				tNode->parent = dst;
				tNode = tNode->siblings;
			}
		}
		//为本节点增加属性，返回属性的指针
		Node()
			:parent(nullptr),
			siblings(nullptr),
			child(nullptr),
			tagName(),
			txtOffset(0),
			count(0) {}
		Node(Node* parent_)
			:parent(parent_),
			siblings(nullptr),
			child(nullptr),
			tagName(),
			txtOffset(0),
			count(0) {}
		~Node() { delete siblings; delete child; }
		vector<Node*> Find_If(const FilterFunc& filter) const
		{
			vector<Node*> res;
			queue<const Node*> q;
			q.push(this);
			while (true)
			{
				if (q.empty()) break;
				const Node* tNode = q.front(); q.pop();
				if (tNode->child != nullptr)
				{
					if (filter(tNode->child))
					{
						res.push_back(const_cast<Node*>(tNode->child));
					}
					if (tNode->child->child != nullptr) q.push(tNode->child);
					tNode = tNode->child->siblings;
					while (tNode != nullptr)
					{
						if (filter(tNode))
						{
							res.push_back(const_cast<Node*>(tNode));
						}
						if (tNode->child != nullptr) q.push(tNode);
						tNode = tNode->siblings;
					}
				}
			}
			return res;
		}
	};
}

// 这里是 HTMLDoc 类的实现
namespace Cyan
{
	class HTMLDoc
	{
	public:
		typedef function<bool(const std::string & tagName, const Attributes & attributs)> SearchCondition;
		HTMLDoc() :raw(), root(nullptr), now(nullptr), use_count(new int(1)) {}
		HTMLDoc(const HTMLDoc& MLP)
		{
			++(*MLP.use_count);
			use_count = MLP.use_count;
			now = root = MLP.now;
			raw = MLP.raw;
		}
		HTMLDoc& operator=(const HTMLDoc& MLP)
		{
			++(*MLP.use_count);
			if (--(*use_count) == 0)
			{
				delete root;
				delete use_count;
			}
			use_count = MLP.use_count;
			now = root = MLP.now;
			raw = MLP.raw;
			return *this;
		}
		~HTMLDoc()
		{
			if (--(*use_count) == 0)
			{
				//delete[] raw;
				delete root;
				delete use_count;
			}
		}
		bool Parse(const string& html)
		{
			if (root != nullptr)
			{
				if (*use_count == 1)
				{
					delete root;
					delete use_count;
					use_count = new int(1);
				}
				else
				{
					--(*use_count);
				}
			}

			raw = html;

			preprocess(raw);
			//initial Scanner & Scan
			TokenScanner tokenScanner = TokenScanner(raw);
			tokenScanner.Scan();

			stack<Node*> NodeStack;
			list<Node*> NodeList;
			root = new Node();
			root->tagName = "_root_";
			Node* lNode = root;//记录上一个操作过的Node
			Node* pNode = root;//下一个新节点的父亲节点
			Node* tNode = root;//便于更新Node信息
			NodeStack.push(root);
			string lAttrName;//记录上一个操作过的Attribute的Name

			//2020/02/09 备注 : 我也不知道我一年前写的是什么，看不懂了。
			//****************************************
			//因为设计错误，不得不使用该变量记录一些信息
			//遇到LeftAngleBracket和EndTag会改变这个值（改为L_A_B或者EndTag）
			//遇到RightAngleBracket会使用这个值：
			//如果该变量(tType) == LeftAngleBracket
			//那么RightAngleBracket就修改lNode的txtOffset字段为它自身的offset
			//反之则不做任何操作
			//****************************************
			TokenType tType = TokenType::Default;

			Token* token = nullptr;
			while ((token = tokenScanner.next()) != nullptr)
			{
				switch (token->type)
				{
				case TokenType::LeftAngleBracket:
					tNode = pNode->CreateChild();//Create a new child node
					tNode->tagName = token->value;
					pNode = lNode = tNode;
					NodeStack.push(tNode);
					NodeList.push_back(tNode);
					tType = TokenType::LeftAngleBracket;//见tType声明前注释
					break;
				case TokenType::AttributeName:
					lNode->attributes.Update(token->value, "");
					lAttrName = token->value;
					break;
				case TokenType::AttributeValue:
					lNode->attributes.Update(lAttrName, token->value);
					break;
				case TokenType::EndTag:
					if (NodeStack.size() <= 1) break;
					tNode = NodeStack.top();
					NodeStack.pop();
					NodeList.pop_back();
					while (tNode->tagName != token->value)
					{
						list<Node*>::iterator it;
						for (it = NodeList.begin(); it != NodeList.end(); ++it)
						{
							if ((*it)->tagName == token->value) break;
						}
						if (it == NodeList.end())
						{
							//如果说tNode->tagName != token->value，就不会出现it == NodeList.end()
							//说明这是凭空出现的一个End_Tag
							//这个凭空出现的End_Tag不应该造成任何影响
							//所以我们把弹出来的Node再加回去
							NodeStack.push(tNode);
							NodeList.push_back(tNode);
							break;
						}
						//下面的代码用于修复parent的指向错误
						if (tNode->child != nullptr)
							tNode->child->MoveTo(tNode->parent);
						if (NodeStack.size() <= 1)
							break;
						tNode = NodeStack.top();
						NodeStack.pop();
						NodeList.pop_back();
					}
					tNode->count = token->offset - tNode->txtOffset;
					pNode = NodeStack.top();
					tType = TokenType::EndTag;//见tType声明前注释
					break;
				case TokenType::RightAngleBracket:
					if (tType == TokenType::LeftAngleBracket)//见tType变量声明的注释
					{
						lNode->txtOffset = token->offset + 1;//token->offset指向右尖括号，+1改为指向内容的第一个字符
					}
					break;
				default:
					break;
				}
			}
			now = root;
			return true;
		}
		void PrintTree(ostream& os, bool printAttributes = false)
		{
			Print(os, now, 0, printAttributes, false);
		}
		void Root()
		{
			now = root;
		}
		HTMLDoc& operator[](const string& tagName)
		{
			if (tagName == "") { return *this; }
			if (now->child != nullptr)
			{
				if (now->child->tagName == tagName)
				{
					now = now->child;
					return *this;
				}
				Node* tNode = now->child->siblings;
				while (tNode != nullptr)
				{
					if (tNode->tagName == tagName)
					{
						now = tNode;
						return *this;
					}
					tNode = tNode->siblings;
				}
			}
			throw CantFindTag(tagName);
		}
		HTMLDoc& operator[](unsigned int n)
		{
			if (n == 0) { return *this; }
			unsigned short i = 0;
			Node* tNode = now->siblings;
			while (tNode != nullptr)
			{
				if (tNode->tagName == now->tagName)
				{
					++i;
				}
				if (i == n)
				{
					now = tNode;
					return *this;
				}
				tNode = tNode->siblings;
			}
			throw CantFindTag(now->tagName + "[" + std::to_string(n) + "]");
		}
		string GetTagName()
		{
			string t = now->tagName;
			this->Root();
			return t;
		}
		string GetAttribute(const string& AttributeName)
		{
			string ps = now->attributes.GetValue(AttributeName);
			this->Root();
			return ps;
		}
		string GetContent()
		{
			using std::regex;
			string t = raw.substr(now->txtOffset, now->count);
			// string t = substr(raw + now->txtOffset, now->count);
			regex exp("<([\\s\\S]+)>");
			t = regex_replace(t, exp, "");
			this->Root();
			return t;
		}
		string GetInner()
		{
			string t = raw.substr(now->txtOffset, now->count);
			//string t =  substr(raw + now->txtOffset, now->count);
			this->Root();
			return t;
		}
		vector<HTMLDoc> Search(const SearchCondition& SC)
		{
			vector<HTMLDoc> res;
			vector<Node*> nodes = now->Find_If([&](const Node* n) { return SC(n->tagName, n->attributes); });
			for (Node* n : nodes)
			{
				HTMLDoc tHR(*this);
				tHR.SetRoot(n);
				res.push_back(std::move(tHR));
			}
			return res;
		}
		vector<HTMLDoc> SearchByTagName(const string& name)
		{
			vector<HTMLDoc> res;
			vector<Node*> nodes = now->Find_If([&](const Node* n) { return(n->tagName == name); });
			for (Node* n : nodes)
			{
				HTMLDoc tHR(*this);
				tHR.SetRoot(n);
				res.push_back(std::move(tHR));
			}
			return res;
		}
		vector<HTMLDoc> SearchByAttribute(const string& AttributeName)
		{
			vector<HTMLDoc> res;
			auto filter = [&](const Node* n)
			{
				return n->attributes.Exist(AttributeName);
			};
			vector<Node*> nodes = now->Find_If(filter);
			for (Node* n : nodes)
			{
				HTMLDoc tHR(*this);
				tHR.SetRoot(n);
				res.push_back(std::move(tHR));
			}
			return res;
		}
		vector<HTMLDoc> SearchByAttribute(const string& AttributeName, const string& AttributeValue)
		{
			vector<HTMLDoc> res;
			auto filter = [&](const Node* n)
			{
				return n->attributes.Exist(AttributeName, AttributeValue);
			};
			vector<Node*> nodes = now->Find_If(filter);
			for (Node* n : nodes)
			{
				HTMLDoc tHR(*this);
				tHR.SetRoot(n);
				res.push_back(std::move(tHR));
			}
			return res;
		}
	private:
		string raw;
		Node* root;
		Node* now;
		int* use_count;
		void Print(ostream& os, Cyan::Node* node, size_t count, bool printAttribute, bool printSiblings)
		{
			using namespace std;
			auto& cout = os;
			using std::endl;
			if (node != nullptr)
			{
				for (size_t i = 0; i < count; ++i)
				{
					cout << "..";
				}
				cout << '<' << node->tagName;
				if (printAttribute)
				{
					auto it = node->attributes.attr.begin();
					auto eit = node->attributes.attr.end();
					for (; it != eit; ++it)
					{
						cout << ' ' << it->first << "=\"" << it->second << '"';
					}
					//Attribute* tA = node->attributes;
					//while (tA != nullptr)
					//{
					//	cout << ' ' << tA->name << "=\"" << tA->value << '"';
					//	tA = tA->next;
					//}
				}
				cout << '>' << endl;
				if (node->child != nullptr)
				{
					Print(os, node->child, count + 1, printAttribute, true);
				}
				for (size_t i = 0; i < count; ++i)
				{
					cout << "..";
				}
				cout << "</" << node->tagName << '>' << endl;
				if (printSiblings && node->siblings != nullptr)
				{
					Print(os, node->siblings, count, printAttribute, printSiblings);
				}
			}
		}
		void preprocess(string& m)
		{
			//以下代码用于去除script标签、style标签与注释
			std::regex exp1("<script([\\S\\s]+?)<\\/script>");
			m = std::regex_replace(m, exp1, "");
			std::regex exp2("<!--([\\S\\s]+?)-->");
			m = std::regex_replace(m, exp2, "");
			std::regex exp3("<!([\\S\\s]+?)>");
			m = std::regex_replace(m, exp3, "");
			std::regex exp4("<style([\\S\\s]+?)<\\/style>");
			m = std::regex_replace(m, exp4, "");

			//以下代码用于去除\n \r \t
			size_t i, k;
			for (i = k = 0; raw[i] != '\0'; ++i)
			{
				if (raw[i] == '\n' || raw[i] == '\r' || raw[i] == '\t') {}
				else
				{
					raw[k] = raw[i];
					k = k + 1;
				}
			}
			raw[k] = '\0';
		}
		void SetRoot(Node* node)
		{
			now = root = node;
		}
	};
}
