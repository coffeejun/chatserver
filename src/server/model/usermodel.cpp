#include "usermodel.h"
#include "db.h"
#include <iostream>

bool UserModel::insert(User &user)
{
    // 组装sql语句
    char sql[1024] = {0};
    snprintf(sql, sizeof(sql), "insert into user(name, password, state) values('%s', '%s', '%s')",
             user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str());

    MySQL mysql;         // 定义一个mysql对象
    if (mysql.connect()) // mysql连接
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(mysql.getConnection()));
            // mysql_insert_id()返回给定的 link_identifier中上一步 INSERT 查询中产生的 AUTO_INCREMENT 的 ID 号。
            return true;
        }
    }

    return false;
}

// 根据用户号码查询用户信息
User UserModel::query(int id)
{
    char sql[1024] = {0};
    snprintf(sql, sizeof(sql), "select * from user where id = %d", id);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            // 根据主键id进行查询
            // mysql_fetch_row() 从和结果标识 data 关联的结果集中取得一行数据并作为数组返回。
            if (row != nullptr)
            {
                // 生成一个User对象，填入信息
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                mysql_free_result(res); // 释放资源
                return user;
            }
        }
    }

    // 返回空User
    return User();
}

bool UserModel::updateState(User user)
{
    char sql[1024] = {0};
    snprintf(sql, sizeof(sql), "update user set state = '%s' where id =%d", user.getState().c_str(), user.getId());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

// 重置用户的状态信息
void UserModel::resetState()
{
    // 1.组装sql语句
    char sql[1024] = "update user set state = 'offline' where state = 'online'";

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}