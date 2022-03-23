#ifndef ASTAR_H_
#define ASTAR_H_

const int MapWidth = 20;
const int MapHeight = 20;
const int Infinity = 100000;

// Mapのマス目
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

	int X;		// 列
	int Z;		// 行
};

// ノード
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

	Cell Position;						// ノード座標
	std::vector<Node*> AdjacentNodes;	// 隣接ノード(辺)
	float HeuristicCost;				// ヒューリスティックコスト
	float TotalCost;					// コスト(ヒューリスティックコスト込み)
};

std::list<Cell> GetList();
void PopList();

void CreateMap();

void AStar(float startx,float startz, float goalx,float goalz);

#endif