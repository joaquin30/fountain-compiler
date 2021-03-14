#include "fountain.hpp"

namespace fountain {
	using namespace std;
	bool parser::starts_with(string s1, string s2) {
		if (s1.size() < s2.size())
			return false;
		
		for (int i = 0, len = s2.size(); i < len; ++i) {
			if (s1[i] != s2[i])
				return false;
		}
		
		return true;
	}

	// lo elimino?, no lo uso
	bool parser::ends_with(string s1, string s2) {
		if (s1.size() < s2.size())
			return false;
		
		for (int i = s1.size() - 1, j = s2.size() - 1; j >= 0; --i, --j) {
			if (s1[i] != s2[j])
				return false;
		}
		
		return true;
	}

	bool parser::is_action(string s) {
		if (s.size() < 1)
			return false;
			
		return s[0] == '!';
	}

	bool parser::is_scene_heading(string s) {
		if (s.size() < 8)
			return false;

		auto tmp = {"INT", "EXT", "EST", "INT./EXT", "INT/EXT", "I/E"};
		for (auto i : tmp) {
			if (starts_with(s, i))
				return true;
		}
		
		return false;
	}

	bool parser::is_character(string s) {
		if (s.size() < 1)
			return false;
		
		return all_of(begin(s), end(s), [](auto i) {
			return isupper(i);
		});
	}

	bool parser::is_parenthetical(string s) {
		if (s.size() < 2)
			return false;
			
		return s[0] == '(' && s.back() == ')';
	}

	parser::parser(string file) {
		ifstream input(file);
		string line;
		while (getline(input, line))
			lines.push_back({line, elem::action});
		
		// para no comprobar si estoy en la 1ra linea
		if (is_action(lines[0].first))
			lines[0].first.erase(begin(lines[0].first));
		else if (is_scene_heading(lines[0].first))
			lines[0].second = elem::scene_heading;
		else if (is_character(lines[0].first))
			lines[0].second = elem::character;
		else if (lines[0].first.empty())
			lines[0].second = elem::empty;
		
		//codigo espageti
		for (int i = 1; i < lines.size(); ++i) {
			auto& [s, e] = lines[i];
			if (s.empty()) {
				e = elem::empty;
			} else if (is_action(s)) {
				s.erase(begin(s));
			} else if (is_scene_heading(s)) {
				e = elem::scene_heading;
			} else if (is_character(s) && lines[i-1].second == elem::empty) {
				e = elem::character;
			} else if (is_parenthetical(s) &&
					   (lines[i-1].second == elem::character ||
					   lines[i-1].second == elem::dialogue)) {
				e = elem::parenthetical;
			} else if (lines[i-1].second == elem::character ||
					   lines[i-1].second == elem::parenthetical) {
				e = elem::dialogue;
			} else if (lines[i-1].second == elem::dialogue ||
					   lines[i-1].second == elem::action) {
				lines[i-1].first.push_back(' ');  
				lines[i-1].first += s;
				lines.erase(begin(lines) + i);
				i--;
			}
		}
		
		for (int i = 0; i < lines.size(); ++i) {
			if (lines[i].second == elem::empty) {
				lines.erase(begin(lines) + i);
				i--;
			}
		}
	}
	
	string parser::html() {
		if (!html_doc.empty())
			return html_doc;
			
		html_doc = R"(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0" />
<style>
html {
	font-size: 62.5%;
}
body {
	font-size: 16.65px;
	width: 61rem;
	font-family: Courier New;
	margin-bottom: 0;
}

p {
	margin-top: 16.65px;
	margin-bottom: 0;
}

.character {
	margin-left: 27rem;
	
}

.parenthetical {
	margin-left: 22rem;
	width: 17rem; 
	margin-top: 0;
	
}

.dialogue {
	margin-left: 15rem;
	width: 32rem;
	margin-top: 0;
}
</style>
</head>
<body>
)";

		for (auto [i, j] : lines) {
			switch (j) {
			case elem::action:
			case elem::scene_heading:
				html_doc += "<p>" + i + "</p>\n";
				break;
				
			case elem::character:
				html_doc += "<p class=\"character\">" + i + "</p>\n";
				break;
				
			case elem::parenthetical:
				html_doc += "<p class=\"parenthetical\">" + i + "</p>\n";
				break;
				
			case elem::dialogue:
				html_doc += "<p class=\"dialogue\">" + i + "</p>\n";
				break;
				
			default:
				break;
			}
		}
		
		html_doc += "</body>\n</html>\n";
		return html_doc;
	}
	
	string parser::tex() {
		if (!tex_doc.empty())
			return tex_doc;
			
		tex_doc = R"(\documentclass{screenplay}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\begin{document}
)";

		switch (lines[0].second) {
		case elem::action:
		case elem::scene_heading:
			tex_doc += lines[0].first + "\n\n";
			break;
		case elem::character:
			tex_doc += "\\begin{dialogue}{" + lines[0].first + "}\n";
			break;
		default:
			break;
		}

		for (int i = 1, len = lines.size(); i < len; ++i) {
			auto& [s, e] = lines[i];
			switch (e) {
			case elem::action:
			case elem::scene_heading:
				if (lines[i-1].second == elem::dialogue ||
					lines[i-1].second == elem::parenthetical ||
					lines[i-1].second == elem::character)
					tex_doc += "\\end{dialogue}\n\n" + s + "\n\n";
				else
					tex_doc += s + "\n\n";
					
				break;
				
			case elem::character:
				if (lines[i-1].second == elem::dialogue ||
					lines[i-1].second == elem::parenthetical ||
					lines[i-1].second == elem::character)
					tex_doc += "\\end{dialogue}\n\n\\begin{dialogue}{" + s + "}\n";
				else
					tex_doc += "\\begin{dialogue}{" + s + "}\n";
					
				break;
				
			case elem::parenthetical:
				tex_doc += "\\paren{" + s.substr(1, s.size() - 2) + "}\n";
				break;
				
			case elem::dialogue:
				if (i == len - 1)
					tex_doc += s + "\n\\end{dialogue}\n";
				else
					tex_doc += s + "\n";
					
				break;
				
			default:
				break;
			}
		}
		
		tex_doc += "\\end{document}\n";
		return tex_doc;
	}
	
	string parser::fountain() {
		if (!fountain_doc.empty())
			return fountain_doc;
		
		switch (lines[0].second) {
		case elem::action:
		case elem::scene_heading:
			fountain_doc += lines[0].first + "\n\n";
			break;
			
		default:
			fountain_doc += lines[0].first + "\n";
			break;
		}

		for (int i = 1, len = lines.size(); i < len; ++i) {
			auto& [s, e] = lines[i];
			switch (e) {
			case elem::action:
			case elem::scene_heading:
				if (lines[i-1].second == elem::dialogue ||
					lines[i-1].second == elem::parenthetical ||
					lines[i-1].second == elem::character)
					fountain_doc += "\n" + s + "\n\n";
				else
					fountain_doc += s + "\n\n";
					
				break;
				
			case elem::character:
				if (lines[i-1].second == elem::dialogue ||
					lines[i-1].second == elem::parenthetical ||
					lines[i-1].second == elem::character)
					fountain_doc += "\n" + s + "\n";
				else
					fountain_doc += s  + "\n";
					
				break;
				
			default:
				fountain_doc += s  + "\n";
				break;
			}
		}
		
		return fountain_doc;
	}
	
	vector<pair<string, elem>> parser::data() {
		return lines;
	}
}
