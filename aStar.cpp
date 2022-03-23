#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "aStar.h"
#include "scene.h"
#include <stdio.h>
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>
#include "block.h"
#include "bullet.h"



// �폜����
enum EraseResult
{
	NotFound,		// ������
	Erased,			// �폜
	CouldntErased	// �폜�ł��Ȃ�
};

std::list<Cell> route_list;



// �R�X�g�e�[�u��
int CostTable[MapHeight][MapWidth] =
{
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 1
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 2
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 3
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 4
	{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // 5
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 6
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 7
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}, // 8
	{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}, // 9
	{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}, // 10
	{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}, // 11
	{ 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}, // 12
	{ 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 13
	{ 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1}, // 14
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1}, // 15
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1}, // 16
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 17
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 18
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 19
};

// �O���t
Node Map[MapHeight][MapWidth];

// �����\�[�g�p�֐�
bool Less(Node* a, Node* b)
{
	if (a->TotalCost < b->TotalCost)
	{
		return true;
	}

	return false;
}

// �Z���͈̓`�F�b�N�֐�
bool IsCellWithinTheRange(int x, int z)
{
	if (x >= 0 &&
		x < MapWidth &&
		z >= 0 &&
		z < MapHeight)
	{
		return true;
	}

	return false;
}

std::list<Cell> GetList()
{
	return route_list;
}

void PopList()
{
	route_list.pop_front();
}

// �m�[�h�̍쐬
void CreateMap()
{
	for (int z = 0; z < MapHeight; z++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[z][x].Position.X = x;
			Map[z][x].Position.Z = z;

			// �u���b�N�̒ǉ�
			if (CostTable[z][x] == 0) 
			{
				Scene* scene = Manager::GetScene();
				scene->AddGameObject<Block>(1)->SetPositionScale(D3DXVECTOR3(((float)x - 10) * 5.0f + 2.5f, 1.0f, ((float)z - 10) * -5.0f - 2.5f),D3DXVECTOR3(2.0f, 1.0f, 2.0f));
			}

			Cell adjacent_cells[] =
			{
				//Cell(x - 1, z - 1),	// ����
				//Cell(x + 1, z - 1),	// �E��
				//Cell(x - 1, z + 1),	// ����
				//Cell(x + 1, z + 1),	// �E��
				Cell(x, z - 1),		// ��
				Cell(x - 1, z),		// ��
				Cell(x + 1, z),		// �E
				Cell(x, z + 1),		// ��
			};

			// �אڃm�[�h�̒ǉ�
			for (const Cell& cell : adjacent_cells)
			{
				if (IsCellWithinTheRange(cell.X, cell.Z) == true &&
					CostTable[cell.Z][cell.X] == 1)
				{
					Map[z][x].AdjacentNodes.push_back(&Map[cell.Z][cell.X]);
				}
			}
		}
	}
}

// �R�X�g������
void InitCost(int heuristic_cost, int total_cost)
{
	for (int z = 0; z < MapHeight; z++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[z][x].HeuristicCost = heuristic_cost;
			Map[z][x].TotalCost = total_cost;
		}
	}
}

// �q���[���X�e�B�b�N�R�X�g�v�Z(�m�[�h�ƃS�[���܂ł̋�����Ԃ��Ă���)
float CalculateHeuristic(const Node* node, const Node* Goal)
{
	float x = fabsf(Goal->Position.X - node->Position.X);
	float z = fabsf(Goal->Position.Z - node->Position.Z);

	return sqrtf(x * x + z * z);
}

// �Z����r
bool IsEqualCell(const Cell& a, const Cell& b)
{
	if (a.X == b.X &&
		a.Z == b.Z)
	{
		return true;
	}

	return false;
}

EraseResult EraseNode(std::list<Node*>& list, Node* new_node, float new_cost)
{
	// �N���[�Y���X�g�`�F�b�N
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// �m�[�h�Ɠ����Z�������邩���ׂ�
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// �R�X�g�̔�r
			if (new_cost < (*itr)->TotalCost)
			{
				list.erase(itr);
				return EraseResult::Erased;
			}
			else
			{
				return EraseResult::CouldntErased;
			}
		}
	}

	return EraseResult::NotFound;
}

// �I�[�v�����X�g�ɒǉ�
bool AddAdjacentNode(std::list<Node*>& open_list, std::list<Node*>& close_list, Node* adjacent_node, float cost)
{
	bool can_update = true;

	std::list<Node*> node_list[] =
	{
		close_list,
		open_list
	};

	for (std::list<Node*>& list : node_list)
	{
		// ���X�g�ɓ����m�[�h�������ă��X�g�̕��̃R�X�g�������Ȃ�폜
		if (EraseNode(list, adjacent_node, cost) == EraseResult::CouldntErased)
		{
			can_update = false;
		}
	}

	if (can_update == true)
	{
		open_list.push_back(adjacent_node);
		return true;
	}

	return false;
}

void AStar(float startx, float startz, float goalx, float goalz)
{
	std::list<Node*> open_list;
	std::list<Node*> close_list;
	int start_x = startx / 5.0f + 10.0f;
	int start_z = startz / -5.0f + 10.0f;
	float goal_x1 = (goalx + 2.5f) / 5.0f + 9.0f;
	float goal_z1 = (goalz - 2.5f) / -5.0f + 9.0f; 
	int goal_x2 = roundf(goal_x1);
	int goal_z2 = roundf(goal_z1);

	Cell start(start_x, start_z);
	Cell goal(goal_x2, goal_z2);

	//const Node* start_node = &Map[start.Y][start.X];
	const Node* goal_node = &Map[goal.Z][goal.X];

	// �X�V�����m�[�h�ʒu�ۑ��p
	Cell last_update_cells[MapHeight][MapWidth];

	// �O���t�̏�����
	InitCost(Infinity, 0);

	// �X�^�[�g�m�[�h�̎w��
	open_list.push_back(&Map[start.Z][start.X]);

	// �o�H�T����
	int count = 0;

	// �I�[�v�����X�g���Ȃ��Ȃ�܂ŉ�
	while (open_list.empty() == false)
	{
		count++;

		Node* search_node = (*open_list.begin());
		// �T�����X�g���珜�O
		open_list.erase(open_list.begin());

		// �S�[���ɓ��B������I���
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// �N���[�Y���X�g�ɍŌ�̃m�[�h��ǉ�����
			close_list.push_back(search_node);
			break;
		}

		for (Node* adjacent_node : search_node->AdjacentNodes)
		{
			// �v�Z���s���Ă��Ȃ������ꍇ�����v�Z
			if (adjacent_node->HeuristicCost == Infinity)
			{
				// �q���[���X�e�B�N�X�R�X�g�v�Z
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}

			// �m�[�h�ԃR�X�g
			float edge_cost = CostTable[adjacent_node->Position.Z][adjacent_node->Position.X];
			// �擾�m�[�h�̃g�[�^���R�X�g
			float node_cost = search_node->TotalCost;
			/*
				�g�[�^���R�X�g�Z�o
					�m�[�h�ԃR�X�g + �q���[���X�e�B�b�N�R�X�g + �擾�m�[�h�̃g�[�^���R�X�g
			*/
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// �m�[�h�ǉ�
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// �g�[�^���R�X�g���X�V
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.Z == 0 && adjacent_node->Position.X == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// �o�H���X�V�����Z����ۑ�
				last_update_cells[adjacent_node->Position.Z][adjacent_node->Position.X] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// �N���[�Y���X�g�`�F�b�N
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			// �m�[�h�Ɠ����Z�������邩���ׂ�
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// �����m�[�h�����������̂Œǉ�
		if (is_add_close == true)
		{
			// ���̃m�[�h�̒T���I��
			close_list.push_back(search_node);
		}

		// �����\�[�g
		open_list.sort(Less);
	}


	// �S�[���Z�����畜������
	route_list.clear();
	route_list.push_back(goal);
	while (route_list.empty() == false)
	{
		Cell route = route_list.front();

		// �X�^�[�g�Z���Ȃ�I��
		if (IsEqualCell(route, start) == true)
		{
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.X, route.Z) == true)
			{
				// �ǉ�
				route_list.push_front(last_update_cells[route.Z][route.X]);
			}
			else
			{

				break;
			}
		}
	}
	route_list.pop_front();
}


