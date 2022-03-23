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



// 削除結果
enum EraseResult
{
	NotFound,		// 未発見
	Erased,			// 削除
	CouldntErased	// 削除できない
};

std::list<Cell> route_list;



// コストテーブル
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

// グラフ
Node Map[MapHeight][MapWidth];

// 昇順ソート用関数
bool Less(Node* a, Node* b)
{
	if (a->TotalCost < b->TotalCost)
	{
		return true;
	}

	return false;
}

// セル範囲チェック関数
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

// ノードの作成
void CreateMap()
{
	for (int z = 0; z < MapHeight; z++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[z][x].Position.X = x;
			Map[z][x].Position.Z = z;

			// ブロックの追加
			if (CostTable[z][x] == 0) 
			{
				Scene* scene = Manager::GetScene();
				scene->AddGameObject<Block>(1)->SetPositionScale(D3DXVECTOR3(((float)x - 10) * 5.0f + 2.5f, 1.0f, ((float)z - 10) * -5.0f - 2.5f),D3DXVECTOR3(2.0f, 1.0f, 2.0f));
			}

			Cell adjacent_cells[] =
			{
				//Cell(x - 1, z - 1),	// 左上
				//Cell(x + 1, z - 1),	// 右上
				//Cell(x - 1, z + 1),	// 左下
				//Cell(x + 1, z + 1),	// 右下
				Cell(x, z - 1),		// 上
				Cell(x - 1, z),		// 左
				Cell(x + 1, z),		// 右
				Cell(x, z + 1),		// 下
			};

			// 隣接ノードの追加
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

// コスト初期化
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

// ヒューリスティックコスト計算(ノードとゴールまでの距離を返している)
float CalculateHeuristic(const Node* node, const Node* Goal)
{
	float x = fabsf(Goal->Position.X - node->Position.X);
	float z = fabsf(Goal->Position.Z - node->Position.Z);

	return sqrtf(x * x + z * z);
}

// セル比較
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
	// クローズリストチェック
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// ノードと同じセルがあるか調べる
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// コストの比較
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

// オープンリストに追加
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
		// リストに同じノードがあってリストの方のコストが高いなら削除
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

	// 更新したノード位置保存用
	Cell last_update_cells[MapHeight][MapWidth];

	// グラフの初期化
	InitCost(Infinity, 0);

	// スタートノードの指定
	open_list.push_back(&Map[start.Z][start.X]);

	// 経路探索回数
	int count = 0;

	// オープンリストがなくなるまで回す
	while (open_list.empty() == false)
	{
		count++;

		Node* search_node = (*open_list.begin());
		// 探索リストから除外
		open_list.erase(open_list.begin());

		// ゴールに到達したら終わり
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// クローズリストに最後のノードを追加する
			close_list.push_back(search_node);
			break;
		}

		for (Node* adjacent_node : search_node->AdjacentNodes)
		{
			// 計算を行っていなかった場合だけ計算
			if (adjacent_node->HeuristicCost == Infinity)
			{
				// ヒューリスティクスコスト計算
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}

			// ノード間コスト
			float edge_cost = CostTable[adjacent_node->Position.Z][adjacent_node->Position.X];
			// 取得ノードのトータルコスト
			float node_cost = search_node->TotalCost;
			/*
				トータルコスト算出
					ノード間コスト + ヒューリスティックコスト + 取得ノードのトータルコスト
			*/
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// ノード追加
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// トータルコストを更新
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.Z == 0 && adjacent_node->Position.X == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// 経路を更新したセルを保存
				last_update_cells[adjacent_node->Position.Z][adjacent_node->Position.X] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// クローズリストチェック
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			// ノードと同じセルがあるか調べる
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// 同じノードが無かったので追加
		if (is_add_close == true)
		{
			// このノードの探索終了
			close_list.push_back(search_node);
		}

		// 昇順ソート
		open_list.sort(Less);
	}


	// ゴールセルから復元する
	route_list.clear();
	route_list.push_back(goal);
	while (route_list.empty() == false)
	{
		Cell route = route_list.front();

		// スタートセルなら終了
		if (IsEqualCell(route, start) == true)
		{
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.X, route.Z) == true)
			{
				// 追加
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


