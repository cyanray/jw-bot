#include <iostream>
#include <sstream>
#include <string_view>
#include <glog/logging.h>
#include "main.h"
#include <regex>
#include <fmt/core.h>
using namespace std;
using namespace Cyan;

void CmdMakeFriend(Message m)
{
	string msg_str = m.MessageChain.GetPlainTextFirst();
	string_view msg_view(msg_str);
	if (!msg_view.starts_with("交个朋友") && !msg_view.starts_with("交朋友")) return;
	try
	{
		LOG(INFO) << "[" << m.Sender << "] 使用 [交个朋友] 指令";
		regex pattern(R"((交个朋友|交朋友) ?([\d]+))");
		smatch match;
		regex_match(msg_str, match, pattern);
		if (match.size() < 3)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用了不正确的 [交个朋友] 指令: " << msg_str;
			m.Reply(MessageChain().Plain("没看懂你说的啥，试试：“交个朋友 <朋友的QQ号>”"));
			return;
		}

		if (UserDb.GetSid(m.Sender).empty())
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [交个朋友] 指令时出现错误: 没有找到学号!";
			m.Reply(MessageChain().Plain(UNKNOWN_SCHOOL_ID_MSG));
			return;
		}

		string friend_qq_str = match[2].str();
		QQ_t friend_qq = QQ_t(std::stoll(friend_qq_str));

		UserDb.UpdateFriendQQ(m.Sender, friend_qq);

		m.Reply(MessageChain().Plain("成功啦！让对方也和你建立好友关系吧！"));

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [交个朋友] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误！\n错误原因: \n").Plain(ex.what()));
		}
		catch (const std::exception& ex)
		{
			LOG(INFO) << "[" << m.Sender << "] 使用 [交个朋友] 指令时出现异常: " << ex.what();
		}
	}


}