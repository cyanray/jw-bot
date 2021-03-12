#include <HTMLReader.h>
#include <iostream>
#include <string>
#include <vector>
int main()
{
	using namespace std;
	using namespace Cyan;

	string htmlStr = "<html><body><div>\
<img src='http://aaa.com/a.jpg' />\
<img src='http://aaa.com/b.jpg' />\
<img src='http://aaa.com/c.jpg' />\
</div>\
<img src='http://aaa.com/d.jpg' />\
</body></html>";

	HTMLDoc hdoc;
	hdoc.Parse(htmlStr);
	HTMLDoc div = hdoc["html"]["body"]["div"];
	vector<HTMLDoc> res = div.SearchByAttribute("src");
	try
	{
		for (auto& h : res)
		{
			cout << h.GetAttribute("src") << endl;
		}
	}
	catch (const CantFindAttribute & cfa)
	{
		cout << cfa.what() << endl;
		hdoc.PrintTree(cout, true);
	}

	return 0;
}