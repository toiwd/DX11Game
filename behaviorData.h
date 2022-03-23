#ifndef BEHAVIOR_DATA_H_
#define BEHAVIOR_DATA_H_

#include <vector>
#include <stack>
#include <map>
#include "BehaviorTree.h"

class NodeBase;
class Enemy;

// Behavior�ۑ��f�[�^
class BehaviorData
{
public:
	BehaviorData()
	{
		Init();
	}

	// �V�[�P���X�m�[�h�̃v�b�V��
	void PushSequenceNode(NodeBase *node)
	{
		m_SequenceStack.push(node);
	}

	// �V�[�P���X�m�[�h�̃|�b�v
	NodeBase *PopSequenceNode()
	{
		// ��Ȃ�NULL
		if (m_SequenceStack.empty() != 0)
		{
			return NULL;
		}

		NodeBase *node = m_SequenceStack.top();

		if (node != NULL)
		{
			// ���o�����f�[�^���폜
			m_SequenceStack.pop();
		}

		return node;
	}

	// �m�[�h�g�p����
	bool IsNodeUsed(std::string name)
	{
		// �g�p���ĂȂ�������false
		if (m_UsedNodeMap.count(name) == 0)
		{
			return false;
		}

		return m_UsedNodeMap[name];
	}

	// �g�p�ς݃m�[�h�ɓo�^
	void EntryUsedNode(std::string name)
	{
		m_UsedNodeMap[name] = true;
	}

	// �V�[�P���X�X�e�b�v�̃Q�b�^�[
	int GetSequenceStep(std::string name)
	{
		if (m_RunSequenceStepMap.count(name) == 0)
		{
			m_RunSequenceStepMap[name] = 0;
		}

		return m_RunSequenceStepMap[name];
	}

	// �V�[�P���X�X�e�b�v�̃Z�b�^�[
	void SetSequenceStep(std::string name, int step)
	{
		m_RunSequenceStepMap[name] = step;
	}

	// ������
	void Init()
	{
		m_RunSequenceStepMap.clear();
		while (m_SequenceStack.size() > 0)
		{
			m_SequenceStack.pop();
		}
	}

	// �g�p�ς݃m�[�h�̃��Z�b�g
	void ResetNodeUsed(std::vector<NodeBase*> *reset_hierachy);

private:
	std::stack<NodeBase*> m_SequenceStack;				// �V�[�P���X�m�[�h�X�^�b�N
	std::map<std::string, int> m_RunSequenceStepMap;	// ���s�V�[�P���X�̃X�e�b�v�}�b�v
	std::map<std::string, bool> m_UsedNodeMap;			// �m�[�h�̎g�p����}�b�v
};

#endif
