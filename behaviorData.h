#ifndef BEHAVIOR_DATA_H_
#define BEHAVIOR_DATA_H_

#include <vector>
#include <stack>
#include <map>
#include "BehaviorTree.h"

class NodeBase;
class Enemy;

// Behavior保存データ
class BehaviorData
{
public:
	BehaviorData()
	{
		Init();
	}

	// シーケンスノードのプッシュ
	void PushSequenceNode(NodeBase *node)
	{
		m_SequenceStack.push(node);
	}

	// シーケンスノードのポップ
	NodeBase *PopSequenceNode()
	{
		// 空ならNULL
		if (m_SequenceStack.empty() != 0)
		{
			return NULL;
		}

		NodeBase *node = m_SequenceStack.top();

		if (node != NULL)
		{
			// 取り出したデータを削除
			m_SequenceStack.pop();
		}

		return node;
	}

	// ノード使用判定
	bool IsNodeUsed(std::string name)
	{
		// 使用してなかったらfalse
		if (m_UsedNodeMap.count(name) == 0)
		{
			return false;
		}

		return m_UsedNodeMap[name];
	}

	// 使用済みノードに登録
	void EntryUsedNode(std::string name)
	{
		m_UsedNodeMap[name] = true;
	}

	// シーケンスステップのゲッター
	int GetSequenceStep(std::string name)
	{
		if (m_RunSequenceStepMap.count(name) == 0)
		{
			m_RunSequenceStepMap[name] = 0;
		}

		return m_RunSequenceStepMap[name];
	}

	// シーケンスステップのセッター
	void SetSequenceStep(std::string name, int step)
	{
		m_RunSequenceStepMap[name] = step;
	}

	// 初期化
	void Init()
	{
		m_RunSequenceStepMap.clear();
		while (m_SequenceStack.size() > 0)
		{
			m_SequenceStack.pop();
		}
	}

	// 使用済みノードのリセット
	void ResetNodeUsed(std::vector<NodeBase*> *reset_hierachy);

private:
	std::stack<NodeBase*> m_SequenceStack;				// シーケンスノードスタック
	std::map<std::string, int> m_RunSequenceStepMap;	// 実行シーケンスのステップマップ
	std::map<std::string, bool> m_UsedNodeMap;			// ノードの使用判定マップ
};

#endif
