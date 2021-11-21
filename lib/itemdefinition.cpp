#include "itemdefintion.h"
item_definition::item item_definition::parse(const std::vector<char>& buf)
{
	input_stream is(buf);
	item_definition::item def;
	while (true)
	{
		const auto opcode = is.read<unsigned char>();
		if (opcode == 0)
			break;

		if (opcode == 1)
		{
			def.inventory_model = is.read<unsigned short>();
		}
		else if (opcode == 2)
		{
			def.name = is.read<std::string>();
		}
		else if (opcode == 4) {
			def.zoom2d = is.read<unsigned short>();
		}
		else if (opcode == 5) {
			def.xan2d = is.read<unsigned short>();
		}
		else if (opcode == 6) {
			def.yan2d = is.read<unsigned short>();
		}
		else if (opcode == 7) {
			def.xOffset2d = is.read<unsigned short>();
			if (def.xOffset2d > 32767)
				def.xOffset2d -= 65536;
		}
		else if (opcode == 8) {
			def.yOffset2d = is.read<unsigned short>();
			if (def.yOffset2d > 32767)
				def.yOffset2d -= 65536;
		}
		else if (opcode == 9) {
			def.unk1 = is.read<std::string>();
		}
		else if (opcode == 11) {
			def.stackable = 1;
		}
		else if (opcode == 12) {
			def.cost = is.read<int>();
		}
		else if (opcode == 16) {
			def.members = true;
		}
		else if (opcode == 23) {
			def.maleModel0 = is.read<unsigned short>();
			def.maleOffset = is.read<unsigned char>();
		}
		else if (opcode == 24) {
			def.maleModel1 = is.read<unsigned short>();
		}
		else if (opcode == 25) {
			def.femaleModel0 = is.read<unsigned short>();
			def.femaleOffset = is.read<unsigned char>();
		}
		else if (opcode == 26) {
			def.femaleModel1 = is.read<unsigned short>();
		}
		else if (opcode >= 30 && opcode < 35)
		{
			def.options[opcode - 30] = is.read<std::string>();
		}
		else if (opcode >= 35 && opcode < 40)
		{
			def.interface_options[opcode - 35] = is.read<std::string>();
		}
		else if (opcode == 40)
		{
			const auto var5 = is.read<unsigned char>();
			def.colorFind.resize(var5);
			def.colorReplace.resize(var5);

			for(auto i = 0; i < var5; i++)
			{
				def.colorFind[i] = is.read<unsigned short>();
				def.colorReplace[i] = is.read<unsigned short>();
			}

		}
		else if (opcode == 41) {

			const auto var5 = is.read<unsigned char>();
			def.textureFind.resize(var5);
			def.textureReplace.resize(var5);

			for(auto i = 0; i < var5; i++)
			{
				def.textureFind[i] = is.read<unsigned short>();
				def.textureReplace[i] = is.read<unsigned short>();
			}

		}  else if (opcode == 42) {
			def.shift_click_drop_index = is.read<char>();
		}  else if (opcode == 65) {
			def.tradeable = true;
		}  else if (opcode == 78) {
			def.maleModel2 = is.read<unsigned short>();
		}  else if (opcode == 79) {
			def.femaleModel2 = is.read<unsigned short>();
		}  else if (opcode == 90) {
			def.maleHeadModel = is.read<unsigned short>();
		}  else if (opcode == 91) {
			def.femaleHeadModel = is.read<unsigned short>();
		}  else if (opcode == 92) {
			def.maleHeadModel2 = is.read<unsigned short>();
		}  else if (opcode == 93) {
			def.femaleHeadModel2 = is.read<unsigned short>();
		}  else if (opcode == 94) {
			def.category = is.read<unsigned short>();
		}  else if (opcode == 95) {
			def.zan2d = is.read<unsigned short>();
		}  else if (opcode == 97) {
			def.noted_id = is.read<unsigned short>();
		}  else if (opcode == 98) {
			def.noted_template = is.read<unsigned short>();
		}  else if (opcode >= 100 && opcode < 110) {
			if(def.count_obj.capacity() == 0)
			{
				def.count_obj.resize(10);
				def.count_co.resize(10);
			}

			def.count_obj[opcode - 100] = is.read<unsigned short>();
			def.count_co[opcode - 100] = is.read<unsigned short>();
		}
		else if (opcode == 110)
		{
			def.resizeX = is.read<unsigned short>();
		}
		else if (opcode == 111)
		{
			def.resizeY = is.read<unsigned short>();
		}
		else if (opcode == 112)
		{
			def.resizeZ = is.read<unsigned short>();
		}
		else if (opcode == 113)
		{
			def.ambient = is.read<char>();
		}
		else if (opcode == 114)
		{
		def.contrast = is.read<char>();
		}
		else if (opcode == 115)
		{
			def.team = is.read<unsigned char>();
		}
		else if (opcode == 139)
		{
		def.bought_id = is.read<unsigned short>();
		}
		else if (opcode == 140)
		{
		def.bought_template_id = is.read<unsigned short>();
		}
		else if (opcode == 148)
		{
		def.placeholder_id = is.read<unsigned short>();
		}
		else if (opcode == 149)
		{
		def.placeholder_template_id = is.read<unsigned short>();
		}
		else if (opcode == 249)
		{
			const auto length = is.read<unsigned char>();

			for(auto i = 0; i < length; i++)
			{
				const bool isstring = is.read<unsigned char>() == 1;
				int key = is.read<int>(3);
				std::variant<std::string, int> val;
				if(isstring)
				{
					val = is.read<std::string>();
				}
				else
				{
					val = is.read<int>();
				}

				def.params.emplace(key, val);
			}
		}
	}
	return def;
}
