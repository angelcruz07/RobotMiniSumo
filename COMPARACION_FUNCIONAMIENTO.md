# Comparación de Funcionamiento: FondoNegro.ino vs SumoAutonomoFondoNegro.ino

## Resultado: **NO HAY DIFERENCIAS EN EL FUNCIONAMIENTO**

Ambos códigos tienen **exactamente el mismo comportamiento funcional**. Solo difieren en la organización y legibilidad del código.

---

## Lógica de Funcionamiento (idéntica en ambos)

### Comportamiento del Robot:

1. **Lee los sensores** (línea negra en los pines A5, A6, A7)

2. **Mide la distancia** al obstáculo delantero con el sensor ultrasónico

3. **Decisión basada en distancia:**

   #### Si distancia < 150 cm (obstáculo cerca):
   - ✅ **Detecta línea negra** → Realiza maniobra evasiva:
     - Se detiene (alto)
     - Retrocede por 1 segundo
     - Gira a la derecha por 0.7 segundos
   - ✅ **NO detecta línea** → Avanza rápido atacando (`adelanter()`)

   #### Si distancia >= 150 cm (sin obstáculo cerca):
   - ✅ **Detecta línea negra** → Realiza maniobra evasiva (igual que arriba)
   - ✅ **NO detecta línea** → Avanza lento buscando (`adelantes()`)

---

## Funciones de Movimiento (idénticas)

| Función | Comportamiento | Valores PWM |
|---------|---------------|-------------|
| `adelanter()` | Avance rápido (ataque) | Sin PWM, velocidad máxima |
| `adelantes()` | Avance lento (búsqueda) | PWM 80 en motA0 |
| `atras()` | Retroceso | Velocidad máxima |
| `derecha()` | Giro a la derecha | Solo motor B activo |
| `izquierda()` | Giro a la izquierda | Solo motor A activo |

---

## Parámetros (idénticos)

- **Umbral de línea negra:** 550 (si sensor > 550 = línea detectada)
- **Distancia de obstáculo:** 150 cm
- **Rango válido ultrasónico:** 1-25 cm
- **Retrasos en maniobra:** 1000ms alto, 1000ms atrás, 700ms giro

---

## Diferencia Real

### FondoNegro.ino:
- Código más compacto
- Lógica directamente en el `loop()`
- Menos comentarios

### SumoAutonomoFondoNegro.ino:
- Código más organizado
- Funciones auxiliares (`leerSensoresLinea()`, `hayLineaDetectada()`, `maniobraEvitarBorde()`)
- Más comentarios explicativos
- Constantes con nombres descriptivos

---

## Conclusión

**El robot se comportará exactamente igual con cualquiera de los dos códigos.** 

La versión `SumoAutonomoFondoNegro.ino` es simplemente una refactorización más legible y mantenible de `FondoNegro.ino`, pero mantiene el mismo comportamiento funcional.



