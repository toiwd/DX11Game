#include "main.h"
#include <iostream>
#include "ExecJudgmentBase.h"
#include "NodeBase.h"
#include "Enemy.h"
#include "BehaviorData.h"
#include "ActionBase.h"

// ノード検索
NodeBase *NodeBase::SearchNode(std::string search_name)
{
	// 名前が一致
	if (m_Name == search_name)
	{
		return this;
	}
	else 
	{
		// 子ノードで検索
		for (auto itr = m_Child.begin(); itr != m_Child.end(); itr++)
		{
			NodeBase *ret = (*itr)->SearchNode(search_name);

			if (ret != NULL)
			{
				return ret;
			}
		}
	}

	return NULL;
}

// 名前表示
void NodeBase::PrintName()
{
	std::cout << m_Name << std::endl;

	for (auto itr = m_Child.begin(); itr != m_Child.end(); itr++)
	{
		(*itr)->PrintName();
	}
}

// ノード推論
NodeBase *NodeBase::Inference(Enemy *enemy, BehaviorData *data)
{
	std::vector<NodeBase*> list;
	NodeBase *result = NULL;

	// 子ノードで実行可能なノードを探す
	for (int i = 0; i < m_Child.size(); i++)
	{
		if (m_Child[i]->m_ExecJudgment != NULL)
		{
			if (m_Child[i]->m_ExecJudgment->Judgment(enemy) == true)
			{
				list.push_back(m_Child[i]);
			}
		}
		else 
		{
			// 判定クラスがなければ無条件に追加
			list.push_back(m_Child[i]);
		}
	}

	// 選択ルールでノード決め
	switch (m_SelectRule)
	{
		// 優先順位
	case BehaviorTree::SELECT_RULE::PRIORITY:
		result = SelectPriority(&list);
		break;
		// オン・オフ
	case BehaviorTree::SELECT_RULE::ON_OFF:
		result = SelectOnOff(&list, data);
		break;
		// ランダム
	case BehaviorTree::SELECT_RULE::RANDOM:
		result = SelectRandom(&list);
		break;
		// シーケンス
	case BehaviorTree::SELECT_RULE::SEQUENCE:
	case BehaviorTree::SELECT_RULE::SEQUENTIAL_LOOPING:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != NULL)
	{
		// 行動があれば終了
		if (result->HasAction() == true)
		{
			return result;
		}
		else 
		{
			// 決まったノードで推論開始
			result = result->Inference(enemy, data);
		}
	}

	return result;
}

// 優先順位選択
NodeBase *NodeBase::SelectPriority(std::vector<NodeBase*> *list)
{
	NodeBase *select_node = NULL;
	int priority = -10000;

	// 一番優先順位が高いノードを探す
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (priority < (*itr)->GetPriority())
		{
			select_node = (*itr);
			priority = (*itr)->GetPriority();
		}
	}

	return select_node;
}

// オン・オフ
NodeBase *NodeBase::SelectOnOff(std::vector<NodeBase*> *list, BehaviorData *data)
{
	std::vector<NodeBase*> off_list;

	// 使用済みノードリストアップする
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (data->IsNodeUsed((*itr)->GetName()) == false)
		{
			off_list.push_back(*itr);
		}
	}

	// リストアップした数チェック
	if (off_list.size() == 0)
	{
		// ノードを全てリセット
		data->ResetNodeUsed(&m_Child);
		off_list = *list;
	}

	// 使用したノードを登録
	data->EntryUsedNode(off_list[0]->GetName());

	// リストの先頭が決定ノード
	return off_list[0];
}

// ランダム
NodeBase *NodeBase::SelectRandom(std::vector<NodeBase*> *list)
{
	int select_no = rand() % list->size();

	return (*list)[select_no];
}

// シーケンス
NodeBase *NodeBase::SelectSequence(std::vector<NodeBase*> *list, BehaviorData *data)
{
	// 今のノードのシーケンスのステップを取得
	int step = data->GetSequenceStep(GetName());

	// シーケンスが終わってたら終了
	if (step >= m_Child.size())
	{
		if (m_SelectRule != BehaviorTree::SELECT_RULE::SEQUENTIAL_LOOPING)
		{
			return NULL;
		}
		else
		{
			// Loopingの場合は頭から実行
			step = 0;
		}
	}

	// 順番のノードが実行できるかのチェック
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (m_Child[step]->GetName() == (*itr)->GetName())
		{
			// シーケンスノードを記録
			data->PushSequenceNode(this);
			// シーケンスステップを更新
			data->SetSequenceStep(GetName(), step + 1);
			return m_Child[step];
		}
	}

	return NULL;
}

// 判定
bool NodeBase::Judgment(Enemy *enemy)
{
	//　判定クラスを持っていれば実効なければtrue扱い
	if (m_ExecJudgment != NULL)
	{
		return m_ExecJudgment->Judgment(enemy);
	}

	return true;
}

// ノード実行
ActionBase::STATE NodeBase::Run(Enemy *enemy)
{
	// アクションノードがあれば実行して結果を返す、なければ失敗扱い
	if (m_Action != NULL)
	{
		return m_Action->Run(enemy);
	}

	return ActionBase::FAILED;
}
