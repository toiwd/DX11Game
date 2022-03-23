#ifndef NODE_BASE_H_
#define NODE_BASE_H_

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

class ExecJudgmentBase;
class BehaviorData;

// ノード
class NodeBase
{
public:
	NodeBase(std::string name, NodeBase *parent, NodeBase *sibling, int priority, BehaviorTree::SELECT_RULE select_rule, ExecJudgmentBase *exec_judgment, ActionBase *action, int hierarchy_no) :
		m_Name(name),
		m_Parent(parent),
		m_Sibling(sibling),
		m_Priority(priority),
		m_SelectRule(select_rule),
		m_ExecJudgment(exec_judgment),
		m_Action(action),
		m_HierarchyNo(hierarchy_no),
		m_Child(NULL)
	{
	}

	// 名前ゲッター
	std::string GetName()
	{
		return m_Name;
	}

	// 親ノードゲッター
	NodeBase *GetParent()
	{
		return m_Parent;
	}

	// 子ノードゲッター
	NodeBase* GetChild(int idx)
	{
		if (m_Child.size() >= idx)
		{
			return NULL;
		}

		return m_Child[idx];
	}

	// 子ノードゲッター(末尾)
	NodeBase *GetLastChild()
	{
		if (m_Child.size() == 0)
		{
			return NULL;
		}

		return m_Child[m_Child.size() - 1];
	}

	// 子ノードゲッター(先頭)
	NodeBase *GetTopChild()
	{
		if (m_Child.size() == 0)
		{
			return NULL;
		}

		return m_Child[0];
	}

	// 兄弟ノードゲッター
	NodeBase *GetSibling(void)
	{
		return m_Sibling;
	}

	// 階層番号ゲッター
	int GetHirerchyNo()
	{
		return m_HierarchyNo;
	}

	// 優先順位ゲッター
	int GetPriority()
	{
		return m_Priority;
	}

	// 親ノードセッター
	void SetParent(NodeBase *parent)
	{
		m_Parent = parent;
	}

	// 子ノード追加
	void AddChild(NodeBase *child,NodeBase *erase_node)
	{
		auto itr = m_Child.begin();
		while (itr != m_Child.end())
		{
			if ((*itr) == erase_node)
			{
				itr = m_Child.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		m_Child.push_back(child);
	}

	// 兄弟ノードセッター
	void SetSibling(NodeBase *sibling)
	{
		m_Sibling = sibling;
	}

	// 行動データを持っているか
	bool HasAction()
	{
		return m_Action != NULL ? true : false;
	}

	// 実行可否判定
	bool Judgment(Enemy *enemy);

	// 優先順位選択
	NodeBase *SelectPriority(std::vector<NodeBase*> *list);
	// ランダム選択
	NodeBase *SelectRandom(std::vector<NodeBase*> *list);
	// オン・オフ選択
	NodeBase *SelectOnOff(std::vector<NodeBase*> *list, BehaviorData *data);
	// シーケンス選択
	NodeBase *SelectSequence(std::vector<NodeBase*> *list, BehaviorData *data);
	// ノード検索
	NodeBase *SearchNode(std::string search_name);
	// ノード推論
	NodeBase *Inference(Enemy *enemy, BehaviorData *data);
	// ノード名表示
	void PrintName();
	// 実行
	ActionBase::STATE Run(Enemy *enemy);

protected:
	std::string m_Name;						// 名前
	BehaviorTree::SELECT_RULE m_SelectRule;	// 選択ルール
	ExecJudgmentBase *m_ExecJudgment;		// 判定クラス
	ActionBase *m_Action;					// 実行クラス
	unsigned int m_Priority;				// 優先順位
	NodeBase *m_Parent;						// 親ノード
	std::vector<NodeBase *> m_Child;		// 子ノード
	NodeBase *m_Sibling;					// 兄弟ノード
	int m_HierarchyNo;						// 階層番号
};
#endif