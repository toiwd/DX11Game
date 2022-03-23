#ifndef ASTAR_H_
#define ASTAR_H_

const int MapWidth = 20;
const int MapHeight = 20;
const int Infinity = 100000;

// Map�̃}�X��
struct Cell
{
	Cell() :
		X(-1),
		Z(-1)
	{
	}

	Cell(int x, int z)
	{
		X = x;
		Z = z;
	}

	int X;		// ��
	int Z;		// �s
};

// �m�[�h
struct Node
{
	Node() :
		Node(0, 0)
	{
	}

	Node(int x, int z) :
		Position(x, z),
		HeuristicCost(Infinity),
		TotalCost(0)
	{
		AdjacentNodes.clear();
	}

	Cell Position;						// �m�[�h���W
	std::vector<Node*> AdjacentNodes;	// �אڃm�[�h(��)
	float HeuristicCost;				// �q���[���X�e�B�b�N�R�X�g
	float TotalCost;					// �R�X�g(�q���[���X�e�B�b�N�R�X�g����)
};

std::list<Cell> GetList();
void PopList();

void CreateMap();

void AStar(float startx,float startz, float goalx,float goalz);

#endif