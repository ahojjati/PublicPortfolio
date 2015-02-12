#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

string to_name(string file)
{
	if(file == "Vengefulspirit")
		return "Vengeful Spirit";
	else if(file == "Treant")
		return "Treant Protector";
	else if(file == "Spirit Breaker")
		return "Spiritbreaker";
	else if(file == "Shredder")
		return "Timbersaw";
	else if(file == "Rattletrap")
		return "Clockwerk";
	else if(file == "Queenofpain")
		return "Queen of Pain";
	else if(file == "Obsidian Destroyer")
		return "Outworld Destroyer";
	else if(file == "Life Stealer")
		return "Lifestealer";
	else if(file == "Furion")
		return "Nature's Prophet";
	else if(file == "Doom Bringer")
		return "Doom";
	else if(file == "Antimage")
		return "Anti-mage";
	else
		return file;
}

string to_file(string name)
{
	if(name == "treantprotector")
		return "treant";
	else if(name == "timbersaw")
		return "shredder";
	else if(name == "clockwerk")
		return "rattletrap";
	else if(name == "outworlddestroyer")
		return "obsidiandestroyer";
	else if(name == "nature'sprophet" || name == "naturesprophet")
		return "furion";
	else if(name == "doom")
		return "doombringer";
	else if(name == "anti-mage")
		return "antimage";
	else
		return name;
}

string fix_name(string name)
{
	bool flag = true;
	vector<char> mediary;
	for (int i = 0; i < name.size(); ++i)
	{
		if (name[i] == '.')
			break;
		if (flag && name[i] >= 'a' && name[i] <= 'z')
			mediary.push_back(name[i] + 'A' - 'a');
		else if(name[i] == '_' || name[i] == '-' || name[i] == '\'')
			mediary.push_back(' ');
		else
			mediary.push_back(name[i]);
		if (name[i] == ' ' || name[i] == '_')
			flag = true;
		else
			flag = false;
	}
	return to_name(string(mediary.begin(),mediary.end()));
}

void write_file(vector<int> start, vector<int> early, vector<int> core, vector<int> lux, int hero, string name)
{
	string name_2;
	string line;
	string filename;
	vector<string> start_names;
	vector<string> early_names;
	vector<string> core_names;
	vector<string> lux_names;
	ifstream myfile("hero_names.txt");
	if (myfile.is_open())
	{
		int i = 0;
		while(getline(myfile,line))
		{
			if (i == hero)
			{
				filename = line;
				break;
			}
			i++;
		}
		myfile.close();
	}
	else
	{
		cout << "Unable to open file! Make sure hero_names.txt is in this directory!" << endl;
		return;
	}
	name_2 = fix_name(filename.substr(8,filename.size() - 8));// !!!!!!!!!!!
	cout << name_2 << endl;
	ifstream itemfile("item_names.txt");
	if (itemfile.is_open())
	{
		int i = 0;
		int num_found = 0;
		int need_found = start.size() + early.size() + core.size() + lux.size();
		while(getline(itemfile,line))
		{
			for (int j = 0; j < start.size(); ++j)
			{
				if (start[j] == i)
				{
					num_found ++;
					start_names.push_back(line);
				}
			}
			for (int j = 0; j < early.size(); ++j)
			{
				if (early[j] == i)
				{
					num_found ++;
					early_names.push_back(line);
				}
			}
			for (int j = 0; j < core.size(); ++j)
			{
				if (core[j] == i)
				{
					num_found ++;
					core_names.push_back(line);
				}
			}
			for (int j = 0; j < lux.size(); ++j)
			{
				if (lux[j] == i)
				{
					num_found ++;
					lux_names.push_back(line);
				}
			}
			if (num_found == need_found)
			{
				break;
			}
			i++;
		}
	}
	ofstream finalfile;
	finalfile.open(filename);
	finalfile << "itembuilds\n{\n	\"author\"		\"Valve\"\n";
	finalfile << "	\"hero\"			\"npc_dota_hero_" <<  name << "\"\n";
	finalfile << "	\"Title\"			\"Recommended items for " << name_2 << "\"\n\n";
	finalfile << "	\"Items\"\n";
	finalfile << "	{		\n";
	finalfile << "		\"#DOTA_Item_Build_Starting_Items\"\n";
	finalfile << "		{\n";
	for (int i = 0; i < start_names.size(); ++i)
	{
		finalfile << "			\"item\"		";
		finalfile << "\""<< start_names[i] << "\"\n";
	}
	finalfile << "		}\n\n";
	finalfile << "		\"#DOTA_Item_Build_Early_Game\"\n";
	finalfile << "		{	\n";
	for (int i = 0; i < early_names.size(); ++i)
	{
		finalfile << "			\"item\"		";
		finalfile << "\""<< early_names[i] << "\"\n";
	}
	finalfile << "		}\n\n";
	finalfile << "		\"#DOTA_Item_Build_Core_Items\"\n";
	finalfile << "		{	\n";
	for (int i = 0; i < core_names.size(); ++i)
	{
		finalfile << "			\"item\"		";
		finalfile << "\""<< core_names[i] << "\"\n";
	}
	finalfile << "		}\n\n";
	finalfile << "		\"#DOTA_Item_Build_Luxury\"\n";
	finalfile << "		{	\n";
	for (int i = 0; i < lux_names.size(); ++i)
	{
		finalfile << "			\"item\"		";
		finalfile << "\""<< lux_names[i] << "\"\n";
	}
	finalfile << "		}\n";
	finalfile << "	}\n";
	finalfile << "}";
	finalfile.close();
}

int check_item(string item)
{
	string filename = "item.txt";
	string line;
	int ret_val = -1;
	ifstream myfile(filename);
	if (myfile.is_open())
	{
		int i = 0;
		while ( getline (myfile,line) )
    	{
			if (item == line)
			{
				ret_val = i;
				break;
			}
			i++;
		}
		myfile.close();
	}
	else
	{
		cout << "Unable to open file! Make sure item.txt is in this directory" << endl;
		return -1;
	}
	return ret_val;
}

//checks to see if hero input is valid
int check_hero(string hero)
{
	string filename = "hero.txt";
	string line;
	int ret_val = -1;;
  	ifstream myfile(filename);
  	if (myfile.is_open())
  	{
  		int i = 0;
		while ( getline (myfile,line) )
    	{
			if (hero == line)
			{
				ret_val = i;
				break;
			}
			i++;
		}
		myfile.close();
	}
	else
	{
		cout << "Unable to open file! Make sure hero.txt is in this directory" << endl;
		return -1;
	}
	return ret_val;
}

//converts string to lower-case
string prep_string(string x)
{
	vector<char> temp;
	for (int i = 0; i < x.size(); ++i)
	{
		if (x[i] >= 'A' && x[i] <= 'Z')
		{
			temp.push_back(x[i] + 'a' - 'A');
		}
		else if((x[i] >= 'a' && x[i] <= 'z') || x[i] == ',')
			temp.push_back(x[i]);
	}
	return string(temp.begin(), temp.end());
}

//parse the string by the character c
vector<string> parse(string in, char c)
{
	vector<char> temp;
	vector<string> ret_val;
	for (int i = 0; i < in.size(); ++i)
	{
		if (in[i] == c)
		{
			if (temp.size() > 0)
			{
				ret_val.push_back(string(temp.begin(),temp.end()));
				temp.clear();
			}
		}
		else
			temp.push_back(in[i]);
	}
	if(temp.size() > 0)
		ret_val.push_back(string(temp.begin(),temp.end()));
	return ret_val;
}

vector<string> get_it(vector<int> & nums)
{
	vector<string> ret_val;
	string item_temp;
	cin>>item_temp;
	item_temp = prep_string(item_temp);
	ret_val = parse(item_temp,',');
	for (int i = 0; i < ret_val.size(); ++i)
	{
		if (ret_val[i] == "quit")
		{
			return vector<string>();
		}
		nums.push_back(check_item(ret_val[i]));
		while(nums[nums.size() - 1] == -1)
		{
			string line;
			cout << "Invalid Item : " << ret_val[i] << " please re-enter item : " << endl;
			cin >> line;
			ret_val[i] = line;
			if (line == "quit")
			{
				return vector<string>();
			}
			nums[nums.size() - 1] = check_item(ret_val[i]);
		}
	}
	return ret_val;
}

int main()
{
	string hero;
	string user_in;
	vector<string> core_items;
	vector<string> early_items;
	vector<string> luxury_items;
	vector<string> starting_items;
	string filename;
	vector<int> start_nums;
	vector<int> early_nums;
	vector<int> core_nums;
	vector<int> luxury_nums;
	int hero_num = -1;

	cout << "Hero Name?" << endl;
	cout << "Do not type ANY space characters! (type 'quit' at any time to quit)" << endl;
	while(1)
	{
		cin >> hero;
		hero = prep_string(hero);
		hero = to_file(hero);
		if (hero == "quit")
		{
			return 0;
		}
		hero_num = check_hero(hero);
		if (hero_num != -1)
		{
			break;
		}
		cout << "Invalid Hero Name! Please re-enter:" << endl;
	}
	cout << "Starting items? (comma-separated)" << endl;
	starting_items = get_it(start_nums);
	if (starting_items.size() == 0)
	{
		return 0;
	}
	cout << "Early-game items? (comma-separated)" << endl;
	early_items = get_it(early_nums);
	if (early_items.size() == 0)
	{
		return 0;
	}
	cout << "Core items? (comma-separated)" << endl;
	core_items = get_it(core_nums);
	if (core_items.size() == 0)
	{
		return 0;
	}
	cout << "Luxury items? (comma-separated)" << endl;
	luxury_items = get_it(luxury_nums);
	if (luxury_items.size() == 0)
	{
		return 0;
	}
	cout << endl << "Please Confirm: (y/n)" << endl;
	cout << "Starting Items : " << endl;
	cout << starting_items[0];
	for (int i = 1; i < starting_items.size(); ++i)
	{
		cout << ", "<< starting_items[i];
	}
	cout << "\n";
	cout << "Early-Game Items : " << endl;
	cout << early_items[0];
	for (int i = 1; i < early_items.size(); ++i)
	{
		cout << ", "<< early_items[i];
	}
	cout << "\n";
	cout << "Core Items : " << endl;
	cout << core_items[0];
	for (int i = 1; i < core_items.size(); ++i)
	{
		cout << ", "<< core_items[i];
	}
	cout << "\n";
	cout << "Luxury Items : " << endl;
	cout << luxury_items[0];
	for (int i = 1; i < luxury_items.size(); ++i)
	{
		cout << ", "<< luxury_items[i];
	}
	cout << "\n";
	cin >> user_in;
	while(1)
	{
		user_in = prep_string(user_in);
		if (user_in == "quit")
		{
			return 0;
		}
		if (user_in.size() > 0 && user_in[0] == 'y')
		{
			break;
		}
		else if (user_in.size() > 0 && user_in[0] == 'n')
		{
			cout << "Would you like to start over? (y to start over, any other character to quit)" << endl;
			cin >> user_in;
			user_in = prep_string(user_in);
			if (user_in.size() > 0 && user_in[0] == 'y')
			{
				return main();
			}
			return 0;
		}
		cout << "Let's try again: (y/n)" << endl;
		cin >> user_in;
	}
	write_file(start_nums, early_nums, core_nums, luxury_nums, hero_num, hero);
	return 0;
}