#include "main.h"
#include "manager.h"
#include "inputBehavior.h"
#include <iostream> //
#include <fstream>	// csvで必要
#include <string>	// 
#include <sstream>	//

#include "enemy.h"
#include "behaviorTree.h"
#include "attackJudgment.h"
#include "shotAction.h"
#include "diveJudgment.h"
#include "diveAction.h"
#include "shotJudgment.h"
#include "bombAction.h"
#include "chaseJudgment.h"
#include "walkJudgment.h"
#include "walkChaseAction.h"
#include "pathfinderAction.h"
#include "waitJudgment.h"
#include "waitAction.h"


void InputBehavior::InputAi()
{
	// csvで必要
	std::string str_buf;
	std::string str_comma_buf;
	std::string input_csv_file_path = "asset\\ai_data\\enemy_ai.csv";
	std::string output_csv_file_path = "asset\\ai_data\\write_ai.csv";

	// 読み込むcsvファイルを開く(std::ofstreamのコンストラクタで開く)
	std::ifstream ifs_csv_file(input_csv_file_path);

	// 書き込むcsvファイルを開く(std::ofstreamのコンストラクタで開く)
	std::ofstream ofs_csv_file(output_csv_file_path);

	BehaviorTree* behavior = new BehaviorTree();

	int load_count = 1;
	// 親ノード名
	std::string parent_node;
	// 追加するノード名
	std::string this_node;
	// 優先度
	int priority;
	// 選択ルール
	BehaviorTree::SELECT_RULE select_rule;
	// 判定用クラス
	ExecJudgmentBase *judgment_instance = NULL;
	// 行動用クラス
	ActionBase *action_instance = NULL;

	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(ifs_csv_file, str_buf)) {
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		// 「,」区切りごとにデータを読み込む
		while (getline(i_stream, str_comma_buf, ',')) {
			switch (load_count)
			{
			case 1:
				parent_node = str_comma_buf;
				load_count++;
				break;
			case 2:
				this_node = str_comma_buf;
				load_count++;
				break;
			case 3:
				priority = stoi(str_comma_buf);
				load_count++;
				break;
			case 4:
				if (str_comma_buf == "Priority")
				{
					select_rule = BehaviorTree::SELECT_RULE::PRIORITY;
				}
				else if (str_comma_buf == "Sequence")
				{
					select_rule = BehaviorTree::SELECT_RULE::SEQUENCE;
				}
				else if (str_comma_buf == "Sequential_Looping")
				{
					select_rule = BehaviorTree::SELECT_RULE::SEQUENTIAL_LOOPING;
				}
				else if (str_comma_buf == "Random")
				{
					select_rule = BehaviorTree::SELECT_RULE::RANDOM;
				}
				else if (str_comma_buf == "On_Off")
				{
					select_rule = BehaviorTree::SELECT_RULE::ON_OFF;
				}
				else
				{
					select_rule = BehaviorTree::SELECT_RULE::NON;
				}
				load_count++;
				break;
			case 5:
				if (str_comma_buf == "Attack")
				{
					judgment_instance = AttackJudgment::GetInstance();
				}
				else if (str_comma_buf == "Dive")
				{
					judgment_instance = DiveJudgment::GetInstance();
				}
				else if (str_comma_buf == "Shot")
				{
					judgment_instance = ShotJudgment::GetInstance();
				}
				else if (str_comma_buf == "Chase")
				{
					judgment_instance = ChaseJudgment::GetInstance();
				}
				else if (str_comma_buf == "Walk")
				{
					judgment_instance = WalkJudgment::GetInstance();
				}
				else if (str_comma_buf == "Wait")
				{
					judgment_instance = WaitJudgment::GetInstance();
				}
				else
				{
					judgment_instance = NULL;
				}
				load_count++;
				break;
			case 6:
				if (str_comma_buf == "Shot")
				{
					action_instance = ShotAction::GetInstance();
				}
				else if (str_comma_buf == "Dive")
				{
					action_instance = DiveAction::GetInstance();
				}
				else if (str_comma_buf == "Bomb")
				{
					action_instance = BombAction::GetInstance();
				}
				else if (str_comma_buf == "WalkChase")
				{
					action_instance = WalkChaseAction::GetInstance();
				}
				else if (str_comma_buf == "Pathfinder")
				{
					action_instance = PathfinderAction::GetInstance();
				}
				else if (str_comma_buf == "Wait")
				{
					action_instance = WaitAction::GetInstance();
				}
				else
				{
					action_instance = NULL;
				}
				load_count=1;
				break;
			default:
				break;
			}
		}

		behavior->AddNode(parent_node, this_node, priority, select_rule, judgment_instance, action_instance);
		
		// 改行する
		ofs_csv_file << std::endl;
	}

	Scene* scene = Manager::GetScene();

	scene->AddGameObject<Enemy>(1,behavior)->SetPosition(D3DXVECTOR3(0.0f, 20.0f, 15.0f));
}
