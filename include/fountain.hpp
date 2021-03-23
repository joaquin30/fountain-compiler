#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "wkhtmltox/pdf.h"

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
        vector<pair<string, elem>> data;
        string output_file;
        string html_doc;
        string fdx_doc;
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
        string html();
        string fdx();

    public:
        parser(const string&, const string&);
        void html_output();
        void pdf_output();
		void fdx_output();
    };
}
