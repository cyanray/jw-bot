#include <iostream>
#include "main.h"
//#include <string>

using namespace std;
using namespace Cyan;
//using namespace string;

void CmdStatus(Cyan::Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "运行状态") return;

	try
	{
		auto mc = MessageChain().Plain("[课表机器人运行状态]\n");
		mc.Plain("总用户数量: ").Plain(UserDb.GetUserCountAll()).Plain(" 人\n");
		mc.Plain("用户数量(17): ").Plain(UserDb.GetUserCount_17()).Plain(" 人\n");
		mc.Plain("用户数量(18): ").Plain(UserDb.GetUserCount_18()).Plain(" 人\n");
		mc.Plain("用户数量(19): ").Plain(UserDb.GetUserCount_19()).Plain(" 人\n");
		mc.Plain("用户数量(20): ").Plain(UserDb.GetUserCount_20()).Plain(" 人");
		m.Reply(mc);
	}
	catch (const std::exception& ex)
	{
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (...)
		{
		}
	}

	//string url = "http://www.weather.com.cn/weather/101040500.shtml";
	//HTTP http;
	//auto resp = http.Get(url);
	//if (!resp.Ready) throw runtime_error("请求无响应.");
	//if (resp.StatusCode != 200) throw runtime_error("返回非 200 状态码.");
	
	
	//HTMLDoc hdoc;
	//hdoc.Parse(resp.Content);


	//auto res = hdoc.Search(
	//	[](const string& tagName, const Attributes& attrs)
	//	{
	//		return (tagName == "ul" && attrs.Exist("class", "t clearfix"));
	//	});

	////auto res = hdoc.SearchByAttribute("class", "ds-1col node node-article view-mode-realtime_listing  clearfix");
	//
	//try {
	//	//实际上只有一个ul
	//	for (auto& ul : res)
	//	{
	//		//cout << m.GetInner() << endl;

	//		auto theLies = ul.SearchByTagName("li");

	//		for (auto& li : theLies) {
	//			auto mc = MessageChain();
	//			//cout << li.GetInner() << endl<<endl;
	//			cout << "日期:" << li["h1"].GetInner() << "  ";   //日期
	//			mc.Plain("日期：").Plain(li["h1"].GetInner()).Plain("\n");

	//			auto ps = li.SearchByTagName("p");
	//			cout << "天气:" << ps[0].GetInner() << "  ";
	//			mc.Plain("天气：").Plain(ps[0].GetInner()).Plain("\n");

	//			string minTem = ps[1]["i"].GetInner();
	//			auto maxTemH = ps[1].SearchByTagName("span");
	//			if (maxTemH.empty())
	//			{
	//				cout << "平均温度:" << minTem << endl;
	//				mc.Plain("平均温度：").Plain(minTem).Plain("\n");
	//			}
	//			else
	//			{
	//				cout << "温度:" << minTem << "-" << maxTemH[0].GetInner() << endl;
	//				mc.Plain("温度：").Plain(minTem).Plain("-").Plain(maxTemH[0].GetInner()).Plain("\n");
	//			}

	//			m.Reply(mc);

	//		}


	//		cout << endl;
	//	}
	//}
	//catch(const CantFindAttribute & cfa){
	//	LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [查天气] 指令时出现异常: " << cfa.what();
	//	try
	//	{
	//		m.Reply(MessageChain().Plain("出现错误："s + cfa.what()));
	//	}
	//	catch (const exception& ex)
	//	{
	//		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [查天气] 指令时出现异常: " << ex.what();
	//	}

	//	hdoc.PrintTree(cout,true);
	//}

	MessageChain mcc;
	auto weathers =  JwApi.GetWeather();
	cout <<"wea size:"<< weathers.size() << endl;
	for (size_t i=0;i<weathers.size();i++)
	{
		//mcc.Plain("地点：").Plain(weathers[i].position).Plain("\n");
		string ss = "科学城";
		cout << "滴答滴答：" << ss << endl;
		cout << "地点：" << weathers[i].position << endl;
		mcc.Plain("日期：").Plain(weathers[i].date).Plain("\n");
		mcc.Plain("天气：").Plain(weathers[i].weather).Plain("\n");
		if (weathers[i].maxTem == "None")
		{
			mcc.Plain("最小温度：").Plain(weathers[i].minTem);
		}
		else
		{
			mcc.Plain("温度：").Plain(weathers[i].minTem).Plain("-").Plain(weathers[i].maxTem);
		}
		
		m.Reply(mcc);
		mcc.Clear();
	}

}