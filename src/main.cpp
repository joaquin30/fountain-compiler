#include <iostream>
#include <string>
#include "fountain.hpp"
#include "argh.hpp"

int main(int argc, char** argv) {
    argh::parser cmd(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
    if (cmd[{"-h", "--help"}]) {
        std::cout << R"(USO:
    fountain <archivo> [-o, --output <archivo>] [-f, --format <formato>] [-h, --help] [-V, --version]

OPCIONES:
    -h, --help              Mostrar la ayuda
    -V, --version           Mostrar la versión
    -o, --output <archivo>  Seleccionar archivo de salida
    -f, --format <formato>  Seleccionar formato (html, fdx, pdf) de salida

Opciones inexistentes o mal escritas son ignoradas.
)";

    } else if (cmd[{"-V", "--version"}]) {
#ifdef PDF_SUPPORT
    std::cout << "Versión 0.0.1 (Soporte de PDF activado)\n";
#else
    std::cout << "Versión 0.0.1 (Soporte de PDF desactivado)\n";
#endif
    } else if (!cmd(1)) {
        std::cerr << "Por lo menos debe haber un argumento\n";
    } else {
        fountain::parser out(cmd[1], cmd({"-o", "--output"}).str());
        if (!cmd({"-f", "--format"}))  {
            out.html_output();
        } else {
            if (cmd({"-f", "--format"}).str() == "html") {
                out.html_output();
            } else if (cmd({"-f", "--format"}).str() == "fdx") {
                out.fdx_output();
            } else if (cmd({"-f", "--format"}).str() == "pdf") {
#ifdef PDF_SUPPORT
                out.pdf_output();
#else
                std::cout << "Exportación a PDF no soportada\n";
#endif
            } else {
                std::cerr << "Formato inválido\n";
                return 1;
            }
        }
    }

    return 0;
}
