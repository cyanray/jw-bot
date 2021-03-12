# 项目简介
HTMLReader 是一个可以解析 HTML(HyperText Markup Language) 文档并读取其中内容的 Header-Only 的库。

正如其名，这个库只提供了读取 HTML 文档的功能，不提供修改、输出等功能，比较适合用于爬虫类项目。

HTMLReader 大量使用STL，因经验不足可能有不当使用，如果有 BUG 还请谅解。

# 安装说明
HTMLReader 是 Header-Only 库，只有一个头文件，不需要特别安装，直接复制头文件到你的项目里即可使用。
# 使用示例

## 获取 HTML 文档的标题
以下代码将会输出：
```
Hello World!
Can't find tag 'title2' .
<head>
..<title>
..</title>
</head>
```
```C++
#include <HTMLReader.h>
#include <iostream>
#include <string>
int main()
{
	using namespace std;
	using namespace Cyan;

	// 以字符串形式保存 HTML 文档
	string htmlStr = "<html><head><title>Hello World!</title></head><body><h1>Hello HTMLReader!</h1></body></html>";

	HTMLDoc hdoc;
	// 解析 htmlStr 中的内容
	hdoc.Parse(htmlStr);

	try
	{
		// 可以使用 operator[] 来访问文档标签
		HTMLDoc head = hdoc["html"]["head"];
		// 使用 GetInner() 获取标签包含的内容
		cout << head["title"].GetInner() << endl;
		// GetInner() 会重置状态，因此变量 head 始终指向 head 标签，
		// 而不会因为上一句代码，指向 title 标签
		cout << head["title"].GetInner() << endl;
		// 下面这一行代码将会引发异常，因为 head 标签下不存在 title2 标签
		cout << head["title2"].GetInner() << endl;
	}
	catch (const CantFindTag & cft)
	{
		// 输出哪个标签找不到了
		cout << cft.what() << endl;
		// 打印解析树，看看是不是解析出错了
		hdoc.PrintTree(cout);
	}

	return 0;
}
```
更多示例请参考 **examples** 目录。

# 其他
未完待续……