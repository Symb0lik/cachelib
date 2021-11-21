#include <string>
#include <vector>
#include <iostream>
#include <format>
#include "inputstream.h"
#include <variant>
#include <map>

class item_definition
{
public:
	struct item
	{
		unsigned id;

		std::string name;
		std::string unk1;
		unsigned short resizeX = 128;
		unsigned short resizeY = 128;
		unsigned short resizeZ = 128;
		unsigned short xan2d = 0;
		unsigned short yan2d = 0;
		unsigned short zan2d = 0;
		unsigned short noted_id;
		unsigned short noted_template;

		bool tradeable;
		unsigned char stackable;
		int model;
		bool members;
		unsigned short inventory_model;
		std::vector<unsigned short> colorFind;
		std::vector<unsigned short> colorReplace;

		std::vector<unsigned short> textureFind;
		std::vector<unsigned short> textureReplace;
		std::vector<unsigned short> count_obj;
		std::vector<unsigned short> count_co;

		char shift_click_drop_index;
		unsigned short zoom2d = 2000;
		unsigned short xOffset2d = 0;
		unsigned short yOffset2d = 0;
		unsigned short maleModel0;
		unsigned char maleOffset;
		unsigned short maleModel1;
		unsigned short maleModel2;
		unsigned short maleHeadModel;
		unsigned short maleHeadModel2;
		unsigned short femaleModel0;
		unsigned char femaleOffset;
		unsigned short femaleModel1;
		unsigned short femaleModel2;
		unsigned short femaleHeadModel;
		unsigned short femaleHeadModel2;
		unsigned short category;
		unsigned short bought_id;
		unsigned short bought_template_id;
		unsigned short placeholder_id;
		unsigned short placeholder_template_id;
		std::vector<std::string> options{ "", "", "Take", "", "" };
		std::vector<std::string> interface_options{ "", "", "", "", "Drop" };
		char ambient;
		char contrast;
		unsigned char team;
		int cost;

		std::map<int, std::variant<std::string, int>> params;
	};

	static item parse(const std::vector<char>& buf);
};