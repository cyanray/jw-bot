#pragma once
#ifndef jw_bot_course_database_h_H_
#define jw_bot_course_database_h_H_
#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using namespace nlohmann;

const static string DB_FILE_NAME = "jwbot.db";

namespace Cyan
{
	class UserDatabase
	{
	public:
		UserDatabase() :db_(DB_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
		{
		}

		~UserDatabase() = default;

		struct Course
		{
			string Name;
			string Classroom;
			string StartTime;
			string EndTime;
			string Time;
		};

		int CourseTimeConvert(const string& time)
		{
			if (time == "08:20") return 1;
			if (time == "10:00") return 2;
			if (time == "10:20") return 3;
			if (time == "12:00") return 4;
			if (time == "14:00") return 5;
			if (time == "15:40") return 6;
			if (time == "16:00") return 7;
			if (time == "17:40") return 8;
			if (time == "19:00") return 9;
			if (time == "20:40") return 10;
			if (time == "21:00") return 11;
			if (time == "22:40") return 12;
			return 0;
		}

		string CourseTimeConvert(int time)
		{
			static string time_list[12] =
			{
				"08:20",
				"10:00",
				"10:20",
				"12:00",
				"14:00",
				"15:40",
				"16:00",
				"17:40",
				"19:00",
				"20:40",
				"21:00",
				"22:40"
			};
			return time_list[time - 1];
		}

		int Add(QQ_t qq)
		{
			SQLite::Statement sql(db_, "INSERT INTO user_info (qq) VALUES (?)");
			sql.bind(1, qq.ToInt64());
			return sql.exec();
		}

		int Add(QQ_t qq, const string& sid)
		{
			SQLite::Statement sql(db_, "INSERT INTO user_info (qq,sid) VALUES (?,?)");
			sql.bind(1, qq.ToInt64());
			sql.bind(2, sid);
			return sql.exec();
		}

		bool Exist(QQ_t qq)
		{
			SQLite::Statement sql(db_, "SELECT sid FROM user_info WHERE qq = ?");
			sql.bind(1, qq.ToInt64());
			if (sql.executeStep()) return true;
			else return false;
		}

		string GetSid(QQ_t qq)
		{
			SQLite::Statement sql(db_, "SELECT sid FROM user_info WHERE qq = ?");
			sql.bind(1, qq.ToInt64());
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else
			{
				return string();
			}
		}

		int UpdateSid(QQ_t qq, const string& sid)
		{
			SQLite::Statement sql(db_, "UPDATE user_info SET sid = ? WHERE qq = ?");
			sql.bind(1, sid);
			sql.bind(2, qq.ToInt64());
			return sql.exec();
		}

		int ClearCourses(QQ_t qq)
		{
			SQLite::Statement sql(db_, "DELETE FROM courses where qq = ?;");
			sql.bind(1, qq.ToInt64());
			return sql.exec();
		}

		int Delete(QQ_t qq)
		{
			int t = ClearCourses(qq);
			SQLite::Statement sql(db_, "UPDATE user_info SET sid = null, news_subscriber = 0,morning_subscriber = 0  WHERE qq = ?");
			sql.bind(1, qq.ToInt64());
			return t + sql.exec();
		}

		vector<QQ_t> GetAllUser()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "SELECT qq FROM user_info WHERE sid IS NOT NULL");
			while (sql.executeStep())
			{
				QQ_t c = (QQ_t)((int64_t)(sql.getColumn(0)));
				res.push_back(c);
			}
			return res;
		}

		int GetUserCountAll()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "select COUNT(sid) from user_info");
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else return -1;
		}

		int GetUserCount_17()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "select COUNT(sid) from user_info where sid like '6317%'");
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else return -1;
		}

		int GetUserCount_18()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "select COUNT(sid) from user_info where sid like '6318%'");
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else return -1;
		}

		int GetUserCount_19()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "select COUNT(sid) from user_info where sid like '6319%'");
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else return -1;
		}

		int GetUserCount_20()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "select COUNT(sid) from user_info where sid like '6320%'");
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else return -1;
		}

		int GetUserCount_21()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "select COUNT(sid) from user_info where sid like '6321%'");
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else return -1;
		}

		void UpdateNewsSubscription(QQ_t qq, bool subscription)
		{
			SQLite::Statement sql(db_, "UPDATE user_info SET news_subscriber = ? WHERE qq = ?");
			sql.bind(1, (int)(subscription));
			sql.bind(2, qq.ToInt64());
			sql.exec();
		}

		void UpdateMorningSubscription(QQ_t qq, bool subscription)
		{
			SQLite::Statement sql(db_, "UPDATE user_info SET morning_subscriber = ? WHERE qq = ?");
			sql.bind(1, (int)(subscription));
			sql.bind(2, qq.ToInt64());
			sql.exec();
		}

		vector<QQ_t> GetNewsSubscribers()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "SELECT qq FROM user_info Where news_subscriber = 1");
			while (sql.executeStep())
			{
				QQ_t c = (QQ_t)((int64_t)(sql.getColumn(0)));
				res.push_back(c);
			}
			return res;
		}

		vector<QQ_t> GetMorningSubscribers()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_,
				R"(
SELECT DISTINCT courses.qq
FROM courses
INNER JOIN user_info
ON user_info.qq = courses.qq
WHERE user_info.morning_subscriber = 1
ORDER BY courses.start_time;
)");
			while (sql.executeStep())
			{
				QQ_t c = (QQ_t)((int64_t)(sql.getColumn(0)));
				res.push_back(c);
			}
			return res;
		}

		vector<QQ_t> GetMorningSubscribers(int week_of_semester, int week)
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, 
R"(
SELECT DISTINCT courses.qq
FROM courses
INNER JOIN user_info
ON user_info.qq = courses.qq
WHERE user_info.morning_subscriber = 1 AND courses.week_of_semester=? AND courses.week=?
ORDER BY courses.start_time;
)");
			sql.bind(1, week_of_semester);
			sql.bind(2, week);
			while (sql.executeStep())
			{
				QQ_t c = (QQ_t)((int64_t)(sql.getColumn(0)));
				res.push_back(c);
			}
			return res;
		}

		int InsertCourse(QQ_t qq,
			const string& name,
			const string& classroom,
			const string& startTime,
			const string& endTime,
			int week_of_semester,
			int week)
		{

			SQLite::Statement sql(db_,
				R"(

INSERT INTO courses 
(qq, name, classroom, start_time, end_time, week_of_semester, week) 
VALUES 
(?,?,?,?,?,?,?);
)");
			sql.bind(1, qq.ToInt64());
			sql.bind(2, name);
			sql.bind(3, classroom);
			sql.bind(4, startTime);
			sql.bind(5, endTime);
			sql.bind(6, week_of_semester);
			sql.bind(7, week);
			return sql.exec();
		}

		vector<Course> GetCourses(QQ_t qq, int week_of_semester, int week)
		{
			SQLite::Statement sql(db_,
				R"(
SELECT name, classroom, start_time, end_time 
FROM courses 
WHERE qq = ? AND week_of_semester = ? AND week = ?;
)");
			sql.bind(1, qq.ToInt64());
			sql.bind(2, week_of_semester);
			sql.bind(3, week);

			vector<Course> result;
			while (sql.executeStep())
			{
				Course c
				{
					.Name = sql.getColumn(0),
					.Classroom = sql.getColumn(1),
					.StartTime = sql.getColumn(2),
					.EndTime = sql.getColumn(3),
				};
				c.Time = c.StartTime + "-" + c.EndTime;
				result.push_back(c);
			}
			return result;
		}

	private:
		SQLite::Database db_;
	};

	struct News
	{
		string title;
		string url;
		string time;
	};

	class NewsDatabase
	{
	public:
		NewsDatabase() :db_(DB_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
		{

		}

		void Add(const string& title, const string& date, const string& url)
		{
			SQLite::Statement sql(db_, "INSERT INTO news (title, date, url) VALUES (?,?,?)");
			sql.bind(1, title);
			sql.bind(2, date);
			sql.bind(3, url);
			sql.exec();
		}


		bool Exist(const string& title)
		{
			SQLite::Statement sql(db_, "SELECT title FROM news WHERE title = ?");
			sql.bind(1, title);
			if (sql.executeStep()) return true;
			else return false;
		}

		vector<News> GetLatestNews()
		{
			vector<News> res;
			SQLite::Statement sql(db_, "SELECT title, date, url FROM news ORDER BY date DESC limit 5");
			while (sql.executeStep())
			{
				News n;
				string title = sql.getColumn(0);
				string time = sql.getColumn(1);
				string url = sql.getColumn(2);
				n.title = title;
				n.time = time;
				n.url = url;
				res.emplace_back(n);
			}
			return res;
		}

		~NewsDatabase() = default;

	private:
		SQLite::Database db_;
	};



} // namespace Cyan

#endif // !jw_bot_course_database_h_H_