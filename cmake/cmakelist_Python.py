import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import os
import re

# ==============================================================================
# SNIPPETS DE CÓDIGO (Librerías, Float/Math y Generación de Archivos)
# ==============================================================================
# Marcadores únicos por bloque: se usan tanto para escribir el encabezado
# como para detectar si el bloque ya fue agregado antes (evita duplicados).
MARCADORES = {
    "lib": "# --- BLOQUE LIBRERIAS AUTOMÁTICAS ---",
    "float": "# --- BLOQUE SOPORTE FLOAT Y MATH (-lm) ---",
    "hex": "# --- BLOQUE GENERACIÓN HEX Y BIN ---",
    "opt": "# --- BLOQUE OPTIMIZACIÓN",  # el nivel (-O2, -Os, etc.) va después, por eso se compara solo el prefijo
}

SNIPPETS = {
    "lib": """
#sino funciona ejecutar comando  CMake: Delete Cache and Reconfigure

#opciones para librerias
# BUSCAR LA CARPETA "librerias" EN CORE, CORE/SRC O EN LA RAÍZ DEL PROYECTO (Insensible a mayúsculas)
# La búsqueda en la raíz cubre el caso en que CubeMX no genera la
# estructura Core/Src (por ejemplo, al elegir el toolchain "CMake"),
# dejando los archivos generados directamente en la carpeta del proyecto.

set(RUTAS_DE_BUSQUEDA 
    "${CMAKE_SOURCE_DIR}/Core"
    "${CMAKE_SOURCE_DIR}/Core/Src"
    "${CMAKE_SOURCE_DIR}"
)

set(LIBRERIAS_DIR "")

#opciones para  carpeta de librerias
# Recorremos cada ruta posible (Core, Core/Src y finalmente la raíz del proyecto)
foreach(ruta ${RUTAS_DE_BUSQUEDA})
    # Solo buscamos si la ruta base existe y aún no hemos encontrado la carpeta
    if(EXISTS "${ruta}" AND NOT LIBRERIAS_DIR)
  
        file(GLOB contenido RELATIVE "${ruta}" "${ruta}/*")
        
        # Buscamos la carpeta ignorando mayúsculas/minúsculas
        foreach(item ${contenido})
            
            if(IS_DIRECTORY "${ruta}/${item}")
                string(TOLOWER "${item}" item_lower)
                if(item_lower STREQUAL "librerias")
                    set(LIBRERIAS_DIR "${ruta}/${item}")
                    break() # Encontramos la carpeta, salimos del bucle
                endif()
            endif()
        endforeach()
    endif()
endforeach()

# Mensaje para la consola de CMake y Fallback
if(LIBRERIAS_DIR)
    message(STATUS "Carpeta de librerías encontrada en: ${LIBRERIAS_DIR}")
else()
    message(WARNING "No se encontró la carpeta 'librerias' ni en Core/, Core/Src/ ni en la raíz del proyecto")
    # Fallback por defecto para evitar errores catastróficos en CMake
    set(LIBRERIAS_DIR "${CMAKE_SOURCE_DIR}/Core/librerias") 
endif()


#  INCLUIR FUENTES Y CABECERAS

# Buscar todos los .c en la carpeta encontrada y subcarpetas
file(GLOB_RECURSE LIBRERIAS_SOURCES CONFIGURE_DEPENDS
    "${LIBRERIAS_DIR}/*.c"
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
   ${LIBRERIAS_SOURCES}
)

# Buscar todos los .h en la carpeta encontrada y subcarpetas
file(GLOB_RECURSE LIBRERIAS_HEADERS CONFIGURE_DEPENDS
    "${LIBRERIAS_DIR}/*.h"
)
# Extraer las carpetas donde están esos .h
set(LIBRERIAS_INCLUDE_DIRS "")

foreach(header ${LIBRERIAS_HEADERS})
    get_filename_component(header_dir "${header}" DIRECTORY)
    list(APPEND LIBRERIAS_INCLUDE_DIRS "${header_dir}")
endforeach()

# Evitar errores si no hay headers
if(LIBRERIAS_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES LIBRERIAS_INCLUDE_DIRS)
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${LIBRERIAS_INCLUDE_DIRS}
)
#fin librerias
""",

    "float": """
# Habilitar soporte de float en printf/sprintf/scanf y vincular math.h (-lm)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -u _printf_float
    -lm
)
""",

    "hex": """
# Generar .hex y .bin automáticamente después de compilar
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMENT "Generando ${CMAKE_PROJECT_NAME}.hex y ${CMAKE_PROJECT_NAME}.bin"
)
"""
}

# ==============================================================================
# OPCIONES DE OPTIMIZACIÓN (Mapeadas a banderas de GCC para Release)
# ==============================================================================
OPTIMIZATION_MAP = {
    "-O2 (Por defecto - Recomendado para producción)": "-O2",
    "-O0 (Sin optimización - Depuración pura)": "-O0",
    "-O1 (Optimización básica)": "-O1",
    "-O3 (Optimización máxima - Rendimiento extremo)": "-O3",
    "-Os (Optimizar para tamaño - ¡Ahorra memoria Flash!)": "-Os",
    "-Og (Optimizar para depuración estándar)": "-Og"
}

def generar_bloque_optimizacion(flag_release):
    """Retorna tu bloque original modificando únicamente la bandera del branch de Release."""
    return f"""
# Optimización según tipo de build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${{CMAKE_PROJECT_NAME}} PRIVATE -Og -g3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${{CMAKE_PROJECT_NAME}} PRIVATE {flag_release} -g0)
endif()
"""

# ==============================================================================
# OPCIONES DE FPU (Hardware vs Software) - modifica gcc-arm-none-eabi.cmake
# ==============================================================================
# Por defecto siempre sale "Hardware" seleccionado (list(...)[0])
FPU_MAP = {
    "Hardware (FPU - rápido, recomendado)": "hard",
    "Software (sin FPU, más lento)": "soft",
}

# Línea final que queda escrita en el toolchain para cada modo
TARGET_FLAGS_HARD = 'set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ")\n'
TARGET_FLAGS_SOFT = 'set(TARGET_FLAGS "-mcpu=cortex-m4 -mfloat-abi=soft ")\n'

# Único núcleo sobre el que este toggle Hardware/Software tiene sentido.
# El F401/F411 (M4) tiene FPU; otros como M0/M0+/M3 no la tienen, y M7
# usa otra variante de -mfpu, así que ahí NO se debe tocar nada.
MCU_OBJETIVO = "cortex-m4"

# Detecta CUALQUIER línea "set(TARGET_FLAGS "...")" sin importar el
# contenido, para poder leer qué -mcpu tiene actualmente
PATRON_TARGET_FLAGS = re.compile(r'^(\s*set\(\s*TARGET_FLAGS\s+")(.*)("\s*\)\s*)$')
# Extrae el valor de -mcpu=xxxx dentro del contenido de la línea
PATRON_MCPU = re.compile(r'-mcpu=(\S+)')


def modificar_toolchain_fpu(ruta_toolchain, modo):
    """
    Busca la línea TARGET_FLAGS en cmake/gcc-arm-none-eabi.cmake.
    - Si el -mcpu detectado es cortex-m4, la reemplaza según el modo
      ('hard' o 'soft'). No duplica nada porque reemplaza in-place la
      línea existente, no agrega texto nuevo.
    - Si el -mcpu es cualquier otro (M0, M0+, M3, M7, etc.), NO toca el
      archivo y lo reporta como "ignorado" (no como error).
    Retorna (exito: bool, mensaje: str, cambio_real: bool)
    """
    if not os.path.exists(ruta_toolchain):
        return False, f"No se encontró el archivo:\n{ruta_toolchain}", False

    with open(ruta_toolchain, "r", encoding="utf-8") as f:
        lineas = f.readlines()

    encontrado = False
    cambio_real = False
    ignorado_por_mcu = None

    for i, linea in enumerate(lineas):
        m = PATRON_TARGET_FLAGS.match(linea)
        if not m:
            continue

        encontrado = True
        contenido_flags = m.group(2)
        mcpu_match = PATRON_MCPU.search(contenido_flags)
        mcpu_detectado = mcpu_match.group(1) if mcpu_match else "(no encontrado)"

        if mcpu_detectado != MCU_OBJETIVO:
            # Otro microcontrolador: se ignora a propósito, no es error
            ignorado_por_mcu = mcpu_detectado
            break

        nueva_linea = TARGET_FLAGS_HARD if modo == "hard" else TARGET_FLAGS_SOFT
        if linea != nueva_linea:
            lineas[i] = nueva_linea
            cambio_real = True
        break

    if not encontrado:
        return False, (
            "No se encontró ninguna línea 'set(TARGET_FLAGS \"...\")' "
            f"en:\n{ruta_toolchain}"
        ), False

    if ignorado_por_mcu is not None:
        return True, (
            f"El microcontrolador detectado es '{ignorado_por_mcu}' (no es {MCU_OBJETIVO}). "
            "No aplica FPU por hardware/software -> archivo no modificado."
        ), False

    if cambio_real:
        with open(ruta_toolchain, "w", encoding="utf-8") as f:
            f.writelines(lineas)

    modo_texto = "Hardware (FPU)" if modo == "hard" else "Software"
    if cambio_real:
        return True, f"TARGET_FLAGS actualizado a modo: {modo_texto}", True
    else:
        return True, f"TARGET_FLAGS ya estaba en modo: {modo_texto} (sin cambios)", False


# ==============================================================================
# CLASE PRINCIPAL DE LA INTERFAZ
# ==============================================================================
class CMakeGeneratorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Generador CMake - STM32_ALCIDES RAMOS")

        # --- Ventana más ancha de inicio y redimensionable a lo ancho ---
        self.root.geometry("680x500")
        self.root.minsize(580, 500)  # Límite mínimo para que no se colapse
        self.root.resizable(True, False)  # Permitir estirar a lo ancho (X), no a lo alto (Y)

        # Variables de estado
        self.folder_path = tk.StringVar()
        self.status_text = tk.StringVar(value="Esperando carpeta...")
        self.cmake_exists = False
        self.cmake_file_path = ""

        # Variables de selección (Activas por defecto)
        self.var_lib = tk.BooleanVar(value=True)
        self.var_float = tk.BooleanVar(value=True)
        self.var_hex = tk.BooleanVar(value=True)
        self.var_opt = tk.BooleanVar(value=True)
        self.var_fpu = tk.BooleanVar(value=True)

        # Guardar la opción por defecto
        self.var_opt_level = tk.StringVar(value=list(OPTIMIZATION_MAP.keys())[0])
        # Por defecto: Hardware (primer elemento del diccionario)
        self.var_fpu_mode = tk.StringVar(value=list(FPU_MAP.keys())[0])

        self.crear_interfaz()
        self.detectar_carpeta_inicial()

    def crear_interfaz(self):
        main_frame = tk.Frame(self.root, padx=20, pady=15)
        main_frame.pack(fill="both", expand=True)

        # --- SECCIÓN: CARPETA ---
        tk.Label(main_frame, text="Carpeta de tu proyecto STM32:", font=("Arial", 10, "bold")).pack(anchor="w")

        folder_frame = tk.Frame(main_frame)
        folder_frame.pack(fill="x", pady=5)

        self.entry_folder = tk.Entry(folder_frame, textvariable=self.folder_path, font=("Arial", 9))
        self.entry_folder.pack(side="left", fill="x", expand=True, padx=(0, 5))

        btn_buscar = tk.Button(folder_frame, text="Examinar...", command=self.seleccionar_carpeta, bg="#E0E0E0")
        btn_buscar.pack(side="right")

        # --- SECCIÓN: ESTADO ---
        self.lbl_status = tk.Label(main_frame, textvariable=self.status_text, font=("Arial", 9, "italic"), fg="#757575")
        self.lbl_status.pack(anchor="w", pady=(0, 10))

        # Divisor visual
        tk.Frame(main_frame, height=2, bd=1, relief="groove").pack(fill="x", pady=5)

        # --- SECCIÓN: OPCIONES ---
        tk.Label(main_frame, text="Opciones para agregar al CMakeLists.txt:", font=("Arial", 10, "bold")).pack(anchor="w", pady=(5, 10))

        chk_lib = tk.Checkbutton(main_frame, text="Incluir ruta de librerías automáticas (addlib)", variable=self.var_lib, font=("Arial", 9))
        chk_lib.pack(anchor="w", pady=3)

        chk_float = tk.Checkbutton(main_frame, text="Usar librerías float para sprintf y math.h (-lm)", variable=self.var_float, font=("Arial", 9))
        chk_float.pack(anchor="w", pady=3)

        chk_hex = tk.Checkbutton(main_frame, text="Generar archivos de salida .HEX y .BIN", variable=self.var_hex, font=("Arial", 9))
        chk_hex.pack(anchor="w", pady=3)

        # Optimización dinámica
        opt_frame = tk.Frame(main_frame)
        opt_frame.pack(fill="x", pady=3)

        chk_opt = tk.Checkbutton(opt_frame, text="Configurar optimización de Release:", variable=self.var_opt, font=("Arial", 9), command=self.toggle_opt_combobox)
        chk_opt.pack(side="left", anchor="w")

        self.combo_opt = ttk.Combobox(opt_frame, textvariable=self.var_opt_level, values=list(OPTIMIZATION_MAP.keys()), state="readonly")
        self.combo_opt.pack(side="left", fill="x", expand=True, padx=(10, 0))

        # Divisor visual antes de FPU
        tk.Frame(main_frame, height=2, bd=1, relief="groove").pack(fill="x", pady=(10, 5))

        tk.Label(main_frame, text="Cálculo de punto flotante (modifica gcc-arm-none-eabi.cmake):",
                 font=("Arial", 10, "bold")).pack(anchor="w", pady=(0, 8))

        # FPU: Hardware / Software
        fpu_frame = tk.Frame(main_frame)
        fpu_frame.pack(fill="x", pady=3)

        chk_fpu = tk.Checkbutton(fpu_frame, text="Configurar modo FPU:", variable=self.var_fpu, font=("Arial", 9), command=self.toggle_fpu_combobox)
        chk_fpu.pack(side="left", anchor="w")

        self.combo_fpu = ttk.Combobox(fpu_frame, textvariable=self.var_fpu_mode, values=list(FPU_MAP.keys()), state="readonly")
        self.combo_fpu.pack(side="left", fill="x", expand=True, padx=(10, 0))

        # --- SECCIÓN: ACCIÓN ---
        self.btn_aceptar = tk.Button(main_frame, text="Aceptar / Generar", command=self.procesar_cmakelist, font=("Arial", 11, "bold"), bg="#2196F3", fg="white", height=2)
        self.btn_aceptar.pack(fill="x", pady=(25, 0))

    def toggle_opt_combobox(self):
        self.combo_opt.config(state="readonly" if self.var_opt.get() else "disabled")

    def toggle_fpu_combobox(self):
        self.combo_fpu.config(state="readonly" if self.var_fpu.get() else "disabled")

    def detectar_carpeta_inicial(self):
        vscode_cwd = os.environ.get("VSCODE_CWD")
        if vscode_cwd and os.path.exists(vscode_cwd):
            self.actualizar_carpeta(vscode_cwd)
            return

        cwd = os.getcwd()
        if os.path.exists(os.path.join(cwd, "CMakeLists.txt")):
            self.actualizar_carpeta(cwd)

    def seleccionar_carpeta(self):
        carpeta = filedialog.askdirectory(title="Selecciona la carpeta raíz de tu proyecto STM32")
        if carpeta:
            self.actualizar_carpeta(carpeta)

    def actualizar_carpeta(self, ruta_carpeta):
        self.folder_path.set(ruta_carpeta)
        posible_cmake = os.path.join(ruta_carpeta, "CMakeLists.txt")

        if os.path.exists(posible_cmake):
            self.cmake_file_path = posible_cmake
            self.cmake_exists = True

            try:
                with open(posible_cmake, "r", encoding="utf-8") as f:
                    contenido = f.read()
                ya_agregados = [
                    nombre for nombre, marcador in [
                        ("librerías", MARCADORES["lib"]),
                        ("float/math", MARCADORES["float"]),
                        ("hex/bin", MARCADORES["hex"]),
                        ("optimización", MARCADORES["opt"]),
                    ] if marcador in contenido
                ]
            except Exception:
                ya_agregados = []

            if ya_agregados:
                self.status_text.set(
                    "✔ CMakeLists.txt detectado. Ya tiene: " + ", ".join(ya_agregados) +
                    " (no se duplicará al aceptar)."
                )
            else:
                self.status_text.set("✔ CMakeLists.txt detectado correctamente en la carpeta.")
            self.lbl_status.config(fg="#4CAF50")
            self.btn_aceptar.config(state="normal", bg="#2196F3")
        else:
            self.cmake_file_path = ""
            self.cmake_exists = False
            self.status_text.set("❌ Error: No se encontró 'CMakeLists.txt' en esta carpeta.")
            self.lbl_status.config(fg="#F44336")
            self.btn_aceptar.config(state="disabled", bg="#B0BEC5")

    def procesar_cmakelist(self):
        if not self.cmake_exists or not os.path.exists(self.cmake_file_path):
            messagebox.showerror("Error", "No hay un archivo CMakeLists.txt válido para modificar.")
            return

        if not (self.var_lib.get() or self.var_float.get() or self.var_hex.get()
                or self.var_opt.get() or self.var_fpu.get()):
            messagebox.showwarning("Atención", "Por favor, selecciona al menos una opción para inyectar.")
            return

        try:
            with open(self.cmake_file_path, "r", encoding="utf-8") as archivo:
                contenido_actual = archivo.read()
        except Exception as e:
            messagebox.showerror("Error de lectura", f"No se pudo leer el archivo:\n{str(e)}")
            return

        # Para cada opción marcada, se arma (encabezado, snippet) solo si su
        # marcador NO está ya presente en el archivo. Así nunca se duplica,
        # sin importar cuántas veces vuelvas a correr el script.
        bloques_a_escribir = []
        bloques_ya_existentes = []

        if self.var_lib.get():
            if MARCADORES["lib"] in contenido_actual:
                bloques_ya_existentes.append("Librerías automáticas")
            else:
                bloques_a_escribir.append((MARCADORES["lib"], SNIPPETS["lib"]))

        if self.var_float.get():
            if MARCADORES["float"] in contenido_actual:
                bloques_ya_existentes.append("Soporte float/math (-lm)")
            else:
                bloques_a_escribir.append((MARCADORES["float"], SNIPPETS["float"]))

        if self.var_hex.get():
            if MARCADORES["hex"] in contenido_actual:
                bloques_ya_existentes.append("Generación HEX/BIN")
            else:
                bloques_a_escribir.append((MARCADORES["hex"], SNIPPETS["hex"]))

        if self.var_opt.get():
            if MARCADORES["opt"] in contenido_actual:
                bloques_ya_existentes.append("Optimización de Release")
            else:
                seleccion_texto = self.var_opt_level.get()
                flag_release = OPTIMIZATION_MAP[seleccion_texto]
                snippet_opt = generar_bloque_optimizacion(flag_release)
                encabezado_opt = f'{MARCADORES["opt"]} ({flag_release}) ---'
                bloques_a_escribir.append((encabezado_opt, snippet_opt))

        # --- FPU: esto NO se agrega al CMakeLists.txt, modifica el toolchain ---
        fpu_resultado = None
        if self.var_fpu.get():
            ruta_carpeta = self.folder_path.get()
            ruta_toolchain = os.path.join(ruta_carpeta, "cmake", "gcc-arm-none-eabi.cmake")
            modo = FPU_MAP[self.var_fpu_mode.get()]
            fpu_resultado = modificar_toolchain_fpu(ruta_toolchain, modo)

        if not bloques_a_escribir and fpu_resultado is None:
            messagebox.showinfo(
                "Nada que hacer",
                "Todas las opciones seleccionadas ya estaban agregadas.\nNo se modificó ningún archivo."
            )
            return

        try:
            if bloques_a_escribir:
                with open(self.cmake_file_path, "a", encoding="utf-8") as archivo:
                    for encabezado, snippet in bloques_a_escribir:
                        archivo.write(f"\n\n{encabezado}\n{snippet}")

            nombres_agregados = []
            for encabezado, _ in bloques_a_escribir:
                if encabezado == MARCADORES["lib"]:
                    nombres_agregados.append("Librerías automáticas")
                elif encabezado == MARCADORES["float"]:
                    nombres_agregados.append("Soporte float/math (-lm)")
                elif encabezado == MARCADORES["hex"]:
                    nombres_agregados.append("Generación HEX/BIN")
                elif encabezado.startswith(MARCADORES["opt"]):
                    nombres_agregados.append("Optimización de Release")

            mensaje = f"Configuraciones procesadas en:\n{self.cmake_file_path}\n\n"
            if nombres_agregados:
                mensaje += "✔ Agregado a CMakeLists.txt:\n  - " + "\n  - ".join(nombres_agregados) + "\n\n"
            if bloques_ya_existentes:
                mensaje += "⏭ Ya existía (no se tocó):\n  - " + "\n  - ".join(bloques_ya_existentes) + "\n\n"

            if fpu_resultado is not None:
                exito_fpu, msg_fpu, _ = fpu_resultado
                if exito_fpu:
                    mensaje += f"✔ FPU (gcc-arm-none-eabi.cmake):\n  - {msg_fpu}"
                else:
                    mensaje += f"⚠ FPU (gcc-arm-none-eabi.cmake):\n  - {msg_fpu}"

            messagebox.showinfo("Éxito", mensaje.strip())
            self.root.destroy()

        except Exception as e:
            messagebox.showerror("Error de escritura", f"No se pudo escribir en el archivo:\n{str(e)}")


if __name__ == "__main__":
    root = tk.Tk()
    app = CMakeGeneratorApp(root)
    root.mainloop()