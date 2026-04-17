# Configuración de la pantalla ILI9488 para el STM32 G474 RE - Proyecto Helios

## 📋 Descripción del Proyecto

Este proyecto contiene la configuración y driver para controlar la pantalla táctil **ILI9488** mediante el microcontrolador **STM32 G474 RE**. El código está diseñado para el **Proyecto Helios**, un club de generación espontánea de la UPV que está construyendo un **coche eléctrico**.

### Características principales:
- ✅ Driver completo para pantalla ILI9488 (3.5 pulgadas)
- ✅ Comunicación SPI de alta velocidad
- ✅ Soporte para pantalla táctil
- ✅ Funciones de dibujo básicas y avanzadas
- ✅ Sistema de fuentes personalizables
- ✅ Optimizado para STM32 G474 RE

---

## 🔧 Requisitos

### Hardware:
- **Microcontrolador**: STM32 G474 RE
- **Pantalla**: ILI9488 (3.5" LCD táctil)
- **Conexión**: SPI + pines de control GPIO
- **Alimentación**: 3.3V

### Software:
- STM32CubeIDE o equivalente
- STM32CubeMX para configuración de pines
- Compilador ARM GCC
- Herramientas de programación (ST-Link)

---

## 📁 Estructura del Proyecto

```
Screem_for_event_Helios/
├── Core/
│   ├── Inc/              # Archivos de cabecera
│   │   ├── ili9488.h     # Driver principal de ILI9488
│   │   ├── font.h        # Definiciones de fuentes
│   │   └── ...
│   └── Src/              # Implementación
│       ├── ili9488.c     # Funciones del driver
│       ├── main.c        # Programa principal
│       └── ...
├── Drivers/              # Drivers STM32 HAL
├── Makefile              # Configuración de compilación
└── README.md             # Este archivo
```

---

## ⚡ Instalación y Configuración

### 1. Clonar el repositorio
```bash
git clone https://github.com/ISMAELMUSTMOUA/Screem_for_event_Helios.git
cd Screem_for_event_Helios
```

### 2. Configurar pines en STM32CubeMX
- Habilitar SPI (SPI1 o SPI2 según tu configuración)
- Configurar velocidad: 10-20 MHz recomendado
- Asignar pines GPIO para control:
  - **CS**: Chip Select
  - **DC**: Data/Command
  - **RST**: Reset
  - **MOSI, MISO, CLK**: SPI

### 3. Compilar el proyecto
```bash
make clean
make
```

---

## 🎨 Uso

### Inicialización de la pantalla:
```c
ili9488_init();
ili9488_fill_screen(BLACK);
```

### Dibujar en la pantalla:
```c
// Dibujar un rectángulo
ili9488_draw_rectangle(10, 10, 100, 100, RED);

// Escribir texto
ili9488_draw_string(20, 30, "Helios Proyecto", WHITE, BLACK);

// Dibujar línea
ili9488_draw_line(0, 0, 320, 480, BLUE);
```

### Actualizar pantalla:
```c
ili9488_update_display();
```

---

## 📊 Configuración de Velocidad SPI

Para mejor rendimiento, ajusta la velocidad SPI:

| Velocidad | Rendimiento | Estabilidad |
|-----------|------------|------------|
| 5 MHz     | Bajo       | Excelente  |
| 10 MHz    | Medio      | Bueno      |
| 20 MHz    | Alto       | Bueno      |
| 40 MHz+   | Muy Alto   | Requiere pruebas |

---

## 🐛 Troubleshooting

### La pantalla no muestra nada:
1. Verificar conexión SPI (MOSI, MISO, CLK)
2. Verificar pines de control (CS, DC, RST)
3. Comprobar voltaje de alimentación (3.3V)
4. Revisar orden de inicialización

### La pantalla muestra colores incorrectos:
1. Verificar formato de color en el driver
2. Comprobar orden de bytes SPI
3. Revisar configuración de endianness

### Pantalla táctil no responde:
1. Verificar comunicación I2C/SPI del touch controller
2. Revisar calibración táctil
3. Comprobar interrupciones GPIO

---

## 📝 Notas Importantes

- El código está optimizado para el **Proyecto Helios**
- Consumo SPI típico: ~50-100 mA
- Velocidad de refresco: 60 FPS recomendado
- Compatibilidad: STM32 G474 RE con mínimo 256KB de RAM

---

## 🚗 Sobre Proyecto Helios

**Helios** es un club de generación espontánea de la **Universidad Politécnica de Valencia (UPV)** dedicado al diseño y construcción de un **coche eléctrico de competición**. Este proyecto forma parte del sistema de visualización e interfaz del vehículo.

**Sitio Web**: [Helios UPV](https://heliosraceupv.com)  
**Contacto**: heliosraceupv@gmail.com

---

## 📄 Licencia

Este proyecto es parte del Proyecto Helios UPV. Consulta el archivo LICENSE para más detalles.

---

## 👨‍💻 Autor

**Ismael Mustmoua**  
GitHub: [@ISMAELMUSTMOUA](https://github.com/ISMAELMUSTMOUA)

---

## 🔗 Referencias

- [Datasheet ILI9488](https://www.ilitek.com/)
- [STM32 G474 Reference Manual](https://www.st.com/)
- [STM32CubeIDE Documentation](https://www.st.com/en/development-tools/stm32cubeide.html)
