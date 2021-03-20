#ifndef FOUNTAIN_HPP
#define FOUNTAIN_HPP

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace fountain {
	using namespace std;
	enum class elem {
		action,
		scene_heading,
		character,
		parenthetical,
		dialogue,
		transition,
		centered_text,
		page_break,
		empty //temporal
	};

    class parser {
    	vector<pair<string, elem>> lines;
    	string html_doc;
    	string tex_doc;
    	string fountain_doc;

    	bool starts_with(string_view, string_view);
		bool ends_with(string_view, string_view);
		string trim(string_view);

		bool is_action(string_view);
		bool is_scene_heading(string_view);
		bool is_character(string_view);
		bool is_parenthetical(string_view);
		bool is_dialogue(string_view);
		bool is_transition(string_view);
		bool is_centered_text(string_view);
		bool is_page_break(string_view);

    public:
    	parser(const char*);
        string html();
        string pdf();
		string fdx();
        string fountain();
        vector<pair<string, elem>> data();
    };
}

#endif
