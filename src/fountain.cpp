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

		for (int i = s1.size() - 1, j = s2.size() - 1; j >= 0; --i, --j) {
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

        return i >= 3 && i == (int) s.size();
	}

	parser::parser(const string& input, const string& output) {
        output_file = output;
        string line;
		if (input == "-") {
            while (getline(cin, line)) {
                if (line.empty())
                    data.push_back({trim(line), elem::empty});
                else
                    data.push_back({trim(line), elem::action});
            }
        } else {
            ifstream in(input);
            while (getline(in, line)) {
                if (line.empty())
                    data.push_back({trim(line), elem::empty});
                else
                    data.push_back({trim(line), elem::action});
            }
        }

		// para no comprobar si estoy en la 1ra linea
		if (is_action(data[0].first)) {
			data[0].first.erase(data[0].first.begin());
        } else if(is_page_break(data[0].first)) {
            data[0].first = "";
            data[0].second = elem::page_break;
        } else if(is_centered_text(data[0].first)) {
            data[0].first = data[0].first.substr(1, data[0].first.size() - 2);
			data[0].second = elem::centered_text;
        } else if(is_transition(data[0].first)) {
			data[0].second = elem::transition;
        } else if (is_scene_heading(data[0].first)) {
			data[0].second = elem::scene_heading;
		} else if (is_character(data[0].first)) {
			data[0].second = elem::character;
        }

		//codigo espageti
		for (size_t i = 1; i < data.size(); ++i) {
			auto& [s, e] = data[i];
			auto& [prev_s, prev_e] = data[i-1];
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
				data.erase(data.begin() + i);
				i--;
			}
		}

		if (data.back().second == elem::character)
			data.back().second = elem::action;

		//ineficiente
		for (size_t i = 0, len = data.size() - 1; i < len; ++i) {
			if (data[i].second == elem::character &&
				!(data[i+1].second == elem::dialogue ||
				data[i+1].second == elem::parenthetical))
				data[i].second = elem::action;
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
.page_break {
    page-break-before: always;
}

body {
	font-size: 12pt;
	font-family: 'Courier Prime', monospace;
    width: 15.3cm;
    margin: 1cm 1cm 1cm 1cm;
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

		for (auto [i, j] : data) {
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

    string parser::fdx() {
        if (!fdx_doc.empty())
            return fdx_doc;

        fdx_doc = R"(
)";
        return fdx_doc;
    }

    void parser::html_output() {
        if (output_file.empty()) {
            cout << html();
        } else {
            ofstream out(output_file);
            out << html();
        }
    }

	void parser::pdf_output() {
		wkhtmltopdf_init(false);
		auto gs = wkhtmltopdf_create_global_settings();
        if (output_file.empty())
		    wkhtmltopdf_set_global_setting(gs, "out", "-");
        else
		    wkhtmltopdf_set_global_setting(gs, "out", output_file.c_str());

        wkhtmltopdf_set_global_setting(gs, "size.pageSize", "A4");
        wkhtmltopdf_set_global_setting(gs, "margin.top", "1cm");
        wkhtmltopdf_set_global_setting(gs, "margin.bottom", "1.5cm");
        wkhtmltopdf_set_global_setting(gs, "margin.left", "2.3cm");
        wkhtmltopdf_set_global_setting(gs, "margin.right", "1.4cm");
        wkhtmltopdf_set_global_setting(gs, "header.fontSize", "12");
		auto os = wkhtmltopdf_create_object_settings();
		auto c = wkhtmltopdf_create_converter(gs);
		wkhtmltopdf_add_object(c, os, html().c_str());
        wkhtmltopdf_convert(c);
        const unsigned char* tmp = nullptr;
        wkhtmltopdf_get_output(c, &tmp);
        wkhtmltopdf_destroy_converter(c);
        wkhtmltopdf_deinit();
	}

    void parser::fdx_output() {
        if (output_file.empty()) {
            cout << fdx();
        } else {
            ofstream out(output_file);
            out << fdx();
        }
    }
}
