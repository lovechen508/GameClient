#ifndef _INCLUDE_YCTASKPARSER_H_
#define _INCLUDE_YCTASKPARSER_H_

struct NET_TASK_MSG_NTF_T;
class YCTaskMessage;

/*
 * YCChatParser : 任务消息解析类
 *
 * 消息实例(Python)：
 *
 *      msg = """
 *			[start]
 *		talk ="<?xml version='1.0' encoding='GB2312'?>
 *			<p>
 *			<t color='ffff0000'>尘归尘，土归土！当死神降临的时候，谁都无法抗拒！
 *			</p>"
 *			1 = 'link' '活动指引' 'dlg1'
 *			2 = 'server_cmd' '传送活动地图' '1'
 *			3 = 'link' '击杀榜查询' 'dlg2'
 *			4 = 'link' '领取活动奖励' 'dlg3'
 *			5 = 'server_cmd' '离开' '2'
 *			[dlg1]
 *		talk ="活动指引"
 *			%s
 *			2 = 'link' '死神考核赛' 'dlg12'
 *			3 = 'link' '返回' 'start'
 *			[dlg11]
 *		talk =" 系统将在活动期间自动记录所有角色间的击杀记录"
 *			1 = 'link' '返回' 'start'
 *			[dlg12]
 *		talk =" 系统将在活动期间自动记录"
 *			1 = 'link' '返回' 'start'
 *			[dlg2]
 *		talk =" 排行榜"
 *			%s
 *			2 = 'server_cmd' '死神考核赛' '22'
 *			3 = 'link' '返回' 'start'
 *			[dlg3]
 *		talk =" 活动奖励"
 *			%s
 *			2 = 'server_cmd' '死神考核赛' '32'
 *			3 = 'link' '返回' 'start'
 *			"""%(GetFormatString(1), GetFormatString(2), GetFormatString(3))
 *
 */
struct YCAPI YCTaskParser
{
	//
	// 函数：parse(const NET_TASK_MSG_T* msg)
	//
	// 目的：从网络数据包构建内建消息单元
	//
	static YCTaskMessage* parse(const NET_TASK_MSG_NTF_T* msg);
};

#endif

