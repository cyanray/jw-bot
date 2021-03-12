#include <HTMLReader.h>
#include <iostream>
#include <string>
int main()
{
	using namespace std;
	using namespace Cyan;

	string htmlStr = "<html><body><img src='http://aaa.com/a.jpg' /></body></html>";

	HTMLDoc hdoc;
	hdoc.Parse(htmlStr);
	try
	{
		cout << "Img Url: ";
		cout << hdoc["html"]["body"]["img"].GetAttribute("src") << endl;
	}
	catch (const CantFindTag & cft)
	{
		cout << cft.what() << endl;
		hdoc.PrintTree(cout);
	}
	catch (const CantFindAttribute & cfa)
	{
		cout << cfa.what() << endl;
		hdoc.PrintTree(cout, true);
	}

	return 0;
}