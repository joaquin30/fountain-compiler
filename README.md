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

Es necesario la libreria [wkhtmltopdf](https://wkhtmltopdf.org) para generar el PDF, pero como su compilación es complicada esta desactivada por defecto.

No lo recomiento pero si tienes la libreria dinámica `libhtmltox` disponible en tu sistema, para compilar este proyecto con soporte de PDF copia la libreria en el folder `lib` y ejecute `make PDF_SUPPORT=1`. Si tu sistema no es linux probablemente salgan errores, pero deberian ser faciles de solucionar.

Una mejor opción seria tu navegador web u otra herramienta como el mismo wkhtmltopdf, [weasyprint](https://weasyprint.org/) o [prince](https://www.princexml.com/).

## Para hacer

- Corregir CSS
- Optimizar el código
