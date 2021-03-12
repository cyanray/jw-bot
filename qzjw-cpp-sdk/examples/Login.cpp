#include <iostream>
#include <qzjw.h>

int main()
{
#if defined(_WIN32) || defined(WIN32)
	system("chcp 65001");
#endif
	using namespace std;
	using namespace cyanray;

	Jw jw("http://jwgl.cqjtu.edu.cn/");

	string uid, pwd;
	cout << "请输入用户名和密码: " << endl;
	cin >> uid >> pwd;
	try
	{
		jw.Login(uid, pwd);
		cout << "登录成功, token: " << jw.GetToken() << endl;
	}
	catch (const exception& ex)
	{
		cout << "登录出错: " << ex.what() << endl;
	}

	try
	{
		auto res = jw.GetCourses(uid, 1);
		for (auto&& c : res)
		{
			cout << c.Name << endl;
		}

	}
	catch (const exception& ex)
	{
		cout << "出现错误: " << ex.what() << endl;
	}


	try
	{
		auto res = jw.GetExamResult(uid);
		for (auto&& c : res)
		{
			cout << c.Name << endl;
		}

	}
	catch (const exception& ex)
	{
		cout << "出现错误: " << ex.what() << endl;
	}


	return 0;
}