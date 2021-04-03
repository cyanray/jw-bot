#pragma once
#ifndef jw_bot_main_h_H_
#define jw_bot_main_h_H_
#include <string>
#include <sstream>
#include <mirai.h>
#include <qzjw.h>
#include <nlohmann/json.hpp>
#include <glog/logging.h>
#undef ERROR		// hack: fu*k windows.h
#include "database.h"

/**
 * @brief 帮助指令的内容
*/
const static std::string HELP_MSG =
R"([重交课表机器人使用帮助]
1. 回复 "课表"、"今天(今日)课表" 查看今天的课表
2. 回复 "明天(明日)课表" 查看明天的课表
3. 回复 "本周(上周或下周)课表" 查看一周的课表
4. 回复 "绑定学号 学号" 绑定学号
5. 回复 "教务新闻" 查看最新的教务网新闻
6. 回复 "查成绩" 查看最新的成绩
7. 回复 "查绩点" 查看各学年绩点
8. 回复 "考试安排" 查看考试安排
9. 回复 "空教室" 查看空教室✨
10. 回复 "刷新课表" 刷新错误的课表(如果发生调课) ✨
11. 其他指令: "订阅每日课表"、"订阅教务新闻"、"第几周"、"帮助"、"校车"✨、"解除绑定学号")";

/**
 * @brief 不知道学号时的友好提示信息
*/
const static std::string UNKNOWN_SCHOOL_ID_MSG =
R"(🙄我还不知道你的学号! 回复 "绑定学号+空格+学号" 启动本功能)";

/**
 * @brief 用户数据库
*/
extern Cyan::UserDatabase UserDb;

/**
 * @brief 教务新闻的数据库
*/
extern Cyan::NewsDatabase NewsDb;

/**
 * @brief Rank 功能依赖的数据库 // TODO: 移除 rank 功能
*/
extern SQLite::Database RankDb;

/**
 * @brief 教务网客户端
*/
extern cyanray::Jw JwApi;

/**
 * @brief 程序配置项
*/
extern json AppConfig;

/**
 * @brief 绑定学号指令
 * @param m 
*/
extern void CmdBinding(Cyan::Message m);

/**
 * @brief 刷新课表
 * @param m 
*/
extern void CmdRefreshCourses(Cyan::Message m);

/**
 * @brief 解除绑定学号指令
 * @param m 
*/
extern void CmdUnbinding(Cyan::Message m);

/**
 * @brief 今日课表指令
 * @param m 
*/
extern void CmdTodayCourses(Cyan::Message m);

/**
 * @brief 明日课表指令
 * @param m 
*/
extern void CmdTomorrowCourses(Cyan::Message m);

/**
 * @brief 本周课表指令
 * @param m 
*/
extern void CmdWeekCourses(Cyan::Message m);

/**
 * @brief 上周课表指令
 * @param m 
*/
extern void CmdLastWeekCourses(Cyan::Message m);

/**
 * @brief 下周课表指令
 * @param m 
*/
extern void CmdNextWeekCourses(Cyan::Message m);

/**
 * @brief 教务新闻指令
 * @param m 
*/
extern void CmdNews(Cyan::Message m);

/**
 * @brief 考试安排指令
 * @param m 
*/
extern void CmdExamSchedule(Cyan::Message m);

/**
 * @brief 查绩点指令
 * @param m 
*/
extern void CmdScore(Cyan::Message m);

/**
 * @brief 查成绩指令
 * @param m 
*/
extern void CmdExamResult(Cyan::Message m);

/**
 * @brief Rank 指令
 * @param m 
*/
extern void CmdRank(Cyan::Message m);

/**
 * @brief 运行状态指令
 * @param m 
*/
extern void CmdStatus(Cyan::Message m);

/**
 * @brief 订阅教务新闻、取消订阅教务新闻指令
 * @param m 
*/
extern void CmdSubscribeNews(Cyan::Message m);

/**
 * @brief 订阅每日课表、取消订阅每日课表指令
 * @param m 
*/
extern void CmdSubscribeMorning(Cyan::Message m);

/**
 * @brief 查空教室指令
 * @param m 
*/
extern void CmdFreeClassroom(Cyan::Message m);

/**
 * @brief 更新所有用户的课表指令(只有管理员可用)
 * @param m 
*/
extern void CmdUpdateCourses(Cyan::GroupMessage m);

/**
 * @brief 一周天气指令(从当天开始的七天)
 * @param m
*/
extern void CmdWeatherOneWeek(Cyan::Message m);

/**
 * @brief 今日天气指令
 * @param m
*/
extern void CmdWeatherOneDay(Cyan::Message m);

/**
 * @brief 每日课表计划任务
 * @param bot 
*/
extern void CronJobMorning(Cyan::MiraiBot& bot);

/**
 * @brief 教务新闻计划任务
 * @param bot 
*/
extern void CronJobNews(Cyan::MiraiBot& bot);

/**
 * @brief 自动重新登录教务网计划任务(防止 token 失效)
 * @param bot 
*/
extern void CronJobLoginJw(Cyan::MiraiBot& bot);

/**
 * @brief 获取当前的日期(yyyy-MM-dd)
 * @return 
*/
inline string GetCurrentDate()
{
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer[64];
	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer, 64, "%Y-%m-%d", timeinfo);
	return string(buffer);
}

/**
 * @brief 获取明天的日期(yyyy-MM-dd)
 * @return 
*/
inline string GetTomorrowDate()
{
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer[64];
	std::time(&rawtime);
	rawtime = rawtime + 24 * 60 * 60;	// plus one day
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer, 64, "%Y-%m-%d", timeinfo);
	return string(buffer);
}

/**
 * @brief 获取当前24小时制的小时值(Hour)
 * @return 小时数值，取值范围：0~23
*/
inline int GetHour24()
{
	char buffer[4];
	time_t timep;
	struct tm* p;
	time(&timep);
	p = localtime(&timep);
	strftime(buffer, 4, "%H", p);
	return atoi(buffer);
}

/**
 * @brief 返回今天星期几
 * @return 星期数值，取值范围：1~7
*/
inline int GetWeekToday()
{
	time_t timep;
	struct tm* p;
	time(&timep);
	p = localtime(&timep);
	int result = p->tm_wday;
	if (result == 0) return 7;
	else return result;
}

/**
 * @brief 根据配置项的开学日期计算今天的教学周次(week of semester)
 * @return 教学周次数值，取值范围：1~20
*/
inline int GetWeekOfSemester()
{
	const static time_t first_day_of_semester = AppConfig["FirstDayOfSemester"].get<int64_t>();
	time_t now;
	time(&now);
	long long diff_sec = (long long)difftime(now, first_day_of_semester);
	int week = (int)(((diff_sec) / (7 * 24 * 60 * 60)) + 1);
	if (week > 20) week = 20;
	return week;
}

/**
 * @brief 获取当前学期（教务网 API 所需格式）
 * @return 字符串，例如: "2020-2021-2"
*/
inline string GetThisSemester()
{
	return "2020-2021-2";	// TODO: calc by datetime
}

/**
 * @brief 获取上个学期（教务网 API 所需格式）
 * @return 字符串，例如: "2020-2021-1"
*/
inline string GetLastSemester()
{
	return "2020-2021-1";	// TODO: calc by datetime
}

/**
 * @brief 将成绩转换成绩点
 * @param Score 字符串格式的成绩，例如："86"、"优"、"合格"
 * @return 绩点
*/
inline double ScoreToGradePoint(const string& Score)
{
	double t = 0.0;
	std::stringstream ss;
	ss << Score;
	ss >> t;
	if (t >= 60)
	{
		t = (t / 10.0 - 5);
	}
	else if (!(t > 0))// 可能为中文成绩
	{
		if (Score == "优") t = 4.5;
		else if (Score == "良") t = 3.5;
		else if (Score == "中") t = 2.5;
		else if (Score == "及格") t = 1.5;
		else if (Score == "不及格") t = 0;
		else if (Score == "合格") t = 3.5;
		else if (Score == "不合格") t = 0;
	}
	else
	{
		t = 0.0;
	}
	return t;
}


inline Cyan::MessageChain CoursesFormat(const vector<Cyan::UserDatabase::Course>& courses)
{
	int idx = 1;
	Cyan::MessageChain mc;
	for (const auto& ele : courses)
	{
		mc
			.Plain(idx++)
			.Plain(". ")
			.Plain(ele.Name)
			.Plain("，")
			.Plain(ele.Classroom)
			.Plain("，")
			.Plain(ele.Time)
			.Plain("\n");
	}
	return mc;
}

/**
 * @brief 计算GPA
 * @param res [IN]: 考试结果
 * @param GPA [OUT]: GPA 的结果
 * @param GPA_require_only  [OUT]: 排除校选课的 GPA 的结果
*/
inline void CalcGPA(const std::vector<cyanray::Jw::ExamResult>& res, double& GPA, double& GPA_require_only)
{
	GPA = 0;
	GPA_require_only = 0;
	double credit_sum = 0, credit_sum_require_only = 0;
	for (auto&& exam : res)
	{
		if (exam.CourseCategory != "校选")
		{
			double t = ScoreToGradePoint(exam.Score);
			GPA_require_only += t * exam.Credit;
			credit_sum_require_only += exam.Credit;
		}

		double t = ScoreToGradePoint(exam.Score);
		GPA += t * exam.Credit;
		credit_sum += exam.Credit;
	}
	GPA = GPA / credit_sum;
	GPA_require_only = GPA_require_only / credit_sum_require_only;
}

#endif // !jw_bot_main_h_H_
