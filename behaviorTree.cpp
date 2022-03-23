#include "main.h"
#include "behaviorTree.h"
#include "actionBase.h"
#include "nodeBase.h"
#include "execJudgmentBase.h"
#include "enemy.h"
#include "behaviorData.h"

void BehaviorTree::AddNode(std::string search_name, std::string entry_name, int priority, SELECT_RULE select_rule, ExecJudgmentBase *judgment, ActionBase *action)
{
	if (search_name != " ")
	{
		NodeBase *search_node = m_Root->SearchNode(search_name);
		NodeBase *entry_node = m_Root->SearchNode(entry_name);

		if (search_node != NULL)
		{
			NodeBase *sibling = search_node->GetLastChild();
			NodeBase *add_node = new NodeBase(entry_name, search_node, sibling, priority, select_rule, judgment, action, search_node->GetHirerchyNo() + 1);

			search_node->AddChild(add_node, entry_node);
		}
	}
	else 
	{
		if (m_Root == NULL)
		{
			m_Root = new NodeBase(entry_name, NULL, NULL, priority, select_rule, judgment, action, 1);
		}
		else {
			printf("ルートは既に登録されています\n");
		}
	}
}

void BehaviorTree::PrintTree()
{
	if (m_Root != NULL)
	{
		m_Root->PrintName();
	}
}

// 推論
NodeBase *BehaviorTree::Inference(Enemy *enemy, BehaviorData *data)
{
	// データをリセットして開始
	data->Init();
	return m_Root->Inference(enemy, data);
}

// シーケンスノードからの推論開始
NodeBase *BehaviorTree::SequenceBack(NodeBase *sequence_node, Enemy *enemy, BehaviorData *data)
{
	return sequence_node->Inference(enemy, data);
}

// ノード実行
NodeBase *BehaviorTree::Run(Enemy *enemy, NodeBase *action_node, BehaviorData *data)
{
	// ノード実行
	ActionBase::STATE state = action_node->Run(enemy);

	// 正常終了
	if (state == ActionBase::STATE::COMPLETE)
	{
		// シーケンスの途中かを判断
		NodeBase *sequence_node = data->PopSequenceNode();

		// 途中じゃないなら終了
		if (sequence_node == NULL)
		{
			return NULL;
		}
		else 
		{
			// 途中ならそこから始める
			return SequenceBack(sequence_node, enemy, data);
		}
		// 失敗は終了
	}
	else if (state == ActionBase::STATE::FAILED) 
	{
		return NULL;
	}

	// 現状維持
	return action_node;
}
