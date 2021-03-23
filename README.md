# Compilador de Fountain

[Fountain](https://fountain.io) es un formato ligero de marcado para escribir guiones de películas. Este proyecto es un intento para escribir un compilador que soporte algunas características de Fountain en C++17.

## Características

- [x] Exportacion a HTML
- [X] Exportacion a PDF
- [ ] Exportacion a Final Draft
- [x] Acciones
- [x] Cabeceras de escena
- [x] Personajes
- [x] Diálogos
- [x] Texto entre paréntesis
- [X] Transiciones
- [ ] Diálogos dobles
- [ ] Líricas
- [X] Texto centrado
- [ ] Énfasis
- [ ] Título de la página
- [X] Saltos de página

## Uso

El binario es generado en la carpeta `build`. Para mostrar su uso utilize la opción `-h`.
Es necesario la libreria [Wkhtmltopdf](https://wkhtmltopdf.org) para generar el PDF, pero su compilación es complicada, por eso esta desactivada por defecto.
Si tienes la libreria dinámica `libhtmltox` disponible en tu sistema, para compilar este proyecto con soporte de PDF ubique la libreria en el folder `lib` y ejecute `make PDF_SUPPORT=1`.
Pero no lo recomiendo, basta con tu navegador para generar el PDF.
Si lo quieres hacer por comando, usa la misma herramienta pero como programa (que es mucho más fácil), o usa otras opciones, como [Weasyprint](https://weasyprint.org/) o [Prince](https://www.princexml.com/).

## Para hacer

- Automatizar las pruebas
- Corregir CSS
-
- Optimizar el código
