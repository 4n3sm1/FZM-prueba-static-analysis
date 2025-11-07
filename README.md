# 🧩 Prueba técnica — Análisis de código C (CERT C / MISRA C)

**IKERLAN - Métodos de Confiabilidad y ciberseguridad**.

---

## 🎯 Objetivos

1. Analizar el código C proporcionado e identificar defectos relacionados con **seguridad, robustez o mantenibilidad**.  
2. Fundamentar cada hallazgo con referencia a **principios de buenas prácticas** o reglas de estándares como **CERT C** y **MISRA C:2012**.  
3. Proponer **correcciones seguras y justificadas**, manteniendo la funcionalidad.  
4. Documentar **desviaciones justificadas** cuando consideres que incumplir una regla está técnicamente razonado.  
5. Implementar o verificar un **CRC** para proteger la configuración en memoria y analizar su impacto en la integridad del sistema.

---

## 🧰 Contexto técnico

El proyecto contiene una pequeña aplicación escrita en C que:

- Carga una configuración desde un fichero de texto.  
- Utiliza un *ring buffer* compartido entre dos hilos (*producer/consumer*).  
- Calcula un CRC de ejemplo.  
- Contiene **defectos intencionados** de memoria, concurrencia, conversión de tipos y seguridad.

### Estructura del proyecto

```
├── src/
│   ├── buffer.c / buffer.h   →  implementación del ring buffer
│   ├── crc.c / crc.h         →  cálculo de CRC-32 (con defectos deliberados)
│   ├── config.c / config.h   →  carga de configuración (parsing inseguro)
│   └── main.c                →  ejecución con hilos y cálculo de CRC
├── tests/
│   ├── test.c                →  pruebas mínimas de ejecución
│   └── example.cfg           →  fichero de configuración de ejemplo
├── Makefile
└── README.md
```


## ⚙️ Compilación y ejecución

```bash
make
./bin/app ./tests/example.cfg
```

Prueba mínima:

```bash
make test
./bin/test
```

---

## 🧮 Tareas

1. **Analiza el código fuente** y documenta los hallazgos:
   - Archivo y línea.
   - Descripción del defecto y posible impacto.
   - Regla, principio o práctica relevante (CERT C, MISRA C, u otra).  
   - Justificación técnica.  
   - Propuesta de corrección o mitigación.

2. **Propón correcciones** o refactorizaciones que solucionen los problemas sin alterar la funcionalidad prevista.

3. **Documenta las desviaciones justificadas** si decides mantener una práctica que incumpla una regla de forma controlada (por eficiencia, compatibilidad o contexto).

4. **Integridad mediante CRC**  
   - Implementa o evalúa el uso de un **CRC** para proteger la configuración (`AppConfig`).  
   - Calcula el CRC sobre una **vista determinista** de la configuración, es decir, una representación binaria **sin bytes de relleno (*padding*) ni memoria no inicializada**, de modo que el valor del CRC dependa **solo de los datos reales**.  
   - Analiza qué tipos de violaciones podrían causar un CRC erróneo, qué regla violan, por qué afecta al CRC y qué riesgo de seguridad implica si la configuración fuera crítica.

---

## 🧾 Entregables

- **Informe técnico** que incluya:  
  - Lista priorizada de hallazgos con explicación y regla (CERT C, MISRA C, u otra) asociada.  
  - Riesgo, impacto y evidencia (línea, fragmento o razonamiento).  
  - Propuesta de corrección o justificación de la desviación documentada.  
  - Análisis de Integridad mediante CRC para proteger la configuración (`AppConfig`)
- **Nueva versión del software con las correcciones más relevantes y la implementación del CRC.  

### 🗂️ Entrega 

Puedes crear un **repositorio público** (por ejemplo, en GitHub o GitLab) con el contenido del ejercicio.

1. Sube al repositorio el código, tus correcciones y el informe técnico.  
2. Compártenos el enlace público del repositorio (si prefieres no hacerlo público, también puedes entregar un archivo `.zip` con la misma estructura.)

---

## 💡 Notas finales

- No se exige el uso de ninguna herramienta específica.
- Puedes usar, si lo deseas, herramientas de análisis estático como referencia para tu trabajo (por ejemplo `clang-tidy`, `cppcheck`, `CodeChecker`, `Infer`, `flawfinder`, `Coverity`, etc.).  
- Si utilizas alguna herramienta, menciona los comandos o pasos para reproducir tus resultados.  
- El propósito de esta prueba es medir tu capacidad para razonar sobre la seguridad y la calidad del software, no memorizar reglas ni dominar una herramienta concreta.

