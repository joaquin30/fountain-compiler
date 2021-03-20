# Compilador de Fountain

[Fountain](https://fountain.io) es un formato ligero de marcado para escribir guiones de películas. Este proyecto es un intento para escribir un compilador que soporte algunas características de Fountain en C++17.

## Características

- [x] Exportacion a HTML
- [ ] Exportacion a PDF
- [ ] Exportacion a Final Draft
- [x] Acciones
- [x] Cabeceras de escena
- [x] Personajes
- [x] Diálogos
- [x] Texto entre parentesis
- [X] Transiciones
- [ ] Diálogos dobles
- [ ] Líricas
- [X] Texto centrado
- [ ] Énfasis
- [ ] Título de la página
- [X] Saltos de página
- [ ] Saltos de linea

## Uso

Para compilar el proyecto, es necesario GNU Make. Si tienes otro compilador (como Clang), cambia la variable `CXX` en el archivo `Makefile`. Por ahora el binario solo acepta un argumento, que es el nombre del archivo a compilar, e imprime HTML.

## Para hacer

- Rehacer la función para detectar un personaje
- Resolver el bug de los personajes solos
- Enlazar la librería [wkhtmltopdf](https://wkhtmltopdf.org)
- Automatizar las pruebas
- Mejorar el CSS
