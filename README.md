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
- [x] Texto entre paréntesis´
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

Una mejor opción sería usar tu navegador web u otra herramienta como el mismo wkhtmltopdf, [weasyprint](https://weasyprint.org/) o [prince](https://www.princexml.com/).

## Notas

El CSS del HTML no es completamente fiel a las medidas exactas de un guión (hasta alli llego mi conocimiento de CSS). Para tener un guión profesional prefiera el formato FDX, que es el estándar.

Aún no se implemento la prueba de errores (`try catch` o `if else`), así que no tengo idea de como pueda fallar el programa, o si fallarás si algo ocurre mal.

Deben haber varios errores de formato en el archivo de salida ya que la forma de parsear el archivo es lo más básica posible, por lo que, por ejemplo, no puede hacer algo tan facil como entender las negritas (aún). Este es solo un proyecto de principiante.

La tipografia elegida no es la ideal.

Planeo portarlo a C++20 cuando GCC lo soporte, así que la base del codigo cambiará mucho.

Si tienes sugerencias o aportes me ayudaría mucho que abras un nuevo *issue*.

## Créditos

- [Wkhtmltopdf](https://wkhtmltopdf.org)
- [Argh!](https://github.com/adishavit/argh)
- [Fountain](https://fountain.io)
- [Screenplain](http://www.screenplain.com/)
- [GNU software](https://www.gnu.org/software/software.html)
