#include "fountain.hpp"

namespace fountain {
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
			return isupper(i) || !isalpha(i);
		});
	}

	bool parser::is_parenthetical(string s) {
		if (s.size() < 2)
			return false;
			
		return s[0] == '(' && s.back() == ')';
	}

	bool parser::is_transition(string s) {
		if (s.size() < 3)
			return false;

		//capaz solo termine con ":"
		return ends_with(s, "TO:") && all_of(begin(s), end(s), [](auto i) {
			return isupper(i) || !isalpha(i);
		});
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
		else if (is_transition(lines[0].first))
			lines[0].second = elem::transition;
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
			} else if (is_transition(s) && lines[i-1].second == elem::empty) {
				e = elem::transition;
			} else if (is_character(s) && lines[i-1].second == elem::empty) {
				e = elem::character;
			} else if (is_parenthetical(s) &&
					   lines[i-1].second == elem::character) {
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
	}
	
	string parser::html() {
		if (!html_doc.empty())
			return html_doc;
		
		for (auto [i, j] : lines) {
			switch (j) {
			case elem::action:
				html_doc += "<p class=\"action\">" + i + "</p>\n";
				break;
			case elem::scene_heading:
				html_doc += "<p class=\"scene_heading\">" + i + "</p>\n";
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
			case elem::transition:
				html_doc += "<p class=\"transition\">" + i + "</p>\n";
				break;	
			default:
				break;
			}
		}
		
		return html_doc;
	}
}

