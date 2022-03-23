#ifndef NODE_BASE_H_
#define NODE_BASE_H_

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

class ExecJudgmentBase;
class BehaviorData;

// �m�[�h
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

	// ���O�Q�b�^�[
	std::string GetName()
	{
		return m_Name;
	}

	// �e�m�[�h�Q�b�^�[
	NodeBase *GetParent()
	{
		return m_Parent;
	}

	// �q�m�[�h�Q�b�^�[
	NodeBase* GetChild(int idx)
	{
		if (m_Child.size() >= idx)
		{
			return NULL;
		}

		return m_Child[idx];
	}

	// �q�m�[�h�Q�b�^�[(����)
	NodeBase *GetLastChild()
	{
		if (m_Child.size() == 0)
		{
			return NULL;
		}

		return m_Child[m_Child.size() - 1];
	}

	// �q�m�[�h�Q�b�^�[(�擪)
	NodeBase *GetTopChild()
	{
		if (m_Child.size() == 0)
		{
			return NULL;
		}

		return m_Child[0];
	}

	// �Z��m�[�h�Q�b�^�[
	NodeBase *GetSibling(void)
	{
		return m_Sibling;
	}

	// �K�w�ԍ��Q�b�^�[
	int GetHirerchyNo()
	{
		return m_HierarchyNo;
	}

	// �D�揇�ʃQ�b�^�[
	int GetPriority()
	{
		return m_Priority;
	}

	// �e�m�[�h�Z�b�^�[
	void SetParent(NodeBase *parent)
	{
		m_Parent = parent;
	}

	// �q�m�[�h�ǉ�
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

	// �Z��m�[�h�Z�b�^�[
	void SetSibling(NodeBase *sibling)
	{
		m_Sibling = sibling;
	}

	// �s���f�[�^�������Ă��邩
	bool HasAction()
	{
		return m_Action != NULL ? true : false;
	}

	// ���s�۔���
	bool Judgment(Enemy *enemy);

	// �D�揇�ʑI��
	NodeBase *SelectPriority(std::vector<NodeBase*> *list);
	// �����_���I��
	NodeBase *SelectRandom(std::vector<NodeBase*> *list);
	// �I���E�I�t�I��
	NodeBase *SelectOnOff(std::vector<NodeBase*> *list, BehaviorData *data);
	// �V�[�P���X�I��
	NodeBase *SelectSequence(std::vector<NodeBase*> *list, BehaviorData *data);
	// �m�[�h����
	NodeBase *SearchNode(std::string search_name);
	// �m�[�h���_
	NodeBase *Inference(Enemy *enemy, BehaviorData *data);
	// �m�[�h���\��
	void PrintName();
	// ���s
	ActionBase::STATE Run(Enemy *enemy);

protected:
	std::string m_Name;						// ���O
	BehaviorTree::SELECT_RULE m_SelectRule;	// �I�����[��
	ExecJudgmentBase *m_ExecJudgment;		// ����N���X
	ActionBase *m_Action;					// ���s�N���X
	unsigned int m_Priority;				// �D�揇��
	NodeBase *m_Parent;						// �e�m�[�h
	std::vector<NodeBase *> m_Child;		// �q�m�[�h
	NodeBase *m_Sibling;					// �Z��m�[�h
	int m_HierarchyNo;						// �K�w�ԍ�
};
#endif