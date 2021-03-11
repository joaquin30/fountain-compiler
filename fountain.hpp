#ifndef FOUNTAIN_HPP
#define FOUNTAIN_HPP

#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <utility>
#include <algorithm>

namespace fountain {
	using namespace std;
	enum class elem {
		action,
		scene_heading,
		character,
		parenthetical,
		dialogue,
		transition,
		empty
	};
	
    class parser {
    	vector<pair<string, elem>> lines;
    	string html_doc;
    	string tex_doc;
    	string fountain_doc;
    	
    	bool starts_with(string, string);
		bool ends_with(string, string);
		
		bool is_action(string);
		bool is_scene_heading(string);
		bool is_character(string);
		bool is_parenthetical(string);
		bool is_dialogue(string);
		bool is_transition(string);
    	
    public:
    	parser(string);
        string html();
        string tex();
        string fountain();
    };
}

#endif
