#include <HTMLReader.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
using namespace Cyan;
int main()
{
	// Windows 调试该程序时，开启可以防止乱码
	system("chcp 65001");
	string htmlStr;
	auto f = fstream("F:/TestData/a.html");
	stringstream ss;
	if (f)
	{
		ss << f.rdbuf();
		f.close();
	}
	htmlStr = ss.str();
	//string htmlStr = "<html><head><h1 shit=fuck >AASASAS</h1><h1>FFEFEFE</h1><title shit='ooww' fuck>火火火标题!!</title></head><body><h1>Hello HTMLReader!</h1></body></html>";
	HTMLDoc hdoc;
	hdoc.Parse(htmlStr);

	try
	{
		cout << "网页标题:";
		cout << hdoc["html"]["head"]["title"].GetInner() << endl;
	}
	catch (const CantFindTag & cft)
	{
		cout << cft.what() << endl;
	}
	catch (const CantFindAttribute & cfa)
	{
		cout << cfa.what() << endl;
	}

	//ml.PrintTree(cout, true);

	auto res = hdoc.Search(
		[](const string& tagName, const Attributes& attrs)
		{
			return (tagName == "span" && attrs.Exist("class", "post-title"));
		});

	//auto res = hdoc.SearchByAttribute("class", "ds-1col node node-article view-mode-realtime_listing  clearfix");
	int i = 1;
	for (auto& m : res)
	{
		cout << i++<<":\t";
		cout << m.GetInner();
		cout << endl;
	}


	cin.get();
	return 0;
}