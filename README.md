# Compilador de Fountain

[Fountain](https://fountain.io) es un formato ligero de marcado para escribir guiones de películas. Este es un simple intento de crear un compilador simple con C++17 para aprender.

## Características soportadas

- [x] Acciones
- [x] Cabeceras de escena
- [x] Personajes
- [x] Diálogos
- [x] Texto entre parentesis
- [x] Transiciones
- [ ] Diálogos dobles
- [ ] Líricas
- [ ] Texto centrado
- [ ] Énfasis
- [ ] Título de la página
- [ ] Saltos de página
- [ ] Saltos de linea
- [ ] Etc...

## Uso

Para compilar el proyecto use (cxx es su compilador de C++17): `cxx *.cpp -o fountain`

Y para ejecutarlo, como ejemplo usaremos el archivo para probar el compilador: `./fountain "test.fountain"`

Nos debe salir texto en HTML con el contenido de `test.fountain`. Aún falta enlazarlo con css para que se vea como un guión en el navegador.

Para usarlo como librería solo necesitamos los archivos `fountain.hpp` y `fountain.cpp` y lo enlazas a tu proyecto con `#include "fountain.hpp"`.

La única clase es `fountain::parser`, se construye con la direccion del archivo y tiene 3 funciones: `html()`, `tex()` y `fountain()`.

Ejemplo:

```cpp
#include <iostream>
#include "fountain.h"

int main() {
	fountain::parser file("test.fountain");
	cout << file.html();
	return 0;
}
```
