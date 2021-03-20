#include "fountain.hpp"

namespace fountain {
	using namespace std;
	bool parser::starts_with(string_view s1, string_view s2) {
		if (s1.size() < s2.size())
			return false;

		for (size_t i = 0, len = s2.size(); i < len; ++i) {
			if (s1[i] != s2[i])
				return false;
		}

		return true;
	}

	bool parser::ends_with(string_view s1, string_view s2) {
		if (s1.size() < s2.size())
			return false;

		for (size_t i = s1.size() - 1, j = s2.size() - 1; j >= 0; --i, --j) {
			if (s1[i] != s2[j])
				return false;
		}

		return true;
	}

	string parser::trim(string_view s) {
		int i = 0, j = s.size() - 1;
		while (isspace(s[i]))
			i++;

		while (isspace(s[j]))
			j--;

		return string(s.substr(i, j - i + 1));
	}

	bool parser::is_action(string_view s) {
		return s[0] == '!';
	}

	bool parser::is_scene_heading(string_view s) {
		if (s[0] == '.')
			return true;

		auto tmp = {"INT", "EXT", "EST", "INT./EXT", "INT/EXT", "I/E",
					"int", "ext", "est", "int./ext", "int/ext", "i/e"};
		for (auto i : tmp) {
			if (starts_with(s, i))
				return true;
		}

		return false;
	}

	bool parser::is_character(string_view s) {
		if (s[0] == '@')
			return true;

		int i = 0;
		for (auto c : s) {
			if (islower(c))
				return false;
			else if (isupper(c))
				i++;
		}

		return i > 0;
	}

	bool parser::is_parenthetical(string_view s) {
        return s[0] == '(' && s.back() == ')';
	}

	bool parser::is_transition(string_view s) {
		return (ends_with(s, "A:") || ends_with(s, "TO:")) &&
                all_of(s.begin(), s.end(), [](auto i) {
			        return isupper(i) || !isalpha(i);
		        });
	}

	bool parser::is_centered_text(string_view s) {
		return s[0] == '>' && s.back() == '<';
	}

	bool parser::is_page_break(string_view s) {
		int i = 0;
        for (auto c : s) {
            if (c != '=')
                break;

            i++;
        }

        return i >= 3 && i == s.size();
	}

	parser::parser(const char* file) {
		ifstream input(file);
		string line;
		while (getline(input, line)) {
			if (line.empty())
				lines.push_back({trim(line), elem::empty});
            else
				lines.push_back({trim(line), elem::action});
		}

		// para no comprobar si estoy en la 1ra linea
		if (is_action(lines[0].first)) {
			lines[0].first.erase(lines[0].first.begin());
        } else if(is_page_break(lines[0].first)) {
            lines[0].first = "";
            lines[0].second = elem::page_break;
        } else if(is_centered_text(lines[0].first)) {
            lines[0].first = lines[0].first.substr(1, lines[0].first.size() - 2);
			lines[0].second = elem::centered_text;
        } else if(is_transition(lines[0].first)) {
			lines[0].second = elem::transition;
        } else if (is_scene_heading(lines[0].first)) {
			lines[0].second = elem::scene_heading;
		} else if (is_character(lines[0].first)) {
			lines[0].second = elem::character;
        }

		//codigo espageti
		for (size_t i = 1; i < lines.size(); ++i) {
			auto& [s, e] = lines[i];
			auto& [prev_s, prev_e] = lines[i-1];
            if (e == elem::empty) {
                continue;
            } else if (is_action(s)) {
				s.erase(s.begin());
			} else if (is_page_break(s)) {
                s = "";
                e = elem::page_break;
            } else if (is_centered_text(s)) {
                s = trim(s.substr(1, s.size() - 2));
				e = elem::centered_text;
			} else if (is_transition(s) && prev_e == elem::empty) {
				e = elem::transition;
			} else if (is_scene_heading(s) && prev_e == elem::empty) {
				if (s[0] == '.')
					s.erase(s.begin());

				e = elem::scene_heading;
			} else if (is_character(s) && prev_e == elem::empty) {
				if (s[0] == '@')
					s.erase(s.begin());

				e = elem::character;
			} else if (is_parenthetical(s) &&
					   (prev_e == elem::character ||
					    prev_e == elem::dialogue)) {
                s = s.substr(1, s.size() - 2);
				e = elem::parenthetical;
			} else if (prev_e == elem::character ||
					   prev_e == elem::parenthetical) {
				e = elem::dialogue;
			} else if (prev_e == elem::dialogue ||
					   prev_e == elem::action) {
				prev_s.push_back('\n');
				prev_s += s;
				lines.erase(lines.begin() + i);
				i--;
			}
		}

		if (lines.back().second == elem::character)
			lines.back().second = elem::action;

		//ineficiente
		for (size_t i = 0, len = lines.size() - 1; i < len; ++i) {
			if (lines[i].second == elem::character &&
				!(lines[i+1].second == elem::dialogue ||
				lines[i+1].second == elem::parenthetical))
				lines[i].second = elem::action;
		}

	}

	string parser::html() {
		if (!html_doc.empty())
			return html_doc;

		html_doc = R"(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="preconnect" href="https://fonts.gstatic.com">
<link href="https://fonts.googleapis.com/css2?family=Courier+Prime:ital,wght@0,400;0,700;1,400;1,700&display=swap" rel="stylesheet">
<style>
@page {
    size: A4;
    margin-top: 2.6cm;
    margin-bottom: 3.1cm;
	margin-left: 3.8cm;
	margin-right: 2.6cm;
}

@media print {
    body {
        margin: 0;
    }

	.page_break {
		page-break-before: always;
	}
}

body {
	font-size: 12pt;
	font-family: 'Courier Prime', monospace;
    width: 15.35cm;
	margin: 10px auto;
}

p {
	margin: 0;
}

.scene_heading {
	font-weight: bold;
}

.character {
    margin-left: 5.3cm;
}

.parenthetical {
    margin-left: 3.95cm;
    width: 6.9cm;
}

.dialogue {
    margin-left: 2.65cm;
    width: 9.55cm;
}

.transition {
	text-align: right;
}

.centered_text {
    text-align: center;
}
</style>
</head>
<body>
)";

		for (auto [i, j] : lines) {
			switch (j) {
			case elem::action:
				html_doc += "<p>" + i + "</p>\n";
				break;

			case elem::scene_heading:
				html_doc += "<p class=\"scene_heading\">" + i + "</p>\n";
				break;

			case elem::character:
				html_doc += "<p class=\"character\">" + i + "</p>\n";
				break;

			case elem::parenthetical:
				html_doc += "<p class=\"parenthetical\">(" + i + ")</p>\n";
				break;

			case elem::dialogue:
				html_doc += "<p class=\"dialogue\">" + i + "</p>\n";
				break;

            case elem::transition:
                html_doc += "<p class=\"transition\">" + i + "</p>\n";
				break;

            case elem::centered_text:
                html_doc += "<p class=\"centered_text\">" + i + "</p>\n";
				break;

            case elem::page_break:
                html_doc += "<div class=\"page_break\"></div>\n";
				break;

			default:
				html_doc += "<br>\n";
				break;
			}
		}

		html_doc += "</body>\n</html>\n";
		return html_doc;
	}

	vector<pair<string, elem>> parser::data() {
		return lines;
	}
}
